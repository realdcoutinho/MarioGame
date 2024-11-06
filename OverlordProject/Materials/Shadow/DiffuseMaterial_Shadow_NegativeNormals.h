

#pragma once
class DiffuseMaterial_Shadow_NegativeNormals final : public Material<DiffuseMaterial_Shadow_NegativeNormals>
{
public:
	DiffuseMaterial_Shadow_NegativeNormals();
	~DiffuseMaterial_Shadow_NegativeNormals() override = default;

	DiffuseMaterial_Shadow_NegativeNormals(const DiffuseMaterial_Shadow_NegativeNormals& other) = delete;
	DiffuseMaterial_Shadow_NegativeNormals(DiffuseMaterial_Shadow_NegativeNormals&& other) noexcept = delete;
	DiffuseMaterial_Shadow_NegativeNormals& operator=(const DiffuseMaterial_Shadow_NegativeNormals& other) = delete;
	DiffuseMaterial_Shadow_NegativeNormals& operator=(DiffuseMaterial_Shadow_NegativeNormals&& other) noexcept = delete;

	void SetDiffuseTexture(const std::wstring& assetFile);
	void FlipLight();

protected:
	void InitializeEffectVariables() override;
	void OnUpdateModelVariables(const SceneContext&, const ModelComponent*) const override;

private:
	TextureData* m_pDiffuseTexture{};
};

