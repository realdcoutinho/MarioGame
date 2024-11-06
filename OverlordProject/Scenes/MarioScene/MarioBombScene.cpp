#include "stdafx.h"
#include "MarioBombScene.h"


#include <string>
#include <locale>
#include <codecvt>

#include "stdafx.h"
#include "Materials/DiffuseMaterial.h"
#include "Materials/Aberration.h"
#include "Materials/DiffuseMaterials_FlipNormals.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow_NegativeNormals.h"
#include "Prefabs/Character.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Materials/ColorMaterial.h"
#include "Scenes/MarioScene/Objects/RotatingBall.h"
#include "HUD/HUD.h"

#include <iostream>
#include <cmath>


//void MarioBombScene::DeleteGombas()
//{
//
//}



void MarioBombScene::Initialize()
{

	//zm_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug = false;


	//Ground Plane
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	GameSceneExt::CreatePhysXGroundPlane(*this, pDefaultMaterial);

	//auto pLevel = new Level();
	//pLevel->Initialize();
	CreateLevel();
	//CreateGomba();


	//Character
	CharacterDesc characterDesc{ pDefaultMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;


	XMFLOAT3 startLocation{ 127.0f, 7.0f, 126.f };
	m_pMario = AddChild(new MarioCharacter(characterDesc, startLocation));
	m_pMario->GetTransform()->Translate(startLocation);






	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	m_SceneContext.pLights->SetDirectionalLight(XMFLOAT3(m_SunLocation), XMFLOAT3(-0.577f, -0.577f, 0.577f));

	//m_SceneContext.pLights->GetLight().range


	XMFLOAT3 gombaPos1 = { 40, 2, 88 };
	Goomba* pGomba1 = AddChild(new Goomba(gombaPos1));
	pGomba1->GetTransform();

	XMFLOAT3 gombaPos2 = { 62, 2, 45 };
	Goomba* pGomba2 = AddChild(new Goomba(gombaPos2));
	pGomba2->GetTransform();

	XMFLOAT3 gombaPos3 = { 53, 2, -29 };
	Goomba* pGomba3 = AddChild(new Goomba(gombaPos3));
	pGomba3->GetTransform();

	XMFLOAT3 gombaPos4 = { 119, 23, -13 };
	Goomba* pGomba4 = AddChild(new Goomba(gombaPos4));
	pGomba4->GetTransform();

	XMFLOAT3 gombaPos5 = { 113, 18, 47 };
	Goomba* pGomba5 = AddChild(new Goomba(gombaPos5));
	pGomba5->GetTransform();


	XMFLOAT3 starPos1 = { 149, 18+3, 79 };
	Star* pStar1 = AddChild(new Star(starPos1));
	pStar1->GetTransform();

	XMFLOAT3 starPos2 = { 63.8f, 18 + 3, 34 };
	Star* pStar2 = AddChild(new Star(starPos2));
	pStar2->GetTransform();

	XMFLOAT3 starPos3 = { -11, 18 + 3, 25 };
	Star* pStar3 = AddChild(new Star(starPos3));
	pStar3->GetTransform();

	XMFLOAT3 starPos4 = { -104, 17 + 3, 23 };
	Star* pStar4 = AddChild(new Star(starPos4));
	pStar4->GetTransform();

	XMFLOAT3 starPos5 = { -21, 18 + 3, 146 };
	Star* pStar5 = AddChild(new Star(starPos5));
	pStar5->GetTransform();

	XMFLOAT3 starPos6 = { 15, 2 + 3, 127 };
	Star* pStar6 = AddChild(new Star(starPos6));
	pStar6->GetTransform();

	XMFLOAT3 starPos7 = { -122, 44 + 3, 127 };
	Star* pStar7 = AddChild(new Star(starPos7));
	pStar7->GetTransform();

	XMFLOAT3 starPos8 = { -38, 87 + 3, -91 };
	Star* pStar8 = AddChild(new Star(starPos8));
	pStar8->GetTransform();





	//XMFLOAT3 poisi = { 80.0f, 10.0f, 90.f };
	//m_pGomba = AddChild(new Goomba(poisi));

	//XMFLOAT3 poisiton = { 150.0f, 0.0f, 130.f };
	//m_pStar = AddChild(new Star(poisiton));

	//XMFLOAT3 po = { 130.0f, 0.0f, 130.f };
	//auto pStar = AddChild(new Star(po));
	//m_pStarsVector.emplace_back(pStar);


	m_HUD = AddChild(new HUD());
	CreatePlatform();




	//m_pBall1 = AddChild(new RotatingBall(XMFLOAT3{20.7f, 60.0f, -81.0f}, XMFLOAT3{ 20.7f, 55.0f, -81.0f }));
	m_pBall1 = AddChild(new RotatingBall(XMFLOAT3{ 20.7f, 60.0f, -81.0f }, XMFLOAT3{ 20.7f, 55.0f, -81.0f }));
	//m_pBall2 = AddChild(new RotatingBall(XMFLOAT3{ -41.f, 77.0f, -121.0f }, XMFLOAT3{ 20.7f, 55.0f, -81.0f }));

	m_pPostAberration = MaterialManager::Get()->CreateMaterial<Aberration>();
	AddPostProcessingEffect(m_pPostAberration);

}


void MarioBombScene::Update()
{	
	OnDistanceCall(m_pBall1);
	//OnDistanceCall(m_pBall2);
	if (m_pMario->m_IsDead)
		m_pPostAberration->SetIsEnabled(true);
	else
		m_pPostAberration->SetIsEnabled(false);
}

float MarioBombScene::CalculateDistance(const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	DirectX::XMVECTOR vec1 = DirectX::XMLoadFloat3(&pos1);
	DirectX::XMVECTOR vec2 = DirectX::XMLoadFloat3(&pos2);
	DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(vec2, vec1);
	DirectX::XMVECTOR lengthSquared = DirectX::XMVector3LengthSq(diff);
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, lengthSquared);

	//double distance = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2);
	//return static_cast<float>(distance);

	return result.x;
}

