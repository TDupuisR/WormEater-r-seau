#include <enet6/enet.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <vector>
#include <experimental/random>

#include "sv_players.hpp"
#include "sv_constant.hpp"
#include "sv_protocol.hpp"

using n_clock = std::chrono::steady_clock;

// using n_timepoint = std::chrono::time_point;
// using n_ms = std::chrono::milliseconds;

struct GameData
{
    GAME_STATE state = GAME_STATE::waiting;
    std::vector<PlayerData> players;
};

ENetPacket* build_playerposition_packet(GameData& a_gameData, const PlayerData& a_player, bool a_reliable = false)
{
    PlayersPositionPacket packet;

    for (PlayerData& player : a_gameData.players)
    {
        auto& packetPlayer = packet.players.emplace_back();
        packetPlayer.id = player.id;
        packetPlayer.position = player.position;
        packetPlayer.velocity = player.velocity;
        packetPlayer.inputs = player.inputs.direction;
    }

    packet.lastInputIndex = a_player.inputs.inputIndex;

    if (a_reliable)
        return build_packet(packet, ENET_PACKET_FLAG_RELIABLE);
    else
        return build_packet(packet, 0);
}

void tick_logic(GameData a_gameData, float a_deltaTime)
{
    for (PlayerData player : a_gameData.players)
    {
        if (!player.name.empty())
        {
            UpdatePhysics(player, a_deltaTime);
        }
    }
} 

void tick_network(GameData a_gameData, float a_deltaTime)
{
    for (const PlayerData& player : a_gameData.players)
    {
        if (player.peer != nullptr && !player.name.empty())
        {
            ENetPacket* packet = build_playerposition_packet(a_gameData, player);
			enet_peer_send(player.peer, 0, packet);
        }
    }
}

void handle_message(PlayerData& player, const byteArray_t& message, GameData& gameData)
{
    std::size_t offset = 0;
    
    OP_CODE opcode = static_cast<OP_CODE>(Deserialize_u8(message, offset));
    //std::cout << "Handle Message - opcode : " << static_cast<int>(opcode) << "\n" << std::flush;
    if (player.name.empty() && opcode != OP_CODE::C_PlayerInfo)
    {
        enet_peer_disconnect_now(player.peer, 0);
        player.peer = nullptr;
        std::cout << "Player #" << player.id << " did not sent PlayerInfo packet as intented!\nDisconnecting player\n" << std::flush;
        return;
    }

    switch (opcode)
    {
        case OP_CODE::C_PlayerInfo:
        {
            PlayerInfoPacket packet = PlayerInfoPacket::Deserialize(message, offset);

            if (packet.name.size() > MaxPlayerNameLength)
            {
                packet.name.resize(MaxPlayerNameLength);
            }
            if (packet.name.empty())
            {
                std::cout << "Player #" << player.id << " tried renaming itself, but given name is empty!\n" << std::flush;

                if (player.name.empty())
                {
                    enet_peer_disconnect_now(player.peer, 0);
                    player.peer = nullptr;
                    std::cout << "Player #" << player.id << " did not sent a correct name!\nDisconnecting player\n" << std::flush;
                }

                break;
            }

            if (player.name.empty())
            {
                player.name = packet.name;
                std::cout << "Player #" << player.id << " joined as " << player.name << "\n" << std::flush;

                GameDataPacket gameDataPacket;
                gameDataPacket.playerId = player.id;

                enet_peer_send(player.peer, 0, build_packet(gameDataPacket, ENET_PACKET_FLAG_RELIABLE));
            }
            else
            {
                player.name = packet.name;
                std::cout << "Player #" << player.id << " renamed itself as " << player.name << "\n" << std::flush;
            }
            break;
        }
        case OP_CODE::C_PlayerInput:
        {
            PlayerInputPacket packet = PlayerInputPacket::Deserialize(message, offset);

            player.inputs = packet.inputs;
        }

        case OP_CODE::Unexpected:
        default:
            std::cerr << "Handle Message : Unexpected opcode (" << static_cast<int>(opcode) <<")\n" << std::flush;
            break;
    }
}

