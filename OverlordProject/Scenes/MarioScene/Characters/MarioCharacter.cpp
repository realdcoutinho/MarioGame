#include "stdafx.h"
#include "MarioCharacter.h"

//MATERIALS
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/DiffuseMaterials_FlipNormals.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_NegativeNormals.h"
#include "Materials/DiffuseMaterial_Skinned.h"


int MarioCharacter::m_NrStars{ 0 };
int MarioCharacter::m_NrLives{ 3 };


MarioCharacter::MarioCharacter(const CharacterDesc& characterDesc, XMFLOAT3 startLocation) :
	Character(characterDesc),
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime),
	m_StartLocation(startLocation)
{

}

MarioCharacter::~MarioCharacter()
{
	for (size_t i = 0; i < m_ClipCount; i++) {
		delete[] m_ClipNames[i];
	}

	// Release the memory for the array itself
	delete[] m_ClipNames;
}

void MarioCharacter::Initialize(const SceneContext&)
{
	ParticleEmitterSettings settings{};
	settings.velocity = { 0.f,2.f,0.f };
	settings.minSize = 0.0f;
	settings.maxSize = 0.0f;
	settings.minEnergy = 0.0f;
	settings.maxEnergy = 0.0f;
	settings.minScale = 0.0f;
	settings.maxScale = 0.0f;
	settings.minEmitterRadius = .0f;
	settings.maxEmitterRadius = .0f;
	settings.color = { 1.f,1.f,1.f, .6f };
	//settings.isFacingCamera = true;
	//settings.isRotateUV = true;


	SetTag(L"Player");
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	m_pCameraHolder = AddChild(new GameObject());
	m_pCamera = new FixedCamera();
	m_pCameraHolder->AddChild(m_pCamera);

	m_pCameraComponent = m_pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera
	m_pCamera->GetTransform()->Translate(0.f, m_CharacterDesc.controller.height * 4.0f, -15.f);
	m_pCamera->GetTransform()->Rotate(20.f, 0.0f, 0.f);

	//m_SceneContext.settings.enableOnGUI = true;

	const auto pSkinnedMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Skinned>();
	pSkinnedMaterial->SetDiffuseTexture(L"Textures/mario_main.png");

	m_pModelContainer = AddChild(new GameObject);
	const auto pModel = m_pModelContainer->AddComponent(new ModelComponent(L"Meshes/Mario1.ovm"));
	m_pModelContainer->GetTransform()->Rotate(0, 180, 0);
	pModel->SetMaterial(pSkinnedMaterial);
	pModel->GetTransform()->Scale(1.3f, 1.3f, 1.3f);
	pModel->GetTransform()->Translate(0.f, -m_CharacterDesc.controller.height * 0.65f, 0.f);

	//pModel->GetTransform()->Scale(0.15f);

	pAnimator = pModel->GetAnimator();
	pAnimator->SetAnimation(m_AnimationClipId);
	pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = pAnimator->GetClipCount();
	m_ClipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto clipName = StringUtil::utf8_encode(pAnimator->GetClip(static_cast<int>(i)).name);
		const auto clipSize = clipName.size();
		m_ClipNames[i] = new char[clipSize + 1];
		strncpy_s(m_ClipNames[i], clipSize + 1, clipName.c_str(), clipSize);
	}

	const auto pObject = AddChild(new GameObject);
	m_pEmitter = pObject->AddComponent(new ParticleEmitterComponent(L"Textures/dust.png", settings, 2));
	m_pEmitter->GetTransform()->Translate(pModel->GetTransform()->GetPosition().x, pModel->GetTransform()->GetPosition().y + 2.0f, pModel->GetTransform()->GetPosition().z);


	SoundManager::Get()->GetSystem()->createSound("Resources/Sounds/dead.wav", FMOD_INIT_NORMAL, nullptr, &m_pDeadSound);
	SoundManager::Get()->GetSystem()->createSound("Resources/Sounds/jump.wav", FMOD_INIT_NORMAL, nullptr, &m_pJumpSound);

}

