#pragma once
#include "DiffuseMaterial.h"
#include <iostream>
#include <map>
#include <string>
#include <any>

class UberMaterial final : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetNormalTexture(const std::wstring& assetFile);
	void SetSpecularTexture(const std::wstring& assetFile);
	void SetOpacityTexture(const std::wstring& assetFile);
	void SetEnviornmentTexture(const std::wstring& assetFile);

	void InitializeEffectVariables() override;
	//void UpdateEffectVariables();

	//void UpdateBools(string variable, bool value);
	//bool GetBoolMap(string variable);

	//std::any GetValueFromMap(const std::string string) const;
	//void SetValueFromMap(const std::string string, std::any value);

protected:

	void InitializeOpacityVaribles();
	void InitializeEnviormentVariables();

private:
	

	//std::map<std::string, std::any> m_VariablesMap;  // creates a map with string keys and any type values
	//void CreateMap();


	TextureData* m_pOpacityTexture{ nullptr };
	TextureData* m_pEnviornmentTexture{ nullptr };

	//Diffuse
	void InitializeDiffuseVariables();
	TextureData* m_pDiffuseTexture{ nullptr };
	//bool m_gUseTextureDiffuse{ true };
	//XMFLOAT4 m_DiffuseColor{ 1, 1, 1, 1 };

	//Normnal
	void InitializeNormalVariables();
	TextureData* m_pNormalTexture{ nullptr };
	/*bool m_gUseTextureNormal{ true };*/

	//Specular
	void InitializeSpecularVariables();
	TextureData* m_pSpecularTexture{ nullptr };
	/*bool m_gUseTextureSpecularIntensity{true};
	bool m_gUseSpecularPhong{ true };
	bool m_gUseSpecularBlinn{ true };
	XMFLOAT4 m_SpecularColor{ 0, 1, 0, 1 };*/

	//Opacity
//	bool m_gTextureOpacityIntensity{ true };
//
//	//Enviorment
//	bool m_gUseEnvironmentMapping{ true };
//	float m_gReflectionStrength{ 0.8f };
//	float m_gRefractionStrength{ 0.5f };
//	float m_gRefractionIndex{ 0.9f };
//
//	//Fresnel
//	bool m_gUseFresnelFalloff{ true };
//	float m_gFresnelPower{ 2.0f };
//	float m_gFresnelMultiplier{ 1.5f };
//	float m_gFresnelHardness{ 2.0f };
//	XMFLOAT4 m_gColorFresnel{ 1, 1, 1, 1 };
};

