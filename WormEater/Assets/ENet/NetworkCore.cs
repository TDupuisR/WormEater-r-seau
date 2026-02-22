using ENet6;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Constant;
using Players;
using Serialization;

public enum NETWORK_STATE
{
    Disconnected,
    Connecting,
    Connected
}

namespace Network
{
    public class NetworkCore : MonoBehaviour
    {
        public static NetworkCore Instance;

        private ENet6.Host m_enetHost = null;
        private ENet6.Peer? m_serverPeer = null;
        private GameData m_gameData;

        private Coroutine m_connectCoroutine = null;

        private NETWORK_STATE m_connectionState = NETWORK_STATE.Disconnected;

        public NETWORK_STATE MConnectionState
        {
            get { return m_connectionState; }
            private set
            {
                m_connectionState = value;
                OnConnectionStateChange?.Invoke(m_connectionState);
            }
        }

        public static event Action<NETWORK_STATE> OnConnectionStateChange;

        public struct GameData
        {
            public GAME_STATE state;
            public List<PlayerData> players;
            public Byte ownPlayerId;
            public PlayerInputs inputs;

            public float interpolationFactor;
            public UInt32 nextInputIndex;
            public List<PlayerInputs> predictedInputs;
        }

        public void Awake()
        {
            if (Instance == null)
            {
                Instance = this;
                DontDestroyOnLoad(gameObject);
            }
            else
            {
                Destroy(gameObject);
            }
        }

        #region ConnectionHandle

        public void Connect(string addressString, ushort port = 14769)
        {
            if (m_connectCoroutine == null)
                StartCoroutine(ConnectRoutine(addressString, port));
        }

        private IEnumerator ConnectRoutine(string addressString, ushort port)
        {
            MConnectionState = NETWORK_STATE.Connecting;

            ENet6.Address address = new ENet6.Address();
            if (!address.SetHost(ENet6.AddressType.Any, addressString))
            {
                Debug.LogError("failed to resolve \"" + addressString + "\"");

                Disconnect();
                yield break;
            }

            Debug.Log("connecting to " + address.GetIP());

            address.Port = port;

            // On recréé l'host à la connexion pour l'avoir en IPv4 / IPv6 selon l'adresse
            if (m_enetHost != null)
                m_enetHost.Dispose();

            m_enetHost = new ENet6.Host();
            m_enetHost.Create(address.Type, 1, 0);
            m_serverPeer = m_enetHost.Connect(address, 0);

            // On laisse la connexion se faire pendant un maximum de 50 * 100ms = 5s
            for (uint i = 0; i < 50; ++i)
            {
                ENet6.Event evt = new ENet6.Event();
                if (m_enetHost.Service(100, out evt) > 0)
                {
                    // Nous avons un événement, la connexion a soit pu s'effectuer (ENET_EVENT_TYPE_CONNECT) soit échoué (ENET_EVENT_TYPE_DISCONNECT)
                    break; //< On sort de la boucle
                }

                yield return new WaitForFixedUpdate();
            }

            if (m_serverPeer.Value.State != PeerState.Connected)
            {
                Debug.LogError("connection to \"" + addressString + "\" failed");

                Disconnect();
                yield break;
            }

            Debug.Log($"connected to {addressString}!");

            m_gameData = new GameData();

            MConnectionState = NETWORK_STATE.Connected;
            yield break;
        }

        public void Disconnect()
        {
            Debug.Log("disconnecting...");

            if (m_enetHost == null || m_serverPeer == null)
                return;

            m_serverPeer.Value.DisconnectNow(0);
            MConnectionState = NETWORK_STATE.Disconnected;
        }

        // Start is called before the first frame update
        void Start()
        {
            if (!ENet6.Library.Initialize())
                throw new Exception("Failed to initialize ENet");
        }

        private void OnApplicationQuit()
        {
            Disconnect();
            ENet6.Library.Deinitialize();
        }

        #endregion

        // FixedUpdate est appelé à chaque Tick (réglé dans le projet)
        void FixedUpdate()
        {
            if (m_enetHost == null || m_serverPeer == null)
                return;

            // Debug.Log("FixedUpdate");

            ENet6.Event evt = new ENet6.Event();
            if (m_enetHost.Service(0, out evt) > 0)
            {
                do
                {
                    switch (evt.Type)
                    {
                        case ENet6.EventType.None:
                            Debug.Log("?");
                            break;

                        case ENet6.EventType.Connect:
                            Debug.Log("Connect");
                            break;

                        case ENet6.EventType.Disconnect:
                            Debug.Log("Disconnect");

                            Disconnect();
                            break;

                        case ENet6.EventType.Receive:
                            Debug.Log("Receive");

                            HandleMessage(evt.Packet, m_gameData);

                            break;

                        case ENet6.EventType.Timeout:
                            Debug.Log("Timeout");

                            Disconnect();
                            break;
                    }
                } while (m_enetHost.CheckEvents(out evt) > 0);
            }
        }

        void HandleMessage(Packet receivedPacket, GameData gameData)
        {
            byte[] data = new byte[receivedPacket.Length];
            receivedPacket.CopyTo(data);

            int offset = 0;

            OP_CODE opcode = (OP_CODE)ByteBuffer.Deserialize_u8(ref data, ref offset);
            switch (opcode)
            {
                case OP_CODE.S_GameData:
                {
                    GameDataPacket packet = GameDataPacket.Deserialize(ref data, ref offset);
                    gameData.ownPlayerId = packet.id;
                    Debug.Log($"Own player id : {(int)gameData.ownPlayerId}");
                    break;
                }
                case OP_CODE.S_PlayerList:
                {
                    break;
                }
                case OP_CODE.S_PlayerPosition:
                {
                    break;
                }

                case OP_CODE.Unexpected:
                default:
                    Debug.LogWarning($"Handle Message : Unexpected opcode ({(int)opcode})");
                    break;
            }

            receivedPacket.Dispose();
        }

        public bool SendPlayerInfo(string name)
        {
            if (m_serverPeer == null)
            {
                Debug.LogError($"SendPlayerInfo : Cannot send packet, server peer is null");
                return false;
            }

            PlayerInfoPacket infoPacket = new PlayerInfoPacket();
            infoPacket.name = name;

            Packet packet = ByteBuffer.build_packet(infoPacket, PacketFlags.Reliable);
            return m_serverPeer.Value.Send(0, ref packet);
        }

        public bool SendPlayerReady(bool ready)
        {
            if (m_serverPeer == null)
            {
                Debug.LogError($"SendPlayerInfo : Cannot send packet, server peer is null");
                return false;
            }

            PlayerReady readyPacket = new PlayerReady();
            readyPacket.isReady = ready;

            Packet packet = ByteBuffer.build_packet(readyPacket, PacketFlags.Reliable);
            return m_serverPeer.Value.Send(0, ref packet);
        }
    }
}
