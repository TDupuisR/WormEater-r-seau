using System;
using UnityEngine;

namespace Constant
{
    using Int8 = SByte;
    using UInt8 = Byte;

    public enum PLAYER_STATE : UInt8
    {
        unexpected,
        connecting,
        waiting,
        ready,
        human,
        worm,
        dead
    }

    public enum GAME_STATE : UInt8
    {
        connecting,
        waiting,
        game,
        finished
    }
}
