#include "stdafx.h"
#include "MainMenuScene.h"

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.enableOnGUI = true;



	m_pBackgroundSprite = new GameObject();
	m_pBackgroundSprite->AddComponent(new SpriteComponent(L"Textures/MainMenuBackground.jpg", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.0}));
	AddChild(m_pBackgroundSprite);


	m_pButtonQuestion = new GameObject();
	m_pButtonQuestion->AddComponent(new SpriteComponent(L"Textures/question.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.0f }));
	AddChild(m_pButtonQuestion);

	m_pControlWindow = new GameObject();
	m_pControlWindow->AddComponent(new SpriteComponent(L"Textures/controls.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.0f }));
	AddChild(m_pControlWindow);

	m_pButtonStart = new GameObject();
	m_pButtonStart->AddComponent(new SpriteComponent(L"Textures/Start.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,1.0f }));
	AddChild(m_pButtonStart);


	m_pBackgroundSprite->GetTransform()->Scale(1.95f, 1.95f, 0.0f);
	m_pBackgroundSprite->GetTransform()->Translate(m_SceneContext.windowWidth/2, m_SceneContext.windowHeight / 2, .9f);

	m_pButtonQuestion->GetTransform()->Translate(66.66f, 653.33f, .6f);
	m_pButtonQuestion->GetTransform()->Scale(0.188f, 0.188f, 0.0f);

	m_pControlWindow->GetTransform()->Translate(-m_SceneContext.windowWidth / 2, -m_SceneContext.windowHeight / 2, .5f);
	//m_pControlWindow->GetTransform()->Scale(0.188f, 0.188f, 0.0f);

	m_pButtonStart->GetTransform()->Translate(66.67f, 533.0f, .3f);
	m_pButtonStart->GetTransform()->Scale(0.25f, 0.406f, 0.0f);


	const auto pSpriteComponent = m_pButtonQuestion->GetComponent<SpriteComponent>();
	m_ControlsButton.width = pSpriteComponent->GetWidth() * 0.188f;
	m_ControlsButton.height = pSpriteComponent->GetHeight() * 0.188f;
	XMFLOAT2 loc = { pSpriteComponent->GetTransform()->GetPosition().x, pSpriteComponent->GetTransform()->GetPosition().y };
	m_ControlsButton.center = loc;

	const auto pSpriteComponent1 = m_pButtonStart->GetComponent<SpriteComponent>();
	m_StartButton.width = pSpriteComponent1->GetWidth() * 0.25f;
	m_StartButton.height = pSpriteComponent1->GetHeight() * 0.406f;
	XMFLOAT2 loc1 = { pSpriteComponent1->GetTransform()->GetPosition().x, pSpriteComponent1->GetTransform()->GetPosition().y };
	m_StartButton.center = loc1;

}

void MainMenuScene::Update()
{
	m_MousePos = { 0.0f, 0.0f };
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{

		// Store the MouseMovement in the local 'look' variable (cast is required)
		const POINT& mouseMove = InputManager::GetMousePosition();
		
		m_MousePos.x = static_cast<float>(mouseMove.x);
		m_MousePos.y = static_cast<float>(mouseMove.y);

		//mouseMoved = mouseMove.x != 0 || mouseMove.y != 0;

		//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look
		//...
		//}

		if (m_ControlsButton.Clicked(m_MousePos))
		{
			if (m_IsControlsActive)
			{
				m_pControlWindow->GetTransform()->Translate(-m_SceneContext.windowWidth / 2, -m_SceneContext.windowHeight / 2, .5f);
				m_IsControlsActive = false;
				return;
			}
			if (!m_IsControlsActive)
			{
				m_pControlWindow->GetTransform()->Translate(m_SceneContext.windowWidth / 2, m_SceneContext.windowHeight / 2, .5f);
				m_IsControlsActive = true;
				return;
			}
		}
		if (m_StartButton.Clicked(m_MousePos))
		{
			if (m_IsControlsActive) return;

			SceneManager::Get()->NextScene();
		}

	}
}

void MainMenuScene::OnGUI()
{
	const auto pSpriteComponent = m_pButtonStart->GetComponent<SpriteComponent>();

	ImGui::SliderFloat2("Position", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetTransform()->GetPosition()), 0, m_SceneContext.windowWidth);
	ImGui::SliderFloat2("Scale", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetTransform()->GetScale()), 0, 3);
	ImGui::SliderFloat2("Pivot", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetPivot()), 0, 1);
	ImGui::ColorEdit4("Color", ConvertUtil::ToImFloatPtr(pSpriteComponent->GetColor()));
	//ImGui::Checkbox("AutoRotate", &m_AutoRotate);
}
