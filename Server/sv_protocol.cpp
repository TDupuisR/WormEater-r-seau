#include "sv_protocol.hpp"

#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <WinSock2.h>
#else
//#include <arpa/inet.h>

inline float ntohf(uint32_t net32)
{
    uint32_t u32 = ntohl(net32);
    return std::bit_cast<float>(u32);
}
inline uint32_t htonf(float f32)
{
    uint32_t u32 = std::bit_cast<uint32_t>(f32);
    return htonl(u32);
}
#endif

#pragma region Serialization

void Serialize_f32(std::vector<std::uint8_t> &byteArray, float value)
{
    std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));

	return Serialize_f32(byteArray, offset, value);
}
void Serialize_f32(std::vector<std::uint8_t> &byteArray, std::size_t offset, float value)
{
    std::uint32_t v = htonf(value);

	if (offset + sizeof(v) > byteArray.size())
    {
        std::cerr << "ERROR -> Serialize_f32 : Cannot continue serialization, offset is bigger than byteArray.size()\n" << std::flush;
        return;
    }
	std::memcpy(&byteArray[offset], &v, sizeof(v));
}

void Serialize_i8(std::vector<std::uint8_t> &byteArray, std::int8_t value)
{
    return Serialize_u8(byteArray, static_cast<std::uint8_t>(value));
}
void Serialize_i8(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::int8_t value)
{
    return Serialize_u8(byteArray, offset, static_cast<std::uint8_t>(value));
}
void Serialize_u8(std::vector<std::uint8_t> &byteArray, std::uint8_t value)
{
    std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));

	return Serialize_u8(byteArray, offset, value);
}
void Serialize_u8(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::uint8_t value)
{
    if (offset >= byteArray.size())
    {
        std::cerr << "ERROR -> Serialize_u8 : Cannot continue serialization, offset is bigger than byteArray.size()\n" << std::flush;
        return;
    }
	byteArray[offset] = value;
}

void Serialize_i16(std::vector<std::uint8_t> &byteArray, std::int16_t value)
{
    return Serialize_u16(byteArray, static_cast<std::uint16_t>(value));
}
void Serialize_i16(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::int16_t value)
{
    return Serialize_u16(byteArray, offset, static_cast<std::uint16_t>(value));
}
void Serialize_u16(std::vector<std::uint8_t> &byteArray, std::uint16_t value)
{
    std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));

	return Serialize_u16(byteArray, offset, value);
}
void Serialize_u16(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::uint16_t value)
{
    value = htons(value);

	if (offset + sizeof(value) > byteArray.size())
    {
        std::cerr << "ERROR -> Serialize_u16 : Cannot continue serialization, offset is bigger than byteArray.size()\n" << std::flush;
        return;
    }
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void Serialize_i32(std::vector<std::uint8_t> &byteArray, std::int32_t value)
{
    return Serialize_u32(byteArray, static_cast<std::uint32_t>(value));
}
void Serialize_i32(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::int32_t value)
{
    return Serialize_u32(byteArray, offset, static_cast<std::uint32_t>(value));
}
void Serialize_u32(std::vector<std::uint8_t> &byteArray, std::uint32_t value)
{
	std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(value));

	return Serialize_u32(byteArray, offset, value);
}
void Serialize_u32(std::vector<std::uint8_t> &byteArray, std::size_t offset, std::uint32_t value)
{
    value = htonl(value);

	if (offset + sizeof(value) > byteArray.size())
    {
        std::cerr << "ERROR -> Serialize_u32 : Cannot continue serialization, offset is bigger than byteArray.size()\n" << std::flush;
        return;
    }
	std::memcpy(&byteArray[offset], &value, sizeof(value));
}

void Serialize_str(std::vector<std::uint8_t> &byteArray, const std::string &value)
{
    std::size_t offset = byteArray.size();
	byteArray.resize(offset + sizeof(std::uint32_t) + value.size());
	return Serialize_str(byteArray, offset, value);
}
void Serialize_str(std::vector<std::uint8_t> &byteArray, std::size_t offset, const std::string &value)
{
    Serialize_u32(byteArray, offset, static_cast<std::uint32_t>(value.size()));
	offset += sizeof(std::uint32_t);

	if (!value.empty())
		std::memcpy(&byteArray[offset], value.data(), value.size());
}

#pragma endregion

#pragma region Deserialization

float Deserialize_f32(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    std::uint32_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));

	float v = ntohf(value);
	offset += sizeof(value);

	return v;
}

std::int8_t Deserialize_i8(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    return static_cast<std::int8_t>(Deserialize_u8(byteArray, offset));
}
std::uint8_t Deserialize_u8(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    std::uint8_t value = byteArray[offset];
	offset += sizeof(value);

	return value;
}