void MarioBombScene::OnDistanceCall(RotatingBall* ball)
{
	if (!ball->GetIsBallDestroyed())
	{
		float treshold = 0.9f;
		double distanceCheck = pow(ball->GetBallRadius() + treshold, 2);
		float distance = CalculateDistance(ball->ReturnBallPosition(), m_pMario->GetTransform()->GetPosition());
		if (distance < static_cast<float>(distanceCheck))
		{
			ball->OnTrigger();
		}
	}
}





inline std::wstring narrowToWide(const std::string& narrowString)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, nullptr, 0);
	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_UTF8, 0, narrowString.c_str(), -1, buffer, length);
	std::wstring wideString(buffer);
	delete[] buffer;
	return wideString;
}

//void MarioBombScene::CreateGomba()
//{
//	//const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
//
//	float scaleFactor = 0.04f;
//	XMFLOAT3 scaleVector{ scaleFactor, scaleFactor, scaleFactor };
//	//const auto gomba = AddChild(new GameObject());
//	//auto meshGomba = gomba->AddComponent(new ModelComponent(L"Meshes/Characters/gomba.ovm"));
//	//meshGomba->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());
//	//meshGomba->GetTransform()->Scale(scaleVector);
//	//meshGomba->GetTransform()->Rotate(0, 0, 0);
//
//
//	//const auto gombaActor = gomba->AddComponent(new RigidBodyComponent(true));
//	//const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Characters/gomba.ovpt");
//	//gombaActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ scaleFactor, scaleFactor, scaleFactor })), *pDefaultMaterial);
//	//gomba->GetTransform()->Scale(scaleVector);
//
//	std::wstring name(L"goomba_tex.PNG");
//
//
//	//ModelComponent* gombaComponent{ new ModelComponent{L"Meshes/Characters/gomba.ovm"} };
//	//const auto gomba = new GameObject{};
//	//auto meshGomba = gomba->AddComponent<ModelComponent>(gombaComponent);
//	//meshGomba->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());
//	//meshGomba->GetTransform()->Scale(scaleVector);
//	//meshGomba->GetTransform()->Rotate(0, 0, 0);
//	//AddChild(gomba);
//
//
//	//auto pConvexChair = ContentManager::Load<PxConvexMesh>(L"Meshes/Characters/gomba.ovpt");
//	//auto pRigidBody = gomba->AddComponent(new RigidBodyComponent(false));
//	//pRigidBody->AddCollider(PxConvexMeshGeometry(pConvexChair), *pDefaultMaterial);
//	//pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexChair }, *pDefaultMaterial);
//
//
//	//gomba->GetTransform()->Scale(scaleVector);
//
//	auto& physx = PxGetPhysics();
//
//	auto pBouncyMaterial = physx.createMaterial(0.0f, 0.0f, 0.0f);
//	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);
//
//	//auto materialManager = MaterialManager::Get();
//
//
//	////DiffuseMaterial
//	//auto materialDiffuse = materialManager->CreateMaterial<DiffuseMaterial>();
//	//materialDiffuse->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
//	//auto materialDiffuseID = materialDiffuse->GetMaterialId();
//
//
//	ModelComponent* modelComponent{ new ModelComponent{L"Meshes/Characters/gomba.ovm"} };
//	auto chair = new GameObject{};
//	chair->AddComponent<ModelComponent>(modelComponent);
//	chair->GetTransform()->Scale(scaleVector);
//	AddChild(chair);
//
//	auto pConvexChair = ContentManager::Load<PxConvexMesh>(L"Meshes/Characters/gomba.ovpc");
//	auto pRigidBody = chair->AddComponent(new RigidBodyComponent(false));
//	pRigidBody->AddCollider(PxConvexMeshGeometry{ pConvexChair, PxMeshScale({ scaleFactor, scaleFactor, scaleFactor }) }, *pBouncyMaterial);
//
//	//modelComponent->SetMaterial(materialDiffuseID);
//
//
//
//	chair->GetTransform()->Translate(100.0f, 4.0f, 130.f);
//	chair->GetTransform()->Rotate(0, 90, 0);
//
//
//
//
//
//
//
//
//
//	DiffuseMaterial_Shadow_NegativeNormals* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_NegativeNormals>();
//	pMaterial->SetDiffuseTexture(L"Textures/GombaTextures/" + name);
//	modelComponent->SetMaterial(pMaterial->GetMaterialId(), 0U);
//
//
//
//
//
//	//std::ifstream objFile(L"Resources/Obj/goomba.obj");
//
//	//std::ifstream mtlFile(L"Resources/Mtl/goomba.mtl");
//
//	//std::wstring folderName(L"GombaTextures");
//
//	//AssignMaterials(meshGomba, objFile, mtlFile, folderName);
//
//	//meshGomba->GetTransform()->Translate(135.0f, 5.0f, 140.f);
//
//}


