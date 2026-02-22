using System;
using UnityEngine;
using ENet6;
using Constant;

namespace Players
{
    using Int8 = SByte;
    using UInt8 = Byte;
    
    public struct PlayerInputs
    {
        public Vector2 direction;
        public bool jump;
        public bool interact;

        public UInt32 inputIndex;
    }

    public struct PlayerData
    {
        public UInt8 id;
        public string name;

        public PLAYER_STATE state;
        public Vector3 position;
        public Vector3 velocity;
        public PlayerInputs inputs;

        PlayerData(string a_name)
        {
            id = 0;
            name = a_name;

            state = PLAYER_STATE.connecting;
            position = Vector3.zero;
            velocity = Vector3.zero;
            inputs = new PlayerInputs();
        }
    }
    
    public static class PlayerConst
    {
        public static float GetVMax(bool IsWorm)
        {
            return IsWorm ? WVMax : HVMax;
        }
        public static float GetAcceleration(bool IsWorm)
        {
            return IsWorm ? WAcceleration : HAcceleration;
        }
        public static float GetDeceleration(bool IsWorm)
        {
            return IsWorm ? WDeceleration : HDeceleration;
        }
        public static float GetGroundLevel(bool IsWorm)
        {
            return IsWorm ? WGroundLevel : HGroundLevel;
        }
        
        
        public const float GroundingTolerance = 0.2f;
        
        // Worm
        public const float WVMax = 8.0f;
        public const float WAcceleration = 1.5f;
        public const float WDeceleration = 0.5f;
        
        public const float WGroundLevel = -5.0f;
        
        // Human
        public const float HVMax = 5.0f;
        public const float HAcceleration = 2.0f;
        public const float HDeceleration = 1.0f;
        
        public const float HGroundLevel = 1.0f;
        
        public const float HJumpPower = 6.0f;
        public const float HGravity = 9.81f;
    }
}
