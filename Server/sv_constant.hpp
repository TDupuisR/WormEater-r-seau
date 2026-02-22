#ifndef _SV_CONSTANT_HPP
#define _SV_CONSTANT_HPP 1

#include <cstdint>

typedef std::vector<std::uint8_t> byteArray_t;

typedef std::uint8_t idSize_t;

const std::uint16_t minPort = 1023;
const std::uint16_t maxPort = 65535;

constexpr int TICK_LOGIC_DELAY = 1000 / 30;
constexpr int TICK_NETWORK_DELAY = 1000 / 10;

constexpr std::size_t MaxPlayerNameLength = 24;

enum class PLAYER_STATE : std::uint8_t
{
    unexpected,
    connecting,
    waiting,
    ready,
    human,
    worm,
    dead
};

enum class GAME_STATE : std::uint8_t
{
    waiting,
    game,
    finished
};

#pragma region PlayerPhysics

    constexpr float GroundingTolerance = 0.2f;

    // Worm
    constexpr float WVMax = 8.0f;
    constexpr float WAcceleration = 1.5f;
    constexpr float WDeceleration = 0.5f;

    constexpr float WGroundLevel = -5.0f;

    // Human
    constexpr float HVMax = 5.0f;
    constexpr float HAcceleration = 2.0f;
    constexpr float HDeceleration = 1.0f;

    constexpr float HGroundLevel = 1.0f;

    constexpr float HJumpPower = 6.0f;
    constexpr float HGravity = 9.81f;

    inline float GetVMax(bool IsWorm)
    {
        return IsWorm ? WVMax : HVMax;
    }
    inline float GetAcceleration(bool IsWorm)
    {
        return IsWorm ? WAcceleration : HAcceleration;
    }
    inline float GetDeceleration(bool IsWorm)
    {
        return IsWorm ? WDeceleration : HDeceleration;
    }
    inline float GetGroundLevel(bool IsWorm)
    {
        return IsWorm ? WGroundLevel : HGroundLevel;
    }
    
#pragma endregion

#endif //_SV_CONSTANT_HPP