using UnityEngine;
using System;
using System.Text;
using System.Collections.Generic;
using Constant;
using ENet6;

namespace Serialization
{
    using Int8 = SByte;
    using UInt8 = Byte;
    
    using idSize_t = Byte;
    
    public static class ByteBuffer
    {
        // NOTES
        // La construction/déstruction de tableau à la sérialization n'est pas optimal en cas de gros packets (ici peu problématique)
        // Envisager d'utiliser MemoryStream + BinaryWriter à l'avenir pour s'occuper de ça.
        
        public static Encoding utf8 = Encoding.UTF8;
        
        public const int SIZE_8 = 1;
        public const int SIZE_16 = 2;
        public const int SIZE_32 = 4;
        public const int SIZE_64 = 8;
        public const int SIZE_128 = 16;
        
        #region Serialization
        
        public static void Serialize_f32(ref byte[] byteArray, float value)
        {
            byte[] valueBytes = BitConverter.GetBytes(value);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);

            if (valueBytes.Length != SIZE_32)
            {
                Debug.LogError($"Serialize_f32 : Cannot continue serialization, valueBytes.length isn't at 32 bit size -> {valueBytes.Length} should be {SIZE_32}");
                return;
            }
            
            Array.Resize(ref byteArray, byteArray.Length + SIZE_32);
            System.Buffer.BlockCopy(valueBytes, 0, byteArray, byteArray.Length - SIZE_32, SIZE_32);
        }
        
        public static void Serialize_i8(ref byte[] byteArray, Int8 value)
        {
            Serialize_u8(ref byteArray, (UInt8)value);
        }
        public static void Serialize_u8(ref byte[] byteArray, UInt8 value)
        {
            byte[] valueBytes = new byte[1] { value };

            if (valueBytes.Length != SIZE_8)
            {
                Debug.LogError($"Serialize_u8 : Cannot continue serialization, valueBytes.length isn't at 8 bit size -> {valueBytes.Length} should be {SIZE_8}");
                return;
            }
            
            Array.Resize(ref byteArray, byteArray.Length + SIZE_8);
            System.Buffer.BlockCopy(valueBytes, 0, byteArray, byteArray.Length - SIZE_8, SIZE_8);
        }
        
        public static void Serialize_i16(ref byte[] byteArray, Int16 value)
        {
            Serialize_u16(ref byteArray, (UInt16)value);
        }
        public static void Serialize_u16(ref byte[] byteArray, UInt16 value)
        {
            byte[] valueBytes = BitConverter.GetBytes(value);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);

            if (valueBytes.Length != SIZE_16)
            {
                Debug.LogError($"Serialize_u16 : Cannot continue serialization, valueBytes.length isn't at 16 bit size -> {valueBytes.Length} should be {SIZE_16}");
                return;
            }
            
