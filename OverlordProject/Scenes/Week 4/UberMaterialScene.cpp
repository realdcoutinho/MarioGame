#include "stdafx.h"
#include "UberMaterialScene.h"
#include <Materials/ColorMaterial.h>
#include <Materials/UberMaterial.h>

void UberMaterialScene::Initialize()
{
	auto materialManager = MaterialManager::Get();

	////ColorMaterial
	//auto materialColor = materialManager->CreateMaterial<ColorMaterial>();
	//materialColor->SetColor(XMFLOAT4{ Colors::Red });
	//auto marterialColorID = materialColor->GetMaterialId();


	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Sphere.ovm"} };
	m_pSphere = new GameObject{};
	m_pSphere->AddComponent<ModelComponent>(modelComponent);
	AddChild(m_pSphere);

	float scale{ 25.0f };
	m_pSphere->GetTransform()->Scale(XMFLOAT3{ scale,scale, scale });


	//modelComponent->SetMaterial(marterialColorID);

	//DiffuseMaterial
	m_pUberMaterial = materialManager->CreateMaterial<UberMaterial>();
	m_pUberMaterial->SetDiffuseTexture(L"Textures/Skulls_Diffusemap.tga");
	m_pUberMaterial->SetNormalTexture(L"Textures/Skulls_Normalmap.tga");
	m_pUberMaterial->SetSpecularTexture(L"Textures/Skulls_Heightmap.tga");
	m_pUberMaterial->SetOpacityTexture(L"Textures/Skulls_Heightmap.tga");
	m_pUberMaterial->SetEnviornmentTexture(L"Textures/CubeMap.dds");
	auto materialDiffuseID = m_pUberMaterial->GetMaterialId();


	//m_pUberMaterial.
	//modelComponent->SetMaterial(materialDiffuseID);
	modelComponent->SetMaterial(materialDiffuseID);


	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
}

void UberMaterialScene::Update()
{
	m_Angle += m_Speed * m_SceneContext.pGameTime->GetElapsed();
	m_pSphere->GetTransform()->Rotate(0.0f, m_Angle, 0.0f);
}

void UberMaterialScene::OnGUI()
{
	m_pUberMaterial->DrawImGui();




	//const std::string buttonText{ "Material ID #" + std::to_string(m_pUberMaterial->GetMaterialId()) };
	//if (ImGui::Button(buttonText.c_str()))
	//{
	//	m_MaterialPopupWindowOpwn = true;
	//}
	//if (m_MaterialPopupWindowOpwn)
	//{
	//	if (ImGui::Begin("Dear ImGui Demo", &m_MaterialPopupWindowOpwn))
	//	{
	//		//USE TEXTURE DIFFUSE
	//		bool useTextureDiffuse = std::any_cast<bool>(m_pUberMaterial->GetValueFromMap("gUseTextureDiffuse"));
	//		ImGui::Checkbox("Use Texure Diffuse", &useTextureDiffuse);
			//m_pUberMaterial->SetValueFromMap("gUseTextureDiffuse", useTextureDiffuse);


	//		//COLOR DIFFUSE
	//		ImGuiColorEditFlags colorFlagsDiffuse = ImGuiColorEditFlags_NoInputs;
	//		XMFLOAT4 diffuseColor = std::any_cast<XMFLOAT4>(m_pUberMaterial->GetValueFromMap("gColorDiffuse"));
	//		ImGui::ColorEdit4("Diffuse Color", ConvertUtil::ToImFloatPtr(diffuseColor), colorFlagsDiffuse);
			//m_pUberMaterial->SetValueFromMap("gColorDiffuse", diffuseColor);
	//		
	//		ImGui::NewLine();
	//		ImGui::PushFont(ImguiFonts::pFont_DIN_Black_18);
	//		ImGui::Text("Specular");
	//		ImGui::PopFont();

	//		//COLOR SPECULAR
	//		ImGuiColorEditFlags colorFlagsSpecular = ImGuiColorEditFlags_NoInputs;
	//		XMFLOAT4 specularColor = std::any_cast<XMFLOAT4>(m_pUberMaterial->GetValueFromMap("gColorSpecular"));
	//		ImGui::ColorEdit4("Specular Color", ConvertUtil::ToImFloatPtr(specularColor), colorFlagsSpecular);
	//		m_pUberMaterial->SetValueFromMap("gColorSpecular", specularColor);


	//		bool useTextureSpecularIntensity = std::any_cast<bool>(m_pUberMaterial->GetValueFromMap("gUseTextureSpecularIntensity"));
	//		ImGui::Checkbox("Use Texure Diffuse", &useTextureSpecularIntensity);
	//		m_pUberMaterial->SetValueFromMap("gUseTextureSpecularIntensity", useTextureSpecularIntensity);





	//		m_pUberMaterial->InitializeEffectVariables();



	//		ImGui::End();
	//		return;
	//	}
	//}


}

