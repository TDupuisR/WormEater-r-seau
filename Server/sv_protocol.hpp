#ifndef _SV_PROTOCOL_HPP
#define _SV_PROTOCOL_HPP 1

#include <cstdint>
#include <string>
#include <vector>

#include "sv_constant.hpp"
#include "sv_players.hpp"

#pragma region Serialization

void Serialize_f32(std::vector<std::uint8_t>& byteArray, float value);
void Serialize_f32(std::vector<std::uint8_t>& byteArray, std::size_t offset, float value);
void Serialize_i8(std::vector<std::uint8_t>& byteArray, std::int8_t value);
void Serialize_i8(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int8_t value);
void Serialize_u8(std::vector<std::uint8_t>& byteArray, std::uint8_t value);
void Serialize_u8(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint8_t value);
void Serialize_i16(std::vector<std::uint8_t>& byteArray, std::int16_t value);
void Serialize_i16(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int16_t value);
void Serialize_u16(std::vector<std::uint8_t>& byteArray, std::uint16_t value);
void Serialize_u16(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint16_t value);
void Serialize_i32(std::vector<std::uint8_t>& byteArray, std::int32_t value); 
void Serialize_i32(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::int32_t value);
void Serialize_u32(std::vector<std::uint8_t>& byteArray, std::uint32_t value);
void Serialize_u32(std::vector<std::uint8_t>& byteArray, std::size_t offset, std::uint32_t value);
void Serialize_str(std::vector<std::uint8_t>& byteArray, const std::string& value);
void Serialize_str(std::vector<std::uint8_t>& byteArray, std::size_t offset, const std::string& value);

float Deserialize_f32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int8_t Deserialize_i8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint8_t Deserialize_u8(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int16_t Deserialize_i16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint16_t Deserialize_u16(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::int32_t Deserialize_i32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::uint32_t Deserialize_u32(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);
std::string Deserialize_str(const std::vector<std::uint8_t>& byteArray, std::size_t& offset);

#pragma endregion

#pragma region OP_COPE messages

enum class OP_CODE : std::uint8_t
{
    Unexpected,

    C_PlayerInfo,
    C_PlayerInput,
    C_PlayerReady,

    S_GameData,
    S_WormAttack,
    S_PlayerList,
    S_PlayerPosition,
    S_Countdown,
    S_PlayerMakeSound,
    S_WormNear,
    S_WaitingState,
    S_GameStartState,
    S_WormArriveState,
    S_FinishedState
};

struct PlayerInfoPacket
{
    static constexpr OP_CODE opcode = OP_CODE::C_PlayerInfo;

    std::string name;
    // Personalistion

    void Serialize(byteArray_t& byteArray) const;
    static PlayerInfoPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct PlayerInputPacket
{
    static constexpr OP_CODE opcode = OP_CODE::C_PlayerInput;

    PlayerInputs inputs;

    void Serialize(byteArray_t& byteArray) const;
    static PlayerInputPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct PlayerReadyPacket
{
    static constexpr OP_CODE opcode = OP_CODE::C_PlayerReady;

    bool isReady;

    void Serialize(byteArray_t& byteArray) const;
    static PlayerReadyPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct GameDataPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_GameData;

    idSize_t playerId;

    void Serialize(byteArray_t& byteArray) const;
    static GameDataPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct WormAttackPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_WormAttack;

    std::vector<idSize_t> targetId; // Empty if None
    Vector3f attackPosition;

    void Serialize(byteArray_t& byteArray) const;
    static WormAttackPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct PlayerListPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_PlayerList;

    struct Player
    {
        idSize_t id;
        std::string name;
        // Personnalisation
    };

    std::vector<Player> players;

    void Serialize(byteArray_t& byteArray) const;
    static PlayerListPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct PlayersPositionPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_PlayerPosition;

    struct PlayerData
    {
        idSize_t id;
        Vector3f position;
        Vector3f velocity;
        Vector2f inputs;
    };

    std::vector<PlayerData> players;
    std::uint32_t lastInputIndex; // Last input of sended player

    void Serialize(byteArray_t& byteArray) const;
    static PlayersPositionPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct CountDownPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_Countdown;

    std::uint16_t countdown;

    void Serialize(byteArray_t& byteArray) const;
    static CountDownPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct PlayersMakeSoundPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_PlayerMakeSound;

    idSize_t id;
    Vector3f position;

    void Serialize(byteArray_t& byteArray) const;
    static PlayersMakeSoundPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct WormNearPacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_PlayerMakeSound;

    float nearRatio;

    void Serialize(byteArray_t& byteArray) const;
    static WormNearPacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct WaitingStatePacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_WaitingState;

    struct PlayerData
    {
        idSize_t id;
        Vector3f position;
    };

    std::vector<PlayerData> players;

    void Serialize(byteArray_t& byteArray) const;
    static WaitingStatePacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct GameStartStatePacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_GameStartState;

    struct PlayerData
    {
        idSize_t id;
        Vector3f position;
        std::uint8_t state;
    };

    std::vector<PlayerData> players;
    std::int16_t countdown;

    void Serialize(byteArray_t& byteArray) const;
    static GameStartStatePacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct WormArriveStatePacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_GameStartState;

    idSize_t wormId;
    std::int16_t coutdown;

    void Serialize(byteArray_t& byteArray) const;
    static WormArriveStatePacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

struct FinishedStatePacket
{
    static constexpr OP_CODE opcode = OP_CODE::S_GameStartState;

    struct Player
    {
        idSize_t id;
        std::string name;
    };

    std::vector<Player> players;

    void Serialize(byteArray_t& byteArray) const;
    static FinishedStatePacket Deserialize(const byteArray_t& byteArray, std::size_t& offset);
};

#pragma endregion

template<typename T> ENetPacket* build_packet(const T& packet, enet_uint32 flags)
{
	// On sérialise l'opcode puis le contenu du packet dans un std::vector<std::uint8_t>
	std::vector<std::uint8_t> byteArray;

	Serialize_u8(byteArray, static_cast<std::uint8_t>(T::opcode));
	packet.Serialize(byteArray);

	// On copie le contenu de ce vector dans un packet enet, et on l'envoie au peer
	return enet_packet_create(byteArray.data(), byteArray.size(), flags);
}

#endif //_SV_PROTOCOL_HPP