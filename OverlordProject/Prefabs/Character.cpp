#include "stdafx.h"
#include "Character.h"

Character::Character(const CharacterDesc& characterDesc) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
{
}

void Character::Initialize(const SceneContext& /*sceneContext*/)
{
	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * 0.5f, -5.f);
}

void Character::Update(const SceneContext& sceneContext)
{
	if (m_pCameraComponent->IsActive())
	{
		//constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero

		//***************
		//HANDLE INPUT
		auto input = sceneContext.pInput;

		//## Input Gathering (move)
		//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
		//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
		XMFLOAT3 move{ 0, 0, 0 };
		move.z = InputManager::IsKeyboardKey(InputState::down, 'W') ? 1.0f : 0.0f;
		if (move.z == 0) move.z = -(InputManager::IsKeyboardKey(InputState::down, 'S') ? 1.0f : 0.0f);
		//if (move.y == 0) move.y = InputManager::GetThumbstickPosition().y;

		//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
		//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement
		move.x = InputManager::IsKeyboardKey(InputState::down, 'D') ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(InputManager::IsKeyboardKey(InputState::down, 'A') ? 1.0f : 0.0f);
		//if (move.x == 0) move.x = InputManager::GetThumbstickPosition().x;

		move.y = InputManager::IsKeyboardKey(InputState::down, VK_SPACE) ? 1.0f : 0.0f;


		


		//std::cout << move.y << move.x << '\n';

		if (move.x != 0 || move.y != 0)
		{

		}

		//## Input Gathering (look)
		XMFLOAT2 look{ 0, 0 };
		bool mouseMoved{ false };
		if (InputManager::IsMouseButton(InputState::down, VK_LBUTTON))
		{
			// Store the MouseMovement in the local 'look' variable (cast is required)
			const POINT& mouseMove = InputManager::GetMouseMovement();
			look.x = static_cast<float>(mouseMove.x);
			look.y = static_cast<float>(mouseMove.y);

			mouseMoved = mouseMove.x != 0 || mouseMove.y != 0;

			//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
			//...
		}
		


//		//************************
////GATHERING TRANSFORM INFO
//
////Retrieve the TransformComponent

		//CALCULATE TRANSFORMS
		const auto forward = XMLoadFloat3(&GetTransform()->GetForward());
		const auto right = XMLoadFloat3(&GetTransform()->GetRight());

		//std::cout << GetTransform()->GetForward().x << ' ' << GetTransform()->GetForward().y << ' ' << GetTransform()->GetForward().z << '\n';

		//***************
		//CAMERA ROTATION

		//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
		
		//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
		m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();

		//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)
 		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

//		//********
//		//MOVEMENT
//
//		//## Horizontal Velocity (Forward/Backward/Right/Left)
//		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
//		
		float currentAcceleration = m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();
//
//
//
		bool W = input->IsKeyboardKey(InputState::down, 'W');
		bool A = input->IsKeyboardKey(InputState::down, 'A');
		bool S = input->IsKeyboardKey(InputState::down, 'S');
		bool D = input->IsKeyboardKey(InputState::down, 'D');
//
// 
// 
// 




//		//If the character is moving (= input is pressed)
		if (W || A || S || D)
		{
			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input

			XMVECTOR currentDirFor;
			currentDirFor = forward * move.z;
			
			XMVECTOR currentDirRig;
			currentDirRig = right * move.x;

			XMVECTOR sum = XMVectorAdd(currentDirFor, currentDirRig);

			XMStoreFloat3(&m_CurrentDirection, sum);

			
			//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
			m_MoveSpeed += currentAcceleration;
			if (m_MoveSpeed > m_CharacterDesc.maxMoveSpeed)
			{
				m_MoveSpeed = m_CharacterDesc.maxMoveSpeed;
			}
		}
		else
		{
			//Else (character is not moving, or stopped moving)
			//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
			m_MoveSpeed -= currentAcceleration;
			if (m_MoveSpeed < 0)
			{
				//Make sure the current MoveSpeed doesn't get smaller than zero
				m_MoveSpeed = 0;
			}
		}
//
//
//
//		//Now we can calculate the Horizontal Velocity which shoulsd be stored in m_TotalVelocity.xz
//		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
//		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
//		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)
		m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
		m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;
//	
//
//
//		//## Vertical Movement (Jump/Fall)
//		//If the Controller Component is NOT grounded (= freefall)
		if (!m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN))
		{
			//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
			//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
			m_TotalVelocity.y -= m_FallAcceleration * sceneContext.pGameTime->GetElapsed();

			if (m_TotalVelocity.y  <= -m_CharacterDesc.maxFallSpeed)
			{
				m_TotalVelocity.y = -m_CharacterDesc.maxFallSpeed;
			}
		}
		//Else If the jump action is triggered
		else if (move.y != 0)
		{
			//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		}
		//Else (=Character is grounded, no input pressed)
		else
		{
			m_TotalVelocity.y = 0;
		}
// 




//
//		//************
//		//DISPLACEMENT
//
//		//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
//		//Calculate the displacement (m) for the current frame and move the ControllerComponent

		XMFLOAT3 displacement;
		XMStoreFloat3(&displacement, XMVectorScale(XMLoadFloat3(&m_TotalVelocity), sceneContext.pGameTime->GetElapsed()));
		//std::cout << m_TotalVelocity.x << ' ' << m_TotalVelocity.y << ' ' << m_TotalVelocity.z << '\n';

		m_pControllerComponent->Move(displacement);
//
//		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
//		//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
	}
}

