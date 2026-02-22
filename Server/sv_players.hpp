#ifndef _SV_PLAYERS_HPP
#define _SV_PLAYERS_HPP 1

#include <cstdint>
#include <string>
#include <enet6/enet.h>

#include "sv_math.hpp"
#include "sv_constant.hpp"

#pragma region deprecated

#pragma endregion

struct PlayerInputs
{
    Vector2f direction;
    bool jump = false;
    bool interact = false;

    uint32_t inputIndex = 0;
};

struct PlayerData
{
    ENetPeer* peer = nullptr;
    idSize_t id;
    std::string name;
    
    PLAYER_STATE state = PLAYER_STATE::connecting;
    Vector3f position;
    Vector3f velocity;
    PlayerInputs inputs;
    PlayerInputs lastInput;

    PlayerData(idSize_t ID) : id(ID) {}

    bool IsWorm() { return state == PLAYER_STATE::worm; }
};

inline bool IsOnGround(PlayerData a_playerData)
{
    return a_playerData.position.y <= GetGroundLevel(a_playerData.state == PLAYER_STATE::worm) + GroundingTolerance;
}

inline void UpdatePhysics(PlayerData& a_playerData, float a_deltaTime)
{
    if (a_playerData.inputs.jump && IsOnGround(a_playerData))
    {
        if (!a_playerData.IsWorm())
        {
            a_playerData.velocity.y = HJumpPower;
        }
    }
    else
    {
        a_playerData.velocity.y -= HGravity * a_deltaTime;

        if (a_playerData.position.y + a_playerData.velocity.y <= HGroundLevel + GroundingTolerance)
        {
            a_playerData.position.y = HGroundLevel;
            a_playerData.velocity.y = 0.0f;
        }
    }

    if (IsOnGround(a_playerData))
    {
        a_playerData.velocity.x += a_playerData.inputs.direction.x * GetAcceleration(a_playerData.IsWorm()) * a_deltaTime;
        a_playerData.velocity.z += a_playerData.inputs.direction.y * GetAcceleration(a_playerData.IsWorm()) * a_deltaTime;
        
        Vector2f m_decelerationVector = Vector2f(a_playerData.velocity.x, a_playerData.velocity.z).normalized() * -1;
        a_playerData.velocity.x -= m_decelerationVector.x * GetDeceleration(a_playerData.IsWorm()) * a_deltaTime;
        a_playerData.velocity.z -= m_decelerationVector.y * GetDeceleration(a_playerData.IsWorm()) * a_deltaTime;
        
        if (a_playerData.velocity.x > GetVMax(a_playerData.IsWorm()))
            a_playerData.velocity.x = GetVMax(a_playerData.IsWorm());
        else if (a_playerData.velocity.x < -GetVMax(a_playerData.IsWorm()))
            a_playerData.velocity.x = -GetVMax(a_playerData.IsWorm());

        if (a_playerData.velocity.z > GetVMax(a_playerData.IsWorm()))
            a_playerData.velocity.z = GetVMax(a_playerData.IsWorm());
        else if (a_playerData.velocity.z < -GetVMax(a_playerData.IsWorm()))
            a_playerData.velocity.z = -GetVMax(a_playerData.IsWorm());
    }
    
    a_playerData.position += a_playerData.velocity * a_deltaTime;
}

#endif //_SV_PLAYERS_HPP