#pragma once
#include "Level.h"
#include "Characters/MarioCharacter.h"
#include "Scenes/MarioScene/Enemies/Goomba.h"
#include "Scenes/MarioScene/Objects/Star.h"
#include "Scenes/MarioScene/Objects/RotatingBall.h"
#include "Prefabs/CubePrefab.h"





class Character;
class MarioCharacter;
class HUD;
class Aberration;

class MarioBombScene : public GameScene
{
public:
	MarioBombScene() :GameScene(L"MarioBombScene") {}
	~MarioBombScene() override = default;
	MarioBombScene(const MarioBombScene& other) = delete;
	MarioBombScene(MarioBombScene&& other) noexcept = delete;
	MarioBombScene& operator=(const MarioBombScene& other) = delete;
	MarioBombScene& operator=(MarioBombScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;

private:

	void CreateLevel();
	//void CreateGomba();
	//void CreateMeshCollsions();
	void AssignMaterials(ModelComponent* mesh, std::ifstream& objFile, std::ifstream& mtlFile, std::wstring& folderName);
	void Update() override;

	float CalculateDistance(const XMFLOAT3& pos1, const XMFLOAT3& pos2);
	void OnDistanceCall(RotatingBall* ball);

	void CreatePlatform();
	void RotatePlatform(float dt);
	float m_Angle{ 0.0f };

	void PostDraw() override;

	

	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump
	};

	Character* m_pCharacter{};
	MarioCharacter* m_pMario{};
	Goomba* m_pGomba{};
	Star* m_pStarSprite{};
	Star* m_pStar{};
	HUD* m_HUD{};
	//static std::vector<Goomba*> m_pGombasVector;
	std::vector<Star*> m_pStarsVector;
	UberMaterial* m_pGrid{};
	UberMaterial* m_pBars{};
	CubePrefab* m_pPlatform{};

	RotatingBall* m_pBall1{ };
	RotatingBall* m_pBall2{ };
	

	Level* m_pLevel{};
	

	bool m_DrawShadowMap{ false };
	float m_ShadowMapScale{ 0.3f };
	XMFLOAT3 m_SunLocation{120, 130, -100};


	Aberration* m_pPostAberration{};

};