void MarioCharacter::Update(const SceneContext& sceneContext)
{
	Respawn(sceneContext);

	

	if (m_State == State::dead)
	{

		m_IsDead = true;
		return;
	}
	m_IsDead = false;



	if (!m_pCameraComponent->IsActive()) return;







	//constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero

	//***************
	//HANDLE INPUT
	auto input = sceneContext.pInput;

	//## Input Gathering (move)
	//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
	//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement
	XMFLOAT3 move{ 0, 0, 0 };

	if (InputManager::IsKeyboardKey(InputState::down, 'W'))
	{
		m_State = State::running;
		move.z = 1;
	}
	else if (InputManager::IsKeyboardKey(InputState::down, 'S'))
	{
		m_State = State::running;
		move.z = -1;
	}
	if (InputManager::IsKeyboardKey(InputState::down, 'D'))
	{
		m_State = State::running;
		move.x = 1;
	}
	else if (InputManager::IsKeyboardKey(InputState::down, 'A'))
	{
		m_State = State::running;
		move.x = -1;
	}
	//if (pAnimator->GetClipById() != static_cast<UINT>(m_State))
	//{
	//	pAnimator->SetAnimation(static_cast<int>(m_State));

	//}
	if (m_pControllerComponent->GetCollisionFlags().isSet(PxControllerCollisionFlag::eCOLLISION_DOWN) && (m_State == State::jumping))
	{
		m_State = State::idle;
	}
	if (InputManager::IsKeyboardKey(InputState::down, VK_SPACE))
	{
		if (pAnimator->GetClipById() != static_cast<UINT>(State::jumping))
		{
 			SoundManager::Get()->GetSystem()->playSound(m_pJumpSound, nullptr, false, nullptr);
		}
		move.y = 1;
		m_State = State::jumping;
	}

	//else
	//{
	//	m_State = State::idle;
	//	move.y = 0;
	//}
	if (pAnimator->GetClipById() != static_cast<UINT>(m_State))
	{
		pAnimator->SetAnimation(static_cast<int>(m_State));

	}

	//## Input Gathering (look)
	XMFLOAT2 look{ 0, 0 };
	bool mouseMoved{ false };

	// Store the MouseMovement in the local 'look' variable (cast is required)
	const POINT& mouseMove = InputManager::GetMouseMovement();
	look.x = static_cast<float>(mouseMove.x);
	look.y = static_cast<float>(mouseMove.y);

	mouseMoved = mouseMove.x != 0 || mouseMove.y != 0;

	//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
	//...
	//}

	if (InputManager::IsMouseButton(InputState::down, VK_LBUTTON))
	{
		look.x = 0.0f;
		look.y = 0.0f;
	}

	//		//************************
	////GATHERING TRANSFORM INFO
	//
	////Retrieve the TransformComponent

			//CALCULATE TRANSFORMS
	const auto forward = XMLoadFloat3(&m_pCameraHolder->GetTransform()->GetForward());
	const auto right = XMLoadFloat3(&m_pCameraHolder->GetTransform()->GetRight());



	//***************
	//CAMERA ROTATION

	//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable

	//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
	m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();
	m_TotalPitch += look.y * m_CharacterDesc.rotationSpeed * sceneContext.pGameTime->GetElapsed();

	//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)
	m_pCameraHolder->GetTransform()->Rotate(0.0f, m_TotalYaw, 0);
	m_pModelContainer->GetTransform()->Rotate(0.0f, m_TotalYaw - 180.0f, 0.0f, true);

	//		//********
	//		//MOVEMENT
	//
	//		//## Horizontal Velocity (Forward/Backward/Right/Left)
	//		//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
	//		
	float currentAcceleration = m_MoveAcceleration * sceneContext.pGameTime->GetElapsed();

	if (m_State == State::jumping)
	{
		currentAcceleration /= 3.0f;
		auto& settings = m_pEmitter->GetSettings();
		settings.velocity = { 0.f, -1.f,0.f };
		settings.minSize = 2.f;
		settings.maxSize = 2.f;
		settings.minScale = 1.5f;
		settings.maxScale = 3.0f;
		settings.minEmitterRadius = 0.0f;
		settings.maxEmitterRadius = 0.0f;
		settings.minEnergy = 0;
		settings.maxEnergy = 3;
	}
	else
	{
		auto& settings = m_pEmitter->GetSettings();
		settings.velocity = { 0.f,-1.f,0.f };
		settings.minSize = 0.0f;
		settings.maxSize = 0.0f;
		settings.minEnergy = 0.0f;
		settings.maxEnergy = 0.0f;
		settings.minScale = 0.0f;
		settings.maxScale = 0.0f;
		settings.minEmitterRadius = .0f;
		settings.maxEmitterRadius = .0f;
		settings.color = { 1.f,1.f,1.f, .6f };
	}

	bool W = input->IsKeyboardKey(InputState::down, 'W');
	bool A = input->IsKeyboardKey(InputState::down, 'A');
	bool S = input->IsKeyboardKey(InputState::down, 'S');
	bool D = input->IsKeyboardKey(InputState::down, 'D');







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

		//float sideRotation = move.x * 90;
		//float frontRotation = 180;
		//if (S)
		//{
		//	frontRotation = 0;
		//}

		//m_pModelContainer->GetTransform()->Rotate(0, frontRotation + sideRotation, 0);





		//XMFLOAT3 cameraPosition = m_pCameraHolder->GetTransform()->GetPosition(); // Camera position
		//XMFLOAT4 cameraRotation = m_pCameraHolder->GetTransform()->GetRotation(); // Camera rotation

		//// Assuming you have an actor object with position and rotation properties
		//XMFLOAT3 actorPosition = m_pModelContainer->GetTransform()->GetPosition(); // Actor position
		//XMFLOAT4 actorRotation = m_pModelContainer->GetTransform()->GetRotation(); // Actor rotation

		//// Calculate the direction from the actor to the camera on the XZ plane
		//float actorToCameraX = cameraPosition.x - actorPosition.x;
		//float actorToCameraZ = cameraPosition.z - actorPosition.z;

		//// Calculate the rotation angle based on the camera direction
		//float yaw = atan2(actorToCameraX, actorToCameraZ);

		//// Set the actor rotation based on the yaw
		//actorRotation.y = yaw;

		//// Apply the rotation to the actor's transformation matrix
		//XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(actorRotation.x, actorRotation.y, actorRotation.z);
		//// Apply the rotation matrix to the actor's transformation



		//float rotationAngle = atan2(m_pCameraHolder->GetTransform()->GetRotation().z - m_pCameraHolder->GetTransform()->GetRotation().z,
		//	m_pModelContainer->GetTransform()->GetPosition().x - m_pModelContainer->GetTransform()->GetPosition().x);

		//// Set the rotation of the actor
		//m_pModelContainer->GetTransform()->Rotate(0.0f, rotationAngle, 0.0f);


	}
	else
	{
		std::wstring run = L"run";
		if (run == pAnimator->GetClipName())
		{
			pAnimator->SetAnimation(0);
		}
		//move.x = 0;



		//Else (character is not moving, or stopped moving)
		//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		m_MoveSpeed -= currentAcceleration;
		if (m_MoveSpeed < 0)
		{
			//Make sure the current MoveSpeed doesn't get smaller than zero
			m_MoveSpeed = 0;
		}
	}


	//		//Now we can calculate the Horizontal Velocity which shoulsd be stored in m_TotalVelocity.xz
	//		//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)
	//		//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
	//		//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)
	m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
	m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;
	if (m_MoveSpeed > 0)
	{
		std::cout << m_MoveSpeed << '\n';
	}

	//m_TotalVelocity.x = m_pCameraHolder->GetTransform()->GetForward().x * m_MoveSpeed;
	//m_TotalVelocity.z = m_pCameraHolder->GetTransform()->GetRight().z * m_MoveSpeed;

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

		if (m_TotalVelocity.y <= -m_CharacterDesc.maxFallSpeed)
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

	//		//************
	//		//DISPLACEMENT
	//
	//		//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
	//		//Calculate the displacement (m) for the current frame and move the ControllerComponent

	XMFLOAT3 displacement;
	XMStoreFloat3(&displacement, XMVectorScale(XMLoadFloat3(&m_TotalVelocity), sceneContext.pGameTime->GetElapsed()));
	//std::cout << m_TotalVelocity.x << ' ' << m_TotalVelocity.y << ' ' << m_TotalVelocity.z << '\n';

	m_pControllerComponent->Move(displacement);
	m_pControllerComponent->Move(displacement);
	//
	//		//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
	//		//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)
	XMFLOAT3 pos = m_pControllerComponent->GetTransform()->GetPosition();
	std::cout << pos.x << "    " << pos.y << "   " << pos.z << '\n';
}

