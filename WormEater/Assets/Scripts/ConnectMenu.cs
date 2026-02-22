using UnityEngine;
using UnityEngine.UI;
using NaughtyAttributes;
using TMPro;
using Network;

public class ConnectMenu : MonoBehaviour
{
    [SerializeField] private Personnalisation m_personnalisation;
    
    [SerializeField] private TextMeshProUGUI m_feedback;
    [SerializeField] private TextMeshProUGUI m_connectionStatus;
    
    [SerializeField] private TMP_InputField m_IPInputField;
    [SerializeField] private TMP_InputField m_portInputField;
    
    [SerializeField] private Button m_connectButton;
    [SerializeField] private Button m_disconnectButton;
    
    private NetworkCore m_networkScript;
    private string inputIP;
    private ushort inputPort;
    private NETWORK_STATE m_lastState = NETWORK_STATE.Disconnected;
    private Canvas m_menuCanvas;

    private void OnEnable()
    {
        NetworkCore.OnConnectionStateChange += Feedback;
    }

    private void OnDisable()
    {
        NetworkCore.OnConnectionStateChange -= Feedback;
    }

    private void Start()
    {
        m_menuCanvas = GetComponent<Canvas>();
        
        m_networkScript = NetworkCore.Instance;
        
        m_feedback.text = "";
        m_IPInputField.text = "localhost";
        m_portInputField.text = "14769";
        
        Feedback(NETWORK_STATE.Disconnected);
    }
    
    [Button]
    public void TryConnect()
    {
        inputIP = m_IPInputField.text.Trim();
        if (string.IsNullOrWhiteSpace(inputIP))
            inputIP = "localhost";
        
        if (!ushort.TryParse(m_portInputField.text.Trim(), out inputPort))
        {
            Debug.LogWarning($"{m_portInputField.text} : Invalid port");
            inputPort = 14769;
        }
        
        m_networkScript.Connect(inputIP, inputPort);
        if (m_networkScript.MConnectionState == NETWORK_STATE.Connected)
        {
            m_personnalisation.ApplyAndSendInfo();
        }
    }

    [Button]
    public void TryDisconnect()
    {
        m_networkScript.Disconnect();
    }
    
    public void Feedback(NETWORK_STATE state)
    {
        switch (state)
        {
            case NETWORK_STATE.Disconnected:
            {
                if (m_lastState == NETWORK_STATE.Connecting)
                {
                    m_feedback.color = Color.red;
                    m_feedback.text = $"{inputIP} : Connection failed!";
                }
                else
                {
                    m_feedback.color = Color.red;
                    m_feedback.text = $"Disconnected!";
                }
                
                m_connectionStatus.text = $"<color=\"white\">{inputIP}:{inputPort} - <color=\"red\">disconected";
                
                m_connectButton.interactable = true;
                m_disconnectButton.interactable = false;
                
                m_menuCanvas.enabled = true;
                m_personnalisation.OpenMenu(true);
                break;
            }
            case NETWORK_STATE.Connected:
            {
                m_feedback.color = Color.green;
                m_feedback.text = $"{inputIP} : Connected!";
                
                m_connectionStatus.text = $"<color=\"white\">{inputIP}:{inputPort} - <color=\"green\">connected";
                
                m_connectButton.interactable = true;
                m_disconnectButton.interactable = true;
                
                m_menuCanvas.enabled = false;
                m_personnalisation.OpenMenu(false);
                break;
            }
            case NETWORK_STATE.Connecting:
            {
                m_feedback.color = Color.yellow;
                m_feedback.text = $"{inputIP} : Try connecting...";
                
                m_connectionStatus.text = $"<color=\"white\">{inputIP}:{inputPort} - <color=\"yellow\">connecting...";
                
                m_connectButton.interactable = false;
                m_disconnectButton.interactable = false;
                break;
            }
        }

        m_lastState = state;
    }

    [Button]
    public void DebugSendReady()
    {
        Debug.Log($"Debug Send Ready : {m_networkScript.SendPlayerReady(true)}");
    }
}