std::int16_t Deserialize_i16(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    return static_cast<std::int16_t>(Deserialize_u16(byteArray, offset));
}
std::uint16_t Deserialize_u16(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    std::uint16_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	value = ntohs(value);

	offset += sizeof(value);

	return value;
}

std::int32_t Deserialize_i32(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    return static_cast<std::int32_t>(Deserialize_u32(byteArray, offset));
}
std::uint32_t Deserialize_u32(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    std::uint32_t value;
	std::memcpy(&value, &byteArray[offset], sizeof(value));
	value = ntohl(value);

	offset += sizeof(value);

	return value;
}

std::string Deserialize_str(const std::vector<std::uint8_t> &byteArray, std::size_t &offset)
{
    std::uint32_t length = Deserialize_u32(byteArray, offset);
	std::string str(length, ' ');
	std::memcpy(&str[0], &byteArray[offset], length);

	offset += length;

	return str;
}

#pragma endregion

#pragma region OP_COPE messages

void PlayerInfoPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_str(byteArray, name);
}
PlayerInfoPacket PlayerInfoPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayerInfoPacket packet;

    packet.name = Deserialize_str(byteArray, offset);

    return packet;
}

void PlayerInputPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_f32(byteArray, inputs.direction.x);
    Serialize_f32(byteArray, inputs.direction.y);

    std::uint8_t actionByte = 0;
    actionByte |= (std::uint8_t) inputs.jump << 0;
    actionByte |= (std::uint8_t) inputs.interact << 1;
    Serialize_u8(byteArray, actionByte);

    Serialize_u32(byteArray, inputs.inputIndex);
}
PlayerInputPacket PlayerInputPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayerInputPacket packet;

    packet.inputs.direction.x = Deserialize_f32(byteArray, offset);
    packet.inputs.direction.y = Deserialize_f32(byteArray, offset);

    std::uint8_t actionByte = Deserialize_u8(byteArray, offset);
    packet.inputs.jump = (actionByte & (1 << 0)) != 0;
    packet.inputs.interact = (actionByte & (1 << 1)) != 0;

    packet.inputs.inputIndex = Deserialize_u32(byteArray, offset);

    return packet;
}

void PlayerReadyPacket::Serialize(byteArray_t &byteArray) const
{
    std::uint8_t infoByte = 0;
    infoByte |= (std::uint8_t) isReady << 0;
    Serialize_u8(byteArray, infoByte);
}
PlayerReadyPacket PlayerReadyPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayerReadyPacket packet;
    
    std::uint8_t infoByte = Deserialize_u8(byteArray, offset);
    packet.isReady = (infoByte & (1 << 0)) != 0;

    return packet;
}

void GameDataPacket::Serialize(byteArray_t &byteArray) const
{
    //TODO Recheck if necessary the bytesize of idSize_t in sv_constant
    Serialize_u8(byteArray, playerId);
}
GameDataPacket GameDataPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    GameDataPacket packet;
    packet.playerId = Deserialize_u8(byteArray, offset);

    return packet;
}

void WormAttackPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, targetId.size());
    for (const auto& target : targetId)
    {
        Serialize_u8(byteArray, target);
    }

    Serialize_f32(byteArray, attackPosition.x);
    Serialize_f32(byteArray, attackPosition.y);
    Serialize_f32(byteArray, attackPosition.z);
}
WormAttackPacket WormAttackPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    WormAttackPacket packet;

    packet.targetId.resize(Deserialize_u16(byteArray, offset));
    for (auto& target : packet.targetId)
    {
        target =Deserialize_u8(byteArray, offset);
    }

    packet.attackPosition.x = Deserialize_f32(byteArray, offset);
    packet.attackPosition.y = Deserialize_f32(byteArray, offset);
    packet.attackPosition.z = Deserialize_f32(byteArray, offset);

    return packet;
}

void PlayerListPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, players.size());
    for (const auto& player : players)
    {
        Serialize_u8(byteArray, player.id);
        Serialize_str(byteArray, player.name);
    }
}
PlayerListPacket PlayerListPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayerListPacket packet;

    packet.players.resize(Deserialize_u16(byteArray, offset));
    for (auto& player : packet.players)
    {
        player.id = Deserialize_u8(byteArray, offset);
        player.name = Deserialize_str(byteArray, offset);
    }

    return packet;
}

void PlayersPositionPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, players.size());
    for (const auto& player : players)
    {
        Serialize_u8(byteArray, player.id);

        Serialize_f32(byteArray, player.position.x);
        Serialize_f32(byteArray, player.position.y);
        Serialize_f32(byteArray, player.position.z);

        Serialize_f32(byteArray, player.velocity.x);
        Serialize_f32(byteArray, player.velocity.y);
        Serialize_f32(byteArray, player.velocity.z);

        Serialize_f32(byteArray, player.inputs.x);
        Serialize_f32(byteArray, player.inputs.y);
    }

    Serialize_u32(byteArray, lastInputIndex);
}
PlayersPositionPacket PlayersPositionPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayersPositionPacket packet;

    packet.players.resize(Deserialize_u16(byteArray, offset));
    for (auto& player : packet.players)
    {
        player.id = Deserialize_u8(byteArray, offset);

        player.position.x = Deserialize_f32(byteArray, offset);
        player.position.y = Deserialize_f32(byteArray, offset);
        player.position.z = Deserialize_f32(byteArray, offset);

        player.velocity.x = Deserialize_f32(byteArray, offset);
        player.velocity.y = Deserialize_f32(byteArray, offset);
        player.velocity.z = Deserialize_f32(byteArray, offset);

        player.inputs.x = Deserialize_f32(byteArray, offset);
        player.inputs.y = Deserialize_f32(byteArray, offset);
    }

    packet.lastInputIndex = Deserialize_u32(byteArray, offset);

    return packet;
}

void CountDownPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, countdown);
}
CountDownPacket CountDownPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    CountDownPacket packet;

    packet.countdown = Deserialize_u16(byteArray, offset);

    return packet;
}

void PlayersMakeSoundPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u8(byteArray, id);
    Serialize_f32(byteArray, position.x);
    Serialize_f32(byteArray, position.y);
    Serialize_f32(byteArray, position.z);
}
PlayersMakeSoundPacket PlayersMakeSoundPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    PlayersMakeSoundPacket packet;

    packet.id = Deserialize_u8(byteArray, offset);
    packet.position.x = Deserialize_f32(byteArray, offset);
    packet.position.y = Deserialize_f32(byteArray, offset);
    packet.position.z = Deserialize_f32(byteArray, offset);

    return packet;
}

void WormNearPacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_f32(byteArray, nearRatio);
}
WormNearPacket WormNearPacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    WormNearPacket packet;

    packet.nearRatio = Deserialize_f32(byteArray, offset);

    return packet;
}

void WaitingStatePacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, players.size());
    for (const auto& player : players)
    {
        Serialize_u8(byteArray, player.id);

        Serialize_f32(byteArray, player.position.x);
        Serialize_f32(byteArray, player.position.y);
        Serialize_f32(byteArray, player.position.z);
    }
}
WaitingStatePacket WaitingStatePacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    WaitingStatePacket packet;

    packet.players.resize(Deserialize_u16(byteArray, offset));
    for (auto& player : packet.players)
    {
        player.id = Deserialize_u8(byteArray, offset);

        player.position.x = Deserialize_f32(byteArray, offset);
        player.position.y = Deserialize_f32(byteArray, offset);
        player.position.z = Deserialize_f32(byteArray, offset);
    }

    return packet;
}

void GameStartStatePacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, players.size());
    for (const auto& player : players)
    {
        Serialize_u8(byteArray, player.id);

        Serialize_f32(byteArray, player.position.x);
        Serialize_f32(byteArray, player.position.y);
        Serialize_f32(byteArray, player.position.z);

        Serialize_u8(byteArray, player.state);
    }

    Serialize_i16(byteArray, countdown);
}
GameStartStatePacket GameStartStatePacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    GameStartStatePacket packet;

    packet.players.resize(Deserialize_u16(byteArray, offset));
    for (auto& player : packet.players)
    {
        player.id = Deserialize_u8(byteArray, offset);

        player.position.x = Deserialize_f32(byteArray, offset);
        player.position.y = Deserialize_f32(byteArray, offset);
        player.position.z = Deserialize_f32(byteArray, offset);
        
        player.state = Deserialize_u8(byteArray, offset);
    }

    packet.countdown = Deserialize_i16(byteArray, offset);

    return packet;
}

void WormArriveStatePacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u8(byteArray, wormId);
    Serialize_i16(byteArray, coutdown);
}
WormArriveStatePacket WormArriveStatePacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    WormArriveStatePacket packet;

    packet.wormId = Deserialize_u8(byteArray, offset);
    packet.coutdown = Deserialize_i16(byteArray, offset);

    return packet;
}

void FinishedStatePacket::Serialize(byteArray_t &byteArray) const
{
    Serialize_u16(byteArray, players.size());
    for (const auto& player : players)
    {
        Serialize_u8(byteArray, player.id);
        Serialize_str(byteArray, player.name);
    }
}
FinishedStatePacket FinishedStatePacket::Deserialize(const byteArray_t &byteArray, std::size_t &offset)
{
    FinishedStatePacket packet;

    packet.players.resize(Deserialize_u16(byteArray, offset));
    for (auto& player : packet.players)
    {
        player.id = Deserialize_u8(byteArray, offset);
        player.name = Deserialize_str(byteArray, offset);
    }

    return packet;
}

#pragma endregion

