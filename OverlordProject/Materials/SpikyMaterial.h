#pragma once
class SpikyMaterial final : public Material<SpikyMaterial>
{
public:
	SpikyMaterial();
	~SpikyMaterial() override = default;

	SpikyMaterial(const SpikyMaterial& other) = delete;
	SpikyMaterial(SpikyMaterial&& other) noexcept = delete;
	SpikyMaterial& operator=(const SpikyMaterial& other) = delete;
	SpikyMaterial& operator=(SpikyMaterial&& other) noexcept = delete;

	//void SetDiffuseTexture(const std::wstring& assetFile);
	//float GetSpikeLenght() const;
	//void SetSpikeLenght(float lenght);
	//void SetDiffuseColor(XMFLOAT4 color);
	//XMFLOAT4 GetDiffuseColor() const;

protected:

	//float m_SpikeLenght{ 0 };
	//XMFLOAT4 m_DiffuseColor{ 1, 1, 1, 1 };

	void InitializeEffectVariables() override {};


//private:
	//TextureData* m_pDiffuseTexture{ nullptr };
};

