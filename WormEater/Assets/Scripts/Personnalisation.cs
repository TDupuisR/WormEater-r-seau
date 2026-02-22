using NaughtyAttributes;
using TMPro;
using UnityEngine;
using Network;

public class Personnalisation : MonoBehaviour
{
    private Canvas m_persoCanvas;
    
    [SerializeField] private TMP_InputField m_name;
    private string savedName;
    
    void Start()
    {
        m_persoCanvas = GetComponent<Canvas>();
        
        //m_name.text = "Lux";
    }
    
    public void ApplyAndSendInfo()
    {
        ApplyInfo();
        SendInfo();
    }

    [Button]
    public void ApplyInfo()
    {
        savedName = m_name.text;
    }
    [Button]
    public void SendInfo()
    {
        NetworkCore.Instance.SendPlayerInfo(savedName);
    }
    
    [Button]
    public void SwitchOpenMenu()
    {
        OpenMenu(!m_persoCanvas.enabled);
    }
    public void OpenMenu(bool open)
    {
        if (open)
        {
            m_name.text = savedName;
        }
        
        m_persoCanvas.enabled = open;
    }
}
