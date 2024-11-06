#include "stdafx.h"
#include "UberMaterial.h"

#include <iostream>
#include <map>
#include <string>
#include <any>

UberMaterial::UberMaterial() :
	Material<UberMaterial>(L"Effects/GP2_W3X-2_UberShader_START.fx")
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeDiffuseVariables();
}

void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeNormalVariables();
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeSpecularVariables();
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeOpacityVaribles();
}

void UberMaterial::SetEnviornmentTexture(const std::wstring& assetFile)
{
	m_pEnviornmentTexture = ContentManager::Load<TextureData>(assetFile);
	InitializeEnviormentVariables();
}

void UberMaterial::InitializeDiffuseVariables()
{
	if (m_pDiffuseTexture)
	{
		SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture->GetShaderResourceView());
		/*SetVariable_Scalar(L"gUseTextureDiffuse", m_gUseTextureDiffuse);
		SetVariable_Vector(L"gColorDiffuse", m_DiffuseColor);*/
	}
}

void UberMaterial::InitializeNormalVariables()
{
	if (m_pNormalTexture)
	{
		SetVariable_Texture(L"gTextureNormal", m_pNormalTexture->GetShaderResourceView());
		/*SetVariable_Scalar(L"gUseTextureNormal", m_gUseTextureNormal);*/
	}
}

void UberMaterial::InitializeSpecularVariables()
{
	if (m_pSpecularTexture)
	{
		SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularTexture->GetShaderResourceView());
		//SetVariable_Scalar(L"gUseTextureSpecularIntensity", m_gUseTextureSpecularIntensity);
		//SetVariable_Scalar(L"gUseSpecularPhong", m_gUseSpecularPhong);
		//SetVariable_Scalar(L"gUseSpecularBlinn", m_gUseSpecularBlinn);
		//SetVariable_Vector(L"gColorSpecular", m_SpecularColor);
	}
}

void UberMaterial::InitializeOpacityVaribles()
{
	if (m_pOpacityTexture)
	{
		SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture->GetShaderResourceView());
		//SetVariable_Scalar(L"gTextureOpacityIntensity", true);
	}
}

void UberMaterial::InitializeEnviormentVariables()
{

	if (m_pEnviornmentTexture)
	{
		SetVariable_Texture(L"gCubeEnvironment", m_pEnviornmentTexture->GetShaderResourceView());
		//SetVariable_Scalar(L"gUseEnvironmentMapping", m_gUseEnvironmentMapping);
		//SetVariable_Scalar(L"gReflectionStrength", m_gReflectionStrength);
		//SetVariable_Scalar(L"gRefractionStrength", m_gRefractionStrength);
		//SetVariable_Scalar(L"gRefractionIndex", m_gRefractionIndex);

		//SetVariable_Scalar(L"gUseFresnelFalloff", m_gUseFresnelFalloff);
		//SetVariable_Scalar(L"gFresnelPower", m_gFresnelPower);
		//SetVariable_Scalar(L"gFresnelMultiplier", m_gFresnelMultiplier);
		//SetVariable_Scalar(L"gFresnelHardness", m_gFresnelHardness);
		//SetVariable_Vector(L"gColorFresnel", m_gColorFresnel);
	}
}

