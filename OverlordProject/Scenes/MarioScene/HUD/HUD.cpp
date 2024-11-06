#include "stdafx.h"
#include "HUD.h"
#include <cstdio>


HUD::HUD()
{

}

void HUD::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pSpriteMarioLives = new GameObject();
	m_pSpriteMarioLives->AddComponent(new SpriteComponent(L"Textures/MarioHeadHUD.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.7f }));
	AddChild(m_pSpriteMarioLives);

	m_pSpriteMarioLives->GetTransform()->Translate(50, 50, .9f);
	m_pSpriteMarioLives->GetTransform()->Scale(0.2f, 0.2f, 0.2f);



	m_pSpriteMarioStars = new GameObject();
	m_pSpriteMarioStars->AddComponent(new SpriteComponent(L"Textures/Star.png", { 0.5f,0.5f }, { 1.f,1.f,1.f,.7f }));
	AddChild(m_pSpriteMarioStars);

	m_pSpriteMarioStars->GetTransform()->Translate(1220, 50, .9f);
	m_pSpriteMarioStars->GetTransform()->Scale(0.5f, 0.5f, 0.2f);

	m_pFontLives = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");
	m_pFontStars = ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt");

}

void HUD::Update(const SceneContext& /*sceneContext*/)
{

	m_NrLives = MarioCharacter::m_NrLives;
	m_NrStars = MarioCharacter::m_NrStars;


	std::string stars = std::to_string(m_NrStars);
	std::string starsInfo = "/8";
	std::string starsTotal = stars + starsInfo;

	std::string lives = std::to_string(m_NrLives);
	std::string livesInfo = "/3";
	std::string livesTotal = lives + livesInfo;


	TextRenderer::Get()->DrawText(m_pFontLives, StringUtil::utf8_decode(livesTotal), m_TextPosition1, m_TextColor);
	TextRenderer::Get()->DrawText(m_pFontStars, StringUtil::utf8_decode(starsTotal), m_TextPosition2, m_TextColor);


}
