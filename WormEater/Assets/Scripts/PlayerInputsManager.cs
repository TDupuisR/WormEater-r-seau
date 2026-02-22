using UnityEngine;
using NaughtyAttributes;
using UnityEngine.InputSystem;
using Players;

public class PlayerInputsManager : MonoBehaviour
{
    public static PlayerInputsManager Instance;
    
    [Header("Movement Input")]
    [SerializeField, ReadOnly] private Vector2 _moveInput;
    public Vector2 MoveInput => _moveInput;
    
    [Header("Jump Input")]
    [SerializeField, ReadOnly] private bool _jumpInput;
    public bool JumpInput => _jumpInput;
    [Header("Interact Input")]
    [SerializeField, ReadOnly] private bool _interactInput;
    public bool InteractInput => _interactInput;
    
    public void UpdateInputs(ref PlayerInputs playerInputs)
    {
        playerInputs.direction = _moveInput;
        playerInputs.jump = _jumpInput;
        playerInputs.interact = _interactInput;
    }


    //Binding des inputs
    public void OnMoveInput(InputAction.CallbackContext context)
    {
        _moveInput = context.ReadValue<Vector2>().normalized;
    }

    public void OnJumpInput(InputAction.CallbackContext context)
    {
        _jumpInput = context.ReadValueAsButton();
    }
    
    public void OnInteractInput(InputAction.CallbackContext context)
    {
        _interactInput = context.ReadValueAsButton();
    }
}
