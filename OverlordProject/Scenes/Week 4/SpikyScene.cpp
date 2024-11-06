#include "stdafx.h"
#include "SpikyScene.h"
#include <Materials/SpikyMaterial.h>
#include <Materials/ColorMaterial.h>


void SpikyScene::Initialize()
{
	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/OctaSphere.ovm"} };
	m_pSphere = new GameObject{};
	m_pSphere->AddComponent<ModelComponent>(modelComponent);
	AddChild(m_pSphere);

	float scaleFactor{ 15.0f };
	XMFLOAT3 scale{ scaleFactor , scaleFactor , scaleFactor };
	m_pSphere->GetTransform()->Scale(scale);

	auto materialManager = MaterialManager::Get();

	////ColorMaterial
	//auto materialColor = materialManager->CreateMaterial<ColorMaterial>();
	//materialColor->SetColor(XMFLOAT4{ Colors::Red });
	//auto marterialColorID = materialColor->GetMaterialId();


	////ColorMaterial
	//modelComponent->SetMaterial(marterialColorID);

	m_pMaterialSpiky = materialManager->CreateMaterial<SpikyMaterial>();
	//materialSpike->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	auto materialSpikyID = m_pMaterialSpiky->GetMaterialId();
	modelComponent->SetMaterial(materialSpikyID);
	//materialSpiky.InitializeEffectVariables();


	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
}

void SpikyScene::Update()
{
	m_Angle += m_Speed * m_SceneContext.pGameTime->GetElapsed();
	m_pSphere->GetTransform()->Rotate(0.0f, m_Angle, 0.0f);
}


void SpikyScene::OnGUI()
{
	m_pMaterialSpiky->DrawImGui();
}