void UberMaterial::InitializeEffectVariables()
{
	////DIFFUSE
	//if(m_pDiffuseTexture)
	//{
	//	SetVariable_Scalar(L"gUseTextureDiffuse", std::any_cast<bool>(m_VariablesMap["gUseTextureDiffuse"]));
	//	SetVariable_Vector(L"gColorDiffuse", std::any_cast<XMFLOAT4>(m_VariablesMap["gColorDiffuse"]));
	//}

	////NORMAL
	//if (m_pNormalTexture)
	//{
	//	SetVariable_Scalar(L"gUseTextureNormal", std::any_cast<bool>(m_VariablesMap["gUseTextureNormal"]));
	//}

	////////Specular
	//if (m_pSpecularTexture)
	//{
	//	SetVariable_Scalar(L"gUseTextureSpecularIntensity", std::any_cast<bool>(m_VariablesMap["gUseTextureSpecularIntensity"]));
	//	SetVariable_Scalar(L"gUseSpecularPhong", std::any_cast<bool>(m_VariablesMap["gUseSpecularPhong"]));
	//	SetVariable_Scalar(L"gUseSpecularBlinn", std::any_cast<bool>(m_VariablesMap["gUseSpecularBlinn"]));
	//	SetVariable_Vector(L"gColorSpecular", std::any_cast<XMFLOAT4>(m_VariablesMap["gColorSpecular"]));
	//}

	////Opacity
	//SetVariable_Scalar(L"gTextureOpacityIntensity", std::any_cast<bool>(m_VariablesMap["gTextureOpacityIntensity"]));

	////Enviornment
	//SetVariable_Scalar(L"gUseEnvironmentMapping", std::any_cast<bool>(m_VariablesMap["gUseEnvironmentMapping"]));
	//SetVariable_Scalar(L"gReflectionStrength", std::any_cast<float>(m_VariablesMap["gReflectionStrength"]));
	//SetVariable_Scalar(L"gRefractionStrength", std::any_cast<float>(m_VariablesMap["gRefractionStrength"]));
	//SetVariable_Scalar(L"gRefractionIndex", std::any_cast<float>(m_VariablesMap["gRefractionIndex"]));

	////Fresnel
	//SetVariable_Scalar(L"gUseFresnelFalloff", std::any_cast<bool>(m_VariablesMap["gUseFresnelFalloff"]));
	//SetVariable_Scalar(L"gFresnelPower", std::any_cast<float>(m_VariablesMap["gFresnelPower"]));
	//SetVariable_Scalar(L"gFresnelMultiplier", std::any_cast<float>(m_VariablesMap["gFresnelMultiplier"]));
	//SetVariable_Scalar(L"gFresnelHardness", std::any_cast<float>(m_VariablesMap["gFresnelHardness"]));
	//SetVariable_Vector(L"gColorFresnel", std::any_cast<XMFLOAT4>(m_VariablesMap["gColorFresnel"]));

}
//
//void UberMaterial::UpdateEffectVariables()
//{
//	if (m_pDiffuseTexture)
//	{
//		SetVariable_Scalar(L"gUseTextureDiffuse", std::any_cast<bool>(m_VariablesMap["gUseTextureDiffuse"]));
//		SetVariable_Vector(L"gColorDiffuse", std::any_cast<XMFLOAT4>(m_VariablesMap["gColorDiffuse"]));
//	}
//	if (m_pNormalTexture)
//	{
//		SetVariable_Scalar(L"gUseTextureNormal", std::any_cast<bool>(m_VariablesMap["gUseTextureNormal"]));
//	}
//}

//
//void UberMaterial::CreateMap()
//{
//	////DIFFUSE
//	//m_VariablesMap.insert({ "gUseTextureDiffuse", m_gUseTextureDiffuse });
//	//m_VariablesMap.insert({ "gUseTextureNormal", m_gUseTextureNormal });
//
//	//m_VariablesMap.insert({ "gColorDiffuse", m_DiffuseColor });
//
//	////Normal
//	//m_VariablesMap.insert({ "gUseTextureNormal", m_gUseTextureNormal });
//
//	//////Specular
//	//m_VariablesMap.insert({ "gUseTextureSpecularIntensity", m_gUseTextureSpecularIntensity });
//	//m_VariablesMap.insert({ "gUseSpecularPhong", m_gUseSpecularPhong });
//	//m_VariablesMap.insert({ "gUseSpecularBlinn", m_gUseSpecularBlinn });
//	//m_VariablesMap.insert({ "gColorSpecular", m_SpecularColor });
//
//	////Opacity
//	//m_VariablesMap.insert({ "gTextureOpacityIntensity", m_gTextureOpacityIntensity });
//
//	////Enviornment
//	//m_VariablesMap.insert({ "gUseEnvironmentMapping", m_gUseEnvironmentMapping });
//	//m_VariablesMap.insert({ "gReflectionStrength", m_gReflectionStrength });
//	//m_VariablesMap.insert({ "gRefractionStrength", m_gRefractionStrength });
//	//m_VariablesMap.insert({ "gRefractionIndex", m_gRefractionIndex });
//
//	////Fresnel
//	//m_VariablesMap.insert({ "gUseFresnelFalloff", m_gUseFresnelFalloff });
//	//m_VariablesMap.insert({ "gFresnelPower", m_gFresnelPower });
//	//m_VariablesMap.insert({ "gFresnelMultiplier", m_gFresnelMultiplier });
//	//m_VariablesMap.insert({ "gFresnelHardness", m_gFresnelHardness });
//	//m_VariablesMap.insert({ "gColorFresnel", m_gColorFresnel });
//
//}
//
//std::any UberMaterial::GetValueFromMap(const std::string string) const
//{
//	auto it = m_VariablesMap.find(string);
//	if (it != m_VariablesMap.end()) {  // if the key is found
//		return it->second;     // return the corresponding value
//	}
//	else {
//		throw std::runtime_error("Key not found in map"); // throw an exception if the key is not found
//	}
//}
//
//void UberMaterial::SetValueFromMap(const std::string string, std::any value)
//{
//	auto it = m_VariablesMap.find(string);
//	if (it != m_VariablesMap.end()) {  // if the key is found
//		it->second = value;     // return the corresponding value
//	}
//	else {
//		throw std::runtime_error("Key not found in map"); // throw an exception if the key is not found
//	}
//}