void Character::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if(ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}

//
//#include "stdafx.h"
//#include "Character.h"
//
//Character::Character(const CharacterDesc& characterDesc) :
//	m_CharacterDesc{ characterDesc },
//	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
//	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime)
//{}
//
//void Character::Initialize(const SceneContext& /*sceneContext*/)
//{
//	//Controller
//	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));
//
//	//Camera
//	const auto pCamera = AddChild(new FixedCamera());
//	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
//	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera
//
//	pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * .5f, 0.f);
//}
//
//void Character::Update(const SceneContext& sceneContext)
//{
//	if (m_pCameraComponent->IsActive())
//	{
//		const float elapsedSec{ sceneContext.pGameTime->GetElapsed() };
//
//		//***************
//		//HANDLE INPUT
//
//		//## Input Gathering (move)
//		XMFLOAT2 move{};
//		move.y = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward) ? 1.0f :
//			(sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward) ? -1.0f : 0.0f);
//
//		move.x = sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight) ? 1.0f :
//			(sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft) ? -1.0f : 0.0f);
//
//		//## Input Gathering (look)
//		XMFLOAT2 look{ 0.f, 0.f };
//		constexpr int mouseButton{ 1 };
//		if (InputManager::IsMouseButton(InputState::down, mouseButton))
//		{
//			const auto& mouseMovement{ InputManager::GetMouseMovement() };
//			look.x = static_cast<float>(mouseMovement.x);
//			look.y = static_cast<float>(mouseMovement.y);
//		}
//
//		//************************
//		//GATHERING TRANSFORM INFO
//
//		const TransformComponent* pTransform{ GetTransform() };
//		const XMVECTOR forward{ XMLoadFloat3(&pTransform->GetForward()) };
//		const XMVECTOR right{ XMLoadFloat3(&pTransform->GetRight()) };
//
//		//***************
//		//CAMERA ROTATION
//
//		const float rotationAmount{ elapsedSec * m_CharacterDesc.rotationSpeed };
//		m_TotalYaw += look.x * rotationAmount;
//		m_TotalPitch += look.y * rotationAmount;
//
//		GetTransform()->Rotate(0.0f, m_TotalYaw, 0.0f);
//		sceneContext.pCamera->GetTransform()->Rotate(m_TotalPitch, 0.0f, 0.0f);
//
//		//********
//		//MOVEMENT
//
//		//## Horizontal Velocity (Forward/Backward/Right/Left)
//		const float currentMoveAcceleration{ m_MoveAcceleration * elapsedSec };
//		//If the character is moving (= input is pressed)
//		if (abs(move.x) > 0.0f || abs(move.y) > 0.0f)
//		{
//			//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
//			const XMVECTOR moveDirection{ forward * move.y + right * move.x };
//			XMStoreFloat3(&m_CurrentDirection, moveDirection);
//
//			m_MoveSpeed += currentMoveAcceleration;
//			m_MoveSpeed = std::min(m_MoveSpeed, m_CharacterDesc.maxMoveSpeed);
//		}
//		else
//		{
//			m_MoveSpeed -= currentMoveAcceleration;
//			m_MoveSpeed = std::max(m_MoveSpeed, 0.0f);
//		}
//
//		m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
//		m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;
//
//		if (!(m_pControllerComponent->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN))
//		{
//			m_TotalVelocity.y -= m_FallAcceleration * elapsedSec;
//			m_TotalVelocity.y = std::max(m_TotalVelocity.y, -m_CharacterDesc.maxFallSpeed);
//		}
//		else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump))
//			m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
//		else
//			m_TotalVelocity.y = -0.1f;
//
//		//************
//		//DISPLACEMENT
//
//		const XMFLOAT3 displacement
//		{
//			m_TotalVelocity.x * elapsedSec,
//			m_TotalVelocity.y * elapsedSec,
//			m_TotalVelocity.z * elapsedSec
//		};
//		m_pControllerComponent->Move(displacement);
//	}
//}
//
//void Character::DrawImGui()
//{
//	if (ImGui::CollapsingHeader("Character"))
//	{
//		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
//		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());
//
//		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
//		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());
//
//		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
//		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
//		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());
//
//		ImGui::Dummy({ 0.f,5.f });
//		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
//			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
//		{
//			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
//		}
//
//		ImGui::Dummy({ 0.f,5.f });
//		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
//			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
//		{
//			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
//		}
//
//		ImGui::Dummy({ 0.f,5.f });
//		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
//		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");
//
//		bool isActive = m_pCameraComponent->IsActive();
//		if (ImGui::Checkbox("Character Camera", &isActive))
//		{
//			m_pCameraComponent->SetActive(isActive);
//		}
//	}
//}