void MarioBombScene::CreateLevel()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	//Simple Level
	const auto pLevelObject = AddChild(new GameObject());
	auto pLevelMesh = pLevelObject->AddComponent(new ModelComponent(L"Meshes/Field/Field1.ovm", true, true));
	pLevelMesh->SetMaterial(MaterialManager::Get()->CreateMaterial<ColorMaterial>());
	pLevelMesh->GetTransform()->Scale({ 20, 20, 20 });
	pLevelMesh->GetTransform()->Rotate(0, 0, 0);

	const auto pLevelActor = pLevelObject->AddComponent(new RigidBodyComponent(true));
	const auto pPxTriangleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Field/Field1.ovpt");
	pLevelActor->AddCollider(PxTriangleMeshGeometry(pPxTriangleMesh, PxMeshScale({ 20, 20, 20 })), *pDefaultMaterial);
	pLevelObject->GetTransform()->Scale({ 20, 20, 20 });

	std::ifstream objFile(L"Resources/Obj/Bob-omb Battlefield.obj");
	std::ifstream mtlFile(L"Resources/Mtl/Bob-omb Battlefield.mtl");
	std::wstring folderName( L"LevelTextures" );

	AssignMaterials(pLevelMesh, objFile, mtlFile, folderName);
}

void MarioBombScene::AssignMaterials(ModelComponent* mesh, std::ifstream& objFile, std::ifstream& mtlFile, std::wstring& folderName)
{
	//check if the files opened correctly
	if (!mtlFile.is_open())
	{
		// Error opening file
	}
	if (!objFile.is_open())
	{
		// Error opening file
	}

	// Initialize variables
	std::string line;
	std::string currentMeshName;
	std::unordered_map<std::string, DiffuseMaterial_Shadow_NegativeNormals*> materials;
	std::unordered_map<std::string, std::string> materialsNamesDebug;
	std::vector<DiffuseMaterial_Shadow_NegativeNormals*> materialsVector;

	int count{};
	while (std::getline(objFile, line))
	{
		std::string searchTerm = "usemtl ";
		// Check if the line defines a new mesh
		if (line.find(searchTerm) != std::string::npos)
		{
			// Extract the mesh name from the line
			currentMeshName = line.substr(searchTerm.length());
			std::cout << currentMeshName << std::endl;

			// Check if the material for the mesh has already been created
			if (materials.find(currentMeshName) == materials.end()) {
				// Find the corresponding material name in the MTL file
				std::string materialName = "default";
				std::string mtlLine;
				//bool materialfound = false;
				bool found = false;
				while (std::getline(mtlFile, mtlLine))
				{
					if (mtlLine.find(currentMeshName) != std::string::npos)
					{
						// Extract the material name from the line
						while (std::getline(mtlFile, mtlLine))
						{
							++count;
							if (mtlLine.find("map_Kd") != std::string::npos)
							{
								
								// Extract the material name from the line
								materialName = mtlLine.substr(17);

								DiffuseMaterial_Shadow_NegativeNormals* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_NegativeNormals>();
								pMaterial->SetDiffuseTexture(L"Textures/" + folderName + L"/" + narrowToWide(materialName));
								//pMaterial->FlipLight();
								materials[currentMeshName] = pMaterial;
								materialsNamesDebug[currentMeshName] = materialName;
								materialsVector.push_back(pMaterial);
								found = true;

							break;
							}

						}
					}
					if (found)
						break;

				}
				mtlFile.clear();
				mtlFile.seekg(0);
			}
		}
	}

	UINT8 size = static_cast<UINT8>(materialsVector.size());
	for (UINT8 pos{ 0 }; pos < size; ++pos)
	{
		if(pos != 0 && pos != 1)
			mesh->SetMaterial(materialsVector.at(pos), pos);
		if(pos == 0)
		{
			UberMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
			pMaterial->SetDiffuseTexture(L"Textures/" + folderName + L"/" + narrowToWide("574B138E_c.png"));
			pMaterial->SetVariable_Scalar(L"gUseTextureDiffuse", true); 
			pMaterial->SetOpacityTexture(L"Textures/" + folderName + L"/" + narrowToWide("Grid_OpacityMap.jpg"));
			pMaterial->SetVariable_Scalar(L"gTextureOpacityIntensity", true);
			mesh->SetMaterial(pMaterial, 0);
		}
		if (pos == 1)
		{
			UberMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
			pMaterial->SetDiffuseTexture(L"Textures/" + folderName + L"/" + narrowToWide("41A41EE3_c.png"));
			pMaterial->SetVariable_Scalar(L"gUseTextureDiffuse", true);
			pMaterial->SetOpacityTexture(L"Textures/" + folderName + L"/" + narrowToWide("Bars_OpacityMap.jpg"));
			pMaterial->SetVariable_Scalar(L"gTextureOpacityIntensity", true);
			mesh->SetMaterial(pMaterial, 1);
		}
	}

	mesh->GetTransform();
	materials;
	materialsNamesDebug;
	// Close the files
	objFile.close();
	mtlFile.close();
}