            Array.Resize(ref byteArray, byteArray.Length + SIZE_16);
            System.Buffer.BlockCopy(valueBytes, 0, byteArray, byteArray.Length - SIZE_16, SIZE_16);
        }
        
        public static void Serialize_i32(ref byte[] byteArray, Int32 value)
        {
            Serialize_u32(ref byteArray, (UInt32)value);
        }
        public static void Serialize_u32(ref byte[] byteArray, UInt32 value)
        {
            byte[] valueBytes = BitConverter.GetBytes(value);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);

            if (valueBytes.Length != SIZE_32)
            {
                Debug.LogError($"Serialize_u32 : Cannot continue serialization, valueBytes.length isn't at 32 bit size -> {valueBytes.Length} should be {SIZE_32}");
                return;
            }
            
            Array.Resize(ref byteArray, byteArray.Length + SIZE_32);
            System.Buffer.BlockCopy(valueBytes, 0, byteArray, byteArray.Length - SIZE_32, SIZE_32);
            // equivalent à
            // -> byteArray.CopyTo(resByteArray, 0);
            // -> valueBytes.CopyTo(resByteArray, byteArray.Length);
            // Apparement plus rapide dans certains cas + standard bas niveau
        }
        
        public static void Serialize_str(ref byte[] byteArray, string value )
        {
            byte[] valueBytes = utf8.GetBytes(value);
            Serialize_u32(ref byteArray, (UInt32)valueBytes.Length);

            if (!string.IsNullOrEmpty(value))
            {
                Array.Resize(ref byteArray, byteArray.Length + valueBytes.Length);
                System.Buffer.BlockCopy(valueBytes, 0, byteArray, byteArray.Length - valueBytes.Length, valueBytes.Length);
            }
        }
        
        #endregion
        
        #region Deserialization
    
        public static float Deserialize_f32(ref byte[] byteArray, ref int offset)
        {
            byte[] valueBytes = new byte[SIZE_32];
            System.Buffer.BlockCopy(byteArray, offset, valueBytes, 0, SIZE_32);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);
            
            offset += SIZE_32;
            return BitConverter.ToSingle(valueBytes, 0);
        }
        
        public static Int8 Deserialize_i8(ref byte[] byteArray, ref int offset)
        {
            return (Int8)Deserialize_u8(ref byteArray, ref offset);
        }
        public static UInt8 Deserialize_u8(ref byte[] byteArray, ref int offset)
        {
            byte[] valueBytes = new byte[SIZE_8];
            System.Buffer.BlockCopy(byteArray, offset, valueBytes, 0, SIZE_8);
            
            offset += SIZE_8;
            return (UInt8)valueBytes[0];
        }
        
        public static Int16 Deserialize_i16(ref byte[] byteArray, ref int offset)
        {
            return (Int16)Deserialize_u16(ref byteArray, ref offset);
        }
        public static UInt16 Deserialize_u16(ref byte[] byteArray, ref int offset)
        {
            byte[] valueBytes = new byte[SIZE_16];
            System.Buffer.BlockCopy(byteArray, offset, valueBytes, 0, SIZE_16);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);
            
            offset += SIZE_16;
            return BitConverter.ToUInt16(valueBytes, 0);
        }
        
        public static Int32 Deserialize_i32(ref byte[] byteArray, ref int offset)
        {
            return (Int32)Deserialize_u32(ref byteArray, ref offset);
        }
        public static UInt32 Deserialize_u32(ref byte[] byteArray, ref int offset)
        {
            byte[] valueBytes = new byte[SIZE_32];
            System.Buffer.BlockCopy(byteArray, offset, valueBytes, 0, SIZE_32);
            
            if (BitConverter.IsLittleEndian)
                Array.Reverse(valueBytes);
            
            offset += SIZE_32;
            return BitConverter.ToUInt32(valueBytes, 0);
        }
        
        public static string Deserialize_str(ref byte[] byteArray, ref int offset)
        {
            UInt32 length = Deserialize_u32(ref byteArray, ref offset);
            if (offset + (int)length > byteArray.Length)
            {
                Debug.LogError($"Deserialize_str : Cannot continue deserialization, offset buffer overflow -> {offset + (int)length} should be <= {byteArray.Length}");
                return "ERROR DESERIALIZATION";
            }
            string res = utf8.GetString(byteArray, offset, (int)length);
            
            offset += (int)length;
            return res;
        }
        
        #endregion

        public static Packet build_packet(ModelPacket MPacket, PacketFlags flag)
        {
            Packet packet = new Packet();
            byte[] byteArray = Array.Empty<byte>();
            
            ByteBuffer.Serialize_u8(ref byteArray, (UInt8)MPacket.opcode);
            MPacket.Serialize(ref byteArray);
            
            // Debug.Log($"Build Packet : opcode -> {(OP_CODE)byteArray[0]} | size -> {byteArray.Length}");

            packet.Create(byteArray, flag);
            return packet;
        }
    }
    
    #region OP_CODE messages
    
    public enum OP_CODE : UInt8
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
    }
    
    public abstract class ModelPacket
    {
        public abstract OP_CODE opcode { get; }
        
        public abstract void Serialize(ref byte[] byteArray);
    }
    
    public class PlayerInfoPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.C_PlayerInfo;

        public string name;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_str(ref byteArray, name);
        }
        public static PlayerInfoPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerInfoPacket packet = new PlayerInfoPacket();
            
            packet.name = ByteBuffer.Deserialize_str(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class PlayerInputPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.C_PlayerInput;

        public Players.PlayerInputs inputs;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_f32(ref byteArray, inputs.direction.x);
            ByteBuffer.Serialize_f32(ref byteArray, inputs.direction.y);

            UInt8 actionByte = 0;
            actionByte |= (byte)((inputs.jump ? 1 : 0) << 0);
            actionByte |= (byte)((inputs.interact ? 1 : 0) << 1);
            ByteBuffer.Serialize_u8(ref byteArray, actionByte);
            
            ByteBuffer.Serialize_u32(ref byteArray, inputs.inputIndex);
            
        }
        public static PlayerInputPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerInputPacket packet = new PlayerInputPacket();
            
            packet.inputs.direction.x = ByteBuffer.Deserialize_u32(ref byteArray, ref offset);
            packet.inputs.direction.y = ByteBuffer.Deserialize_u32(ref byteArray, ref offset);
            
            UInt8 actionByte = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
            packet.inputs.jump = (actionByte & (1 << 0)) != 0;
            packet.inputs.interact = (actionByte & (1 << 1)) != 0;
            
            packet.inputs.inputIndex = ByteBuffer.Deserialize_u32(ref byteArray, ref offset);

            return packet;
        }
    }

    public class PlayerReady : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.C_PlayerReady;

        public bool isReady;
        
        public override void Serialize(ref byte[] byteArray)
        {
            UInt8 actionByte = 0;
            actionByte |= (byte)((isReady ? 1 : 0) << 0);
            ByteBuffer.Serialize_u8(ref byteArray, actionByte);
        }
        public static PlayerReady Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerReady packet = new PlayerReady();
            
            UInt8 actionByte = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
            packet.isReady = (actionByte & (1 << 0)) != 0;

            return packet;
        }
    }

    public class GameDataPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_GameData;

        public idSize_t id;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u8(ref byteArray, id);
        }
        public static GameDataPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            GameDataPacket packet = new GameDataPacket();
            
            packet.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);

            return packet;
        }
    }

    public class WormAttackPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_WormAttack;

        public List<idSize_t> targetId;
        public Vector3 position;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)targetId.Count);
            foreach (idSize_t target in targetId)
            {
                ByteBuffer.Serialize_u8(ref byteArray, target);
            }
            
            ByteBuffer.Serialize_f32(ref byteArray, position.x);
            ByteBuffer.Serialize_f32(ref byteArray, position.y);
            ByteBuffer.Serialize_f32(ref byteArray, position.z);
        }
        public static WormAttackPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            WormAttackPacket packet = new WormAttackPacket();
            
            UInt16 targetCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < targetCount; i++)
            {
                packet.targetId.Add(ByteBuffer.Deserialize_u8(ref byteArray, ref offset));
            }

            packet.position.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
            packet.position.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
            packet.position.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class PlayerListPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_PlayerList;

        public struct Player
        {
            public idSize_t id;
            public string name;
            // Personnalisation
        }
        
        public List<Player> players;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)players.Count);
            foreach (Player player in players)
            {
                ByteBuffer.Serialize_u8(ref byteArray, player.id);
                ByteBuffer.Serialize_str(ref byteArray, player.name);
            }
        }
        public static PlayerListPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerListPacket packet = new PlayerListPacket();
            
            UInt16 playerCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < playerCount; i++)
            {
                Player player = new Player();
                
                player.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
                player.name = ByteBuffer.Deserialize_str(ref byteArray, ref offset);
                
                packet.players.Add(player);
            }

            return packet;
        }
    }
    
    public class PlayerPositionPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_PlayerPosition;

        public struct PlayerPos
        {
            public idSize_t id;
            public Vector3 position;
            public Vector3 velocity;
            public Vector2 input;
        }
        
        public List<PlayerPos> players;
        public UInt32 lastInputIndex;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)players.Count);
            foreach (PlayerPos player in players)
            {
                ByteBuffer.Serialize_u8(ref byteArray, player.id);
                
                ByteBuffer.Serialize_f32(ref byteArray, player.position.x);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.y);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.z);
                
                ByteBuffer.Serialize_f32(ref byteArray, player.velocity.x);
                ByteBuffer.Serialize_f32(ref byteArray, player.velocity.y);
                ByteBuffer.Serialize_f32(ref byteArray, player.velocity.z);
                
                ByteBuffer.Serialize_f32(ref byteArray, player.input.x);
                ByteBuffer.Serialize_f32(ref byteArray, player.input.y);
            }
            
            ByteBuffer.Serialize_u32(ref byteArray, lastInputIndex);
        }
        public static PlayerPositionPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerPositionPacket packet = new PlayerPositionPacket();
            
            UInt16 playerCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < playerCount; i++)
            {
                PlayerPos player = new PlayerPos();
                
                player.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
                
                player.position.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                
                player.velocity.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.velocity.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.velocity.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                
                player.input.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.input.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                
                packet.players.Add(player);
            }
            
            packet.lastInputIndex = ByteBuffer.Deserialize_u32(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class CountDownPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_Countdown;

        public UInt16 countdown;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, countdown);
        }
        public static CountDownPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            CountDownPacket packet = new CountDownPacket();
            
            packet.countdown = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class PlayerMakeSound : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_PlayerMakeSound;

        public idSize_t id;
        public Vector3 position;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u8(ref byteArray, id);
            ByteBuffer.Serialize_f32(ref byteArray, position.x);
            ByteBuffer.Serialize_f32(ref byteArray, position.y);
            ByteBuffer.Serialize_f32(ref byteArray, position.z);
        }
        public static PlayerMakeSound Deserialize(ref byte[] byteArray, ref int offset)
        {
            PlayerMakeSound packet = new PlayerMakeSound();
            
            packet.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
            packet.position.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
            packet.position.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
            packet.position.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class WormNearPacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_WormNear;

        public float nearRatio;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_f32(ref byteArray, nearRatio);
        }
        public static WormNearPacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            WormNearPacket packet = new WormNearPacket();
            
            packet.nearRatio = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class WaitingStatePacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_WaitingState;

        public struct Player
        {
            public idSize_t id;
            public Vector3 position;
        }
        
        public List<Player> players;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)players.Count);
            foreach (Player player in players)
            {
                ByteBuffer.Serialize_u8(ref byteArray, player.id);
                
                ByteBuffer.Serialize_f32(ref byteArray, player.position.x);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.y);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.z);
            }
        }
        public static WaitingStatePacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            WaitingStatePacket packet = new WaitingStatePacket();
            
            UInt16 playerCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < playerCount; i++)
            {
                Player player = new Player();
                
                player.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
                
                player.position.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                
                packet.players.Add(player);
            }

            return packet;
        }
    }
    
    public class GameStartStatePacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_GameStartState;

        public struct Player
        {
            public idSize_t id;
            public Vector3 position;
            public PLAYER_STATE state;
        }
        
        public List<Player> players;
        public Int16 countDown;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)players.Count);
            foreach (Player player in players)
            {
                ByteBuffer.Serialize_u8(ref byteArray, player.id);
                
                ByteBuffer.Serialize_f32(ref byteArray, player.position.x);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.y);
                ByteBuffer.Serialize_f32(ref byteArray, player.position.z);
                
                ByteBuffer.Serialize_u8(ref byteArray, (UInt8)player.state);
            }
            
            ByteBuffer.Serialize_i16(ref byteArray, countDown);
        }
        public static GameStartStatePacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            GameStartStatePacket packet = new GameStartStatePacket();
            
            UInt16 playerCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < playerCount; i++)
            {
                Player player = new Player();
                
                player.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
                
                player.position.x = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.y = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                player.position.z = ByteBuffer.Deserialize_f32(ref byteArray, ref offset);
                
                packet.players.Add(player);
            }
            
            packet.countDown = ByteBuffer.Deserialize_i16(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class WormArriveStatePacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_WormArriveState;

        public idSize_t wormId;
        public Int16 countdown;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u8(ref byteArray, wormId);
            ByteBuffer.Serialize_i16(ref byteArray, countdown);
        }
        public static WormArriveStatePacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            WormArriveStatePacket packet = new WormArriveStatePacket();
            
            packet.wormId = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
            packet.countdown = ByteBuffer.Deserialize_i16(ref byteArray, ref offset);

            return packet;
        }
    }
    
    public class FinishedStatePacket : ModelPacket
    {
        public override OP_CODE opcode => OP_CODE.S_FinishedState;

        public struct Player
        {
            public idSize_t id;
            public string name;
        }
        
        public List<Player> players;
        
        public override void Serialize(ref byte[] byteArray)
        {
            ByteBuffer.Serialize_u16(ref byteArray, (UInt16)players.Count);
            foreach (Player player in players)
            {
                ByteBuffer.Serialize_u8(ref byteArray, player.id);
                ByteBuffer.Serialize_str(ref byteArray, player.name);
            }
        }
        public static FinishedStatePacket Deserialize(ref byte[] byteArray, ref int offset)
        {
            FinishedStatePacket packet = new FinishedStatePacket();
            
            UInt16 playerCount = ByteBuffer.Deserialize_u16(ref byteArray, ref offset);
            for (int i = 0; i < playerCount; i++)
            {
                Player player = new Player();
                
                player.id = ByteBuffer.Deserialize_u8(ref byteArray, ref offset);
                player.name = ByteBuffer.Deserialize_str(ref byteArray, ref offset);
                
                packet.players.Add(player);
            }

            return packet;
        }
    }
    
    #endregion
}