int main(int argc, char** argv)
{
    enet_uint16 port;
    if (argc != 2)
    {
        port = (enet_uint16)std::experimental::randint(minPort, maxPort);
        std::cout << "No port given, random port assigned...\n" << std::flush;
    }
    else
    {
        port = (enet_uint16)std::atoi(argv[1]);
    }

    if (enet_initialize() != 0)
    {
        std::cout << "Failed to initialize ENet\n" << std::flush;
        return EXIT_FAILURE;
    }

    //création hôte server
    ENetAddress address;
    ENetHost* host;

    enet_address_build_any(&address, ENET_ADDRESS_TYPE_IPV6);
    address.port = port;


    host = enet_host_create(ENET_ADDRESS_TYPE_ANY, &address, 16, 0, 0, 0);
    if (!host)
    {
        std::cerr << "Failed to create ENet host\n" << std::flush;
		return EXIT_FAILURE;
    }

    std::cout << "Server creation success!\n" << std::flush;
    std::cout << "Port : " << port << "\n" << std::flush;

    GameData gameData;

    //Init clock
    std::chrono::time_point lastTickLogic = n_clock::now();
    std::chrono::time_point lastTickNetwork = n_clock::now();

    std::chrono::milliseconds LogicTickRate = std::chrono::milliseconds(TICK_LOGIC_DELAY);
    std::chrono::milliseconds NetworkTickRate = std::chrono::milliseconds(TICK_NETWORK_DELAY);

    std::cout << "Starting Server loop...\n" << std::flush;
    while (true)
    {
        std::chrono::time_point now = n_clock::now();
        
        ENetEvent event;
        if (enet_host_service(host, &event, 1) > 0)
        {
            do
            {
                // std::cout << "Packet received\n" << std::flush;

                switch (event.type)
                {
                    case ENET_EVENT_TYPE_CONNECT:
                    {
                        auto it = std::find_if(gameData.players.begin(), gameData.players.end(), [&](const PlayerData& player) { return player.peer == nullptr; });
                        if (it == gameData.players.end()) // Pas de Slot libre
                        {
                            gameData.players.emplace_back((idSize_t)gameData.players.size());
                            it = gameData.players.end() - 1; // Set l'itérateur sur le nouvelle élément
                        }

                        PlayerData& player = *it;
                        player.peer = event.peer; // Association du joueur à son peer

                        player.name.clear();
                        player.state = PLAYER_STATE::connecting;
                        std::cout << "Player #" << static_cast<int>(player.id) << " Connected! " << "\n" << std::flush;
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT:
					case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
					{
                        auto it = std::find_if(gameData.players.begin(), gameData.players.end(), [&](const PlayerData& player) { return player.peer == event.peer; });
                        if (it == gameData.players.end())
                        {
                            break;
                        }

                        PlayerData& player = *it;

                        std::cout << "Player #" << static_cast<int>(player.id) << " [" << player.name << "] disconnected ";
                        if (event.type == ENET_EVENT_TYPE_DISCONNECT_TIMEOUT)
                            std::cout << "(time out)";
                        std::cout << "\n" << std::flush;

                        player.peer = nullptr;

                        if (!player.name.empty())
                        {
                            //Envoyer le message aux autres joueurs
                        }

                        //Check l'état du jeu / s'il y a encore des joueurs connecté

                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                    {
                        auto it = std::find_if(gameData.players.begin(), gameData.players.end(), [&](const PlayerData& player) { return player.peer == event.peer; });
                        if (it == gameData.players.end())
                        {
                            break;
                        }

                        PlayerData& player = *it;

                        byteArray_t content(event.packet->dataLength);
                        std::memcpy(content.data(), event.packet->data, event.packet->dataLength);

                        handle_message(player, content, gameData);

                        enet_packet_destroy(event.packet);
                        break;
                    }
                    case ENET_EVENT_TYPE_NONE:
                    default:
                    {
						// n'est pas censé se produire
						std::cout << "unexpected ENet event\n" << std::flush;
						break;
                    }
                }
            } while (enet_host_check_events(host, &event) > 0);
        } 
        
        auto deltaLogic = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickLogic);
        if (deltaLogic >= LogicTickRate)
        {
            TickLogic(gameData, std::chrono::duration<float>(deltaLogic).count());

            lastTickLogic = now;
        }

        auto deltaNetwork = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTickLogic);
        if (deltaLogic >= NetworkTickRate)
        {
            TickLogic(gameData, std::chrono::duration<float>(deltaNetwork).count());

            lastTickLogic = now;
        }
    }
}