void MarioBombScene::OnGUI()
{
	m_pMario->DrawImGui();
	//m_pGrid->DrawImGui();

	ImGui::Checkbox("Draw ShadowMap", &m_DrawShadowMap);
	ImGui::SliderFloat("ShadowMap Scale", &m_ShadowMapScale, 0.f, 1.f);

	//ImGui::SliderFloat("Light Direction", &m_SceneContext.pLights->GetLight(0).direction.x, 0.0f, 1.0f);
	//ImGui::SliderFloat("Light Direction", &m_SceneContext.pLights->GetLight(0).direction.y, 0.0f, 1.0f);
	//ImGui::SliderFloat("Light Direction", &m_SceneContext.pLights->GetLight(0).direction.z, 0.0f, 1.0f);
	//ImGui::SliderFloat("Light Direction", &m_SceneContext.pLights->GetLight(0).direction.w, 0.0f, 1.0f);
	//MaterialManager::Get()->GetMaterial(2)->DrawImGui();

	//m_pStarSprite->GuiCall();
}

void MarioBombScene::PostDraw()
{
	//Draw ShadowMap (Debug Visualization)
	if (m_DrawShadowMap) {

		ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
	}
}

void MarioBombScene::CreatePlatform()
{
	const auto pDefaultMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);
	XMFLOAT3 dimensions{ 30.0f, 1.0f, 7.0f };
	PxBoxGeometry cubeGeometry = PxBoxGeometry(dimensions.x / 2.f, dimensions.y / 2.f, dimensions.z / 2.f);

	m_pPlatform = new CubePrefab(dimensions, XMFLOAT4{ Colors::BlanchedAlmond });
	auto rigid = m_pPlatform->AddComponent(new RigidBodyComponent(true));
	rigid->AddCollider(PxBoxGeometry{ dimensions.x / 2, dimensions.y / 2, dimensions.z / 2 }, *pDefaultMaterial, false);

	m_pPlatform->GetTransform()->Translate(62.5f, 15.0f, 34.6f);
	m_pPlatform->GetTransform()->Rotate(0.0f, 45.0f, 0.0f);
	AddChild(m_pPlatform);	
}

void MarioBombScene::RotatePlatform(float dt)
{
	//if(m_Angle < 45.0f)
	m_pPlatform->GetTransform()->Translate(-62.5f, -15.0f, -34.6f);
		m_pPlatform->GetTransform()->Rotate(0.0f, -45.0f, 0.0f);
		m_Angle += 10 + dt;
		m_pPlatform->GetTransform()->Rotate(0.0f, 0.0f, m_Angle);
		m_pPlatform->GetTransform()->Rotate(0.0f, 45.0f, 0.0f);
		m_pPlatform->GetTransform()->Translate(62.5f, 15.0f, 34.6f);
}