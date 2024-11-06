#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount, XMFLOAT3 location):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings),
	m_Location(location)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	SafeDelete(m_ParticlesArray);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	if (!m_pParticleMaterial)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}

	CreateVertexBuffer(sceneContext);

	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	if(m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer)


	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	HRESULT result{ sceneContext.d3dContext.pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pVertexBuffer) };
	if (FAILED(result))
	{
		HANDLE_ERROR(L"Failed to create vertexbuffer, ParticleEmitterComponent");
	}
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	//if (m_EmitterSettings.isRotating)
	//{
	//	if (m_EmitterSettings.isRotating)
	//		m_Angle += sceneContext.pGameTime->GetElapsed() * m_EmitterSettings.rotationSpeed;
	//	XMMATRIX rotationMatrix = XMMatrixRotationY(m_Angle);
	//
	//	XMMATRIX translationMatrix = XMMatrixTranslation(-m_Location.x, -m_Location.y, -m_Location.z);
	//	XMMATRIX worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());
	//
	//	worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
	//	worldMatrix = XMMatrixMultiply(worldMatrix, rotationMatrix);
	//
	//	translationMatrix = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
	//	worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
	//
	//	XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);
	//}

	if (m_EmitterSettings.rotationAxis.x > 0 || m_EmitterSettings.rotationAxis.y > 0 || m_EmitterSettings.rotationAxis.z > 0)
	{
		m_Angle += sceneContext.pGameTime->GetElapsed() * m_EmitterSettings.rotationSpeed;
		XMMATRIX rotationMatrix;
		if (m_EmitterSettings.rotationAxis.x > 0 && m_EmitterSettings.rotationAxis.y == 0 && m_EmitterSettings.rotationAxis.z == 0)
		{
			rotationMatrix = XMMatrixRotationX(m_Angle);
		}
		if (m_EmitterSettings.rotationAxis.x == 0 && m_EmitterSettings.rotationAxis.y > 0 && m_EmitterSettings.rotationAxis.z == 0)
		{
			rotationMatrix = XMMatrixRotationY(m_Angle);
		}
		if (m_EmitterSettings.rotationAxis.x == 0 && m_EmitterSettings.rotationAxis.y == 0 && m_EmitterSettings.rotationAxis.z > 0)
		{
			rotationMatrix = XMMatrixRotationZ(m_Angle);
		}

		XMMATRIX translationMatrix = XMMatrixTranslation(-m_Location.x, -m_Location.y, -m_Location.z);
		XMMATRIX worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());

		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, rotationMatrix);

		translationMatrix = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
		worldMatrix = XMMatrixMultiply(worldMatrix, translationMatrix);

		XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);


		//XMMATRIX rotationMatrix = XMMatrixRotationY(m_Angle);

		//// Define the distance 'x' from the rotation point
		//float distance = 5.0f; // Adjust this value as needed

		//// Create a translation matrix to move away from the rotation point
		//XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, distance);

		//// Create a translation matrix to move back to the rotation point
		//XMMATRIX inverseTranslationMatrix = XMMatrixTranslation(0.0f, 0.0f, -distance);

		//// Combine the matrices
		//XMMATRIX finalMatrix = translationMatrix * rotationMatrix * inverseTranslationMatrix;

		//// Apply the transformation to the object's world matrix
		//XMMATRIX worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());
		//worldMatrix = XMMatrixMultiply(worldMatrix, finalMatrix);
		//XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);


		//XMMATRIX rotationMatrix = XMMatrixRotationY(m_Angle);

		//// Define the distance 'x' from the rotation point
		//float distance = 1000.0f; // Adjust this value as needed

		//// Create a translation matrix to move to the rotation point
		////XMMATRIX translationToRotationPointMatrix = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);

		//// Create a translation matrix to move away from the rotation point by a distance 'x'
		//XMMATRIX translationAwayFromRotationPointMatrix = XMMatrixTranslation(distance, 0.0f, distance);

		//// Create a translation matrix to move back to the rotation point
		////XMMATRIX translationToRotationPointMatrixInverse = XMMatrixTranslation(-m_Location.x, -m_Location.y, -m_Location.z);

		//// Combine the matrices
		//XMMATRIX finalMatrix = /*translationToRotationPointMatrix * */ translationAwayFromRotationPointMatrix * rotationMatrix /** translationToRotationPointMatrixInverse*/;

		//// Apply the transformation to the object's world matrix
		//XMMATRIX worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());
		//worldMatrix = XMMatrixMultiply(worldMatrix, finalMatrix);
		//XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);




		//XMMATRIX rotationMatrix = XMMatrixRotationY(m_Angle);

		//// Define the distance 'x' from the rotation point
		//float distance = 1000.0f; // Adjust this value as needed

		//// Create a translation matrix to move to the rotation point
		//XMMATRIX trasnlateToCenter = XMMatrixTranslation(-m_Location.x, -m_Location.y, -m_Location.z);

		//// Create a translation matrix to move away from the rotation point by a distance 'x'
		//XMMATRIX translateToPoint = XMMatrixTranslation(distance, 0.0f, 0.0f);

		//XMMATRIX trasnlateToAwayFromCenter = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);

		//XMMATRIX matrix1 = trasnlateToCenter * translateToPoint * trasnlateToAwayFromCenter;

		//// Create a translation matrix to move back to the rotation point
		////XMMATRIX translationToRotationPointMatrixInverse = XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);

		//// Combine the matrices
		//XMMATRIX finalMatrix = matrix1/* * translationToRotationPointMatrixInverse*/;

		//// Apply the transformation to the object's world matrix
		//XMMATRIX worldMatrix = XMLoadFloat4x4(&GetTransform()->GetWorld());
		//worldMatrix = XMMatrixMultiply(worldMatrix, finalMatrix);
		//XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);




	}






	//float particleInterval = (m_EmitterSettings.maxEnergy - m_EmitterSettings.minEnergy) * m_MaxParticles;

	const float averageEnergy{ (m_EmitterSettings.minEnergy + m_EmitterSettings.maxEnergy) * 0.5f };
	const float particleInterval{ averageEnergy / m_ParticleCount };

	//float elapsedTime = sceneContext.pGameTime->GetElapsed();
	//m_LastParticleSpawn += elapsedTime;

	const float elapsedSec{ sceneContext.pGameTime->GetElapsed() };
	m_LastParticleSpawn += elapsedSec;

	m_ActiveParticles = 0;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource{};
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);

	VertexParticle* pVertexParticleBuffer{ reinterpret_cast<VertexParticle*>(mappedSubResource.pData) };


	for (UINT index{}; index < m_ParticleCount; ++index)
	{
		Particle& currParticle{ m_ParticlesArray[index] };

		if (currParticle.isActive)
		{
			UpdateParticle(currParticle, elapsedSec);
		}

		if (!currParticle.isActive && m_LastParticleSpawn >= particleInterval)
		{
			SpawnParticle(currParticle);
			m_LastParticleSpawn -= particleInterval;
		}

		if (currParticle.isActive)
		{
			pVertexParticleBuffer[m_ActiveParticles] = currParticle.vertexInfo;
			++m_ActiveParticles;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);

}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	if (!p.isActive) return;

	//Update energy
	p.currentEnergy -= elapsedTime;

	if (p.currentEnergy < 0)
	{
		p.isActive = false;
		return;
	}

	//Update position
	p.vertexInfo.Position.x += m_EmitterSettings.velocity.x * elapsedTime;
	p.vertexInfo.Position.y += m_EmitterSettings.velocity.y * elapsedTime;
	p.vertexInfo.Position.z += m_EmitterSettings.velocity.z * elapsedTime;

	//Update color
	const float lifePercent{ p.currentEnergy / p.totalEnergy };
	constexpr float fadeDelay{ 2.f };

	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w *= lifePercent * fadeDelay;

	//Update size
	const float initialSize{ p.initialSize };
	p.vertexInfo.Size = initialSize + initialSize * (p.sizeChange - 1.f) * (1.f - lifePercent);
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;

	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	XMFLOAT3 unitVector = { 1,0,0 };
	auto randomMatrix = XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	auto randomDirection = XMVector3TransformNormal(XMLoadFloat3(&unitVector), randomMatrix);

	float randomDistance  = MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.minEmitterRadius);

	XMStoreFloat3(&p.vertexInfo.Position, XMLoadFloat3(&GetTransform()->GetWorldPosition()) + (randomDirection * randomDistance));

	p.vertexInfo.Size = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.initialSize = p.vertexInfo.Size;
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture->GetShaderResourceView());
	m_pParticleMaterial->SetVariable_Scalar(L"gIsFacingCamera", m_EmitterSettings.isFacingCamera);
	m_pParticleMaterial->SetVariable_Scalar(L"gIsRotateUV", m_EmitterSettings.isRotateUV);
	m_pParticleMaterial->SetVariable_Scalar(L"gIsRotating", m_EmitterSettings.isRotating);
	//if (m_EmitterSettings.isRotating)
	//{
		m_pParticleMaterial->SetVariable_Matrix(L"gWorld", m_WorldMatrix);
	//}


	const auto& techniqueContext{ m_pParticleMaterial->GetTechniqueContext() };
	ID3D11DeviceContext* pDeviceContext{ sceneContext.d3dContext.pDeviceContext };

	pDeviceContext->IASetInputLayout(techniqueContext.pInputLayout);

	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	constexpr UINT offset{}, vertexSize{ sizeof(VertexParticle) };
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &vertexSize, &offset);

	D3DX11_TECHNIQUE_DESC techniqueDesc{};
	techniqueContext.pTechnique->GetDesc(&techniqueDesc);

	for (uint32_t pass{}; pass < techniqueDesc.Passes; ++pass)
	{
		techniqueContext.pTechnique->GetPassByIndex(pass)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}