void MarioCharacter::DrawImGui()
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
		if (ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}

	if (ImGui::Button(pAnimator->IsPlaying() ? "PAUSE" : "PLAY"))
	{
		if (pAnimator->IsPlaying())pAnimator->Pause();
		else pAnimator->Play();
	}

	if (ImGui::Button("RESET"))
	{
		pAnimator->Reset();
	}

	ImGui::Dummy({ 0,5 });

	bool reversed = pAnimator->IsReversed();
	if (ImGui::Checkbox("Play Reversed", &reversed))
	{
		pAnimator->SetPlayReversed(reversed);
	}

	if (ImGui::ListBox("Animation Clip", &m_AnimationClipId, m_ClipNames, static_cast<int>(m_ClipCount)))
	{
		pAnimator->SetAnimation(m_AnimationClipId);
	}

	if (ImGui::SliderFloat("Animation Speed", &m_AnimationSpeed, 0.f, 4.f))
	{
		pAnimator->SetAnimationSpeed(m_AnimationSpeed);
	}

}

void MarioCharacter::AddStar()
{
	++m_NrStars;
}

void MarioCharacter::Respawn(const SceneContext& sceneContext)
{
	if (m_CurrLives > m_NrLives)
	{
		--m_CurrLives;
		if (m_State != State::dead)
		{
			SoundManager::Get()->GetSystem()->playSound(m_pDeadSound, nullptr, false, nullptr);

			m_State = State::dead;
			pAnimator->SetAnimation(static_cast<int>(m_State));
		}
	}
	if (m_State == State::dead)
	{
		m_TimeSinceDead += sceneContext.pGameTime->GetElapsed();
		if (m_TimeSinceDead > m_TimeToRespawn)
		{
			m_TimeSinceDead = 0.0f;
			m_State = State::idle;
			pAnimator->SetAnimation(static_cast<int>(m_State));
			GetTransform()->Translate(m_StartLocation);
		}
	}


}
