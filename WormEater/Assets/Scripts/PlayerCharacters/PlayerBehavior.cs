using System;
using Constant;
using UnityEngine;
using Players;

public class PlayerBehavior : MonoBehaviour
{
    public bool _isLocalPlayer;
    public bool _isWorm { get => m_playerData.state == PLAYER_STATE.worm; }
    
    private PlayerData m_playerData;
    
    private void FixedUpdate()
    {
        if (m_playerData.state == PLAYER_STATE.unexpected ||
            m_playerData.state == PLAYER_STATE.connecting ||
            m_playerData.state == PLAYER_STATE.dead)
            return;
        
        if (_isLocalPlayer)
        {
            PlayerInputsManager.Instance.UpdateInputs(ref m_playerData.inputs);
            UpdatePhysics(m_playerData.inputs, Time.fixedDeltaTime);
        }
        
        transform.position = m_playerData.position;
        
        Vector2 orientationVector = _isLocalPlayer ? m_playerData.inputs.direction : new Vector2(m_playerData.velocity.normalized.x, m_playerData.velocity.normalized.z);
        transform.rotation = Quaternion.LookRotation(orientationVector , Vector3.up);
    }

    private bool IsOnGround()
    {
        return m_playerData.position.y <= PlayerConst.GetGroundLevel(_isWorm) + PlayerConst.GroundingTolerance;
    }

    public void UpdatePlayer(Vector3 a_position, Vector3 a_velocity, Vector2 a_inputs)
    {
        m_playerData.position = a_position;
        m_playerData.velocity = a_velocity;
        if (!_isLocalPlayer)
        {
            m_playerData.inputs.direction = a_inputs.normalized;
        }
    }
    
    public void UpdatePhysics(PlayerInputs a_playerInputs, float a_deltaTime)
    {
        if (a_playerInputs.jump && IsOnGround())
        {
            if (!_isWorm)
            {
                m_playerData.velocity.y = PlayerConst.HJumpPower;
            }
        }
        else
        {
            m_playerData.velocity.y -= PlayerConst.HGravity * a_deltaTime;

            if (m_playerData.position.y + m_playerData.velocity.y <=
                PlayerConst.HGroundLevel + PlayerConst.GroundingTolerance)
            {
                m_playerData.position.y = PlayerConst.HGroundLevel;
                m_playerData.velocity.y = 0f;
            }
        }
        
        if (IsOnGround())
        {
            m_playerData.velocity.x += a_playerInputs.direction.x * PlayerConst.GetAcceleration(_isWorm) * a_deltaTime;
            m_playerData.velocity.z += a_playerInputs.direction.y * PlayerConst.GetAcceleration(_isWorm) * a_deltaTime;
            
            Vector2 m_decelerationVector = new Vector2(m_playerData.velocity.x, m_playerData.velocity.z).normalized * -1;
            m_playerData.velocity.x -= m_decelerationVector.x * PlayerConst.GetDeceleration(_isWorm) * a_deltaTime;
            m_playerData.velocity.z -= m_decelerationVector.y * PlayerConst.GetDeceleration(_isWorm) * a_deltaTime;
            
            m_playerData.velocity.x = Mathf.Clamp(m_playerData.velocity.x, -PlayerConst.GetVMax(_isWorm), PlayerConst.GetVMax(_isWorm));
            m_playerData.velocity.z = Mathf.Clamp(m_playerData.velocity.z, -PlayerConst.GetVMax(_isWorm), PlayerConst.GetVMax(_isWorm));
        }
        
        m_playerData.position += m_playerData.velocity * a_deltaTime;
    }
}
