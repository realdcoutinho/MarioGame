#include "stdafx.h"
#include "SpikyMaterial.h"

SpikyMaterial::SpikyMaterial() :
	Material<SpikyMaterial>(L"Effects/SpikyShader.fx")
{
	//InitializeEffectVariables();
}

//void SpikyMaterial::InitializeEffectVariables()
//{
//	SetVariable_Scalar(L"gSpikeLength", 0.1f);
//}
//
//void SpikyMaterial::SetSpikeLenght(float lenght)
//{
//	m_SpikeLenght = lenght;
//	SetVariable_Scalar(L"gSpikeLength", lenght);
//}
//
//float SpikyMaterial::GetSpikeLenght() const
//{
//	return m_SpikeLenght;
//}
//
//void SpikyMaterial::SetDiffuseColor(XMFLOAT4 color)
//{
//	m_DiffuseColor = color;
//	SetVariable_Vector(L"gColorDiffuse", color);
//}
//
//XMFLOAT4 SpikyMaterial::GetDiffuseColor() const
//{
//	return m_DiffuseColor;
//}