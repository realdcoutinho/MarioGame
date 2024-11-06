#include "stdafx.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(XM_PIDIV4),
	m_Size(25.0f),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}


void CameraComponent::Initialize(const SceneContext& sceneContext)
{
	m_SceneContext = sceneContext;
}

void CameraComponent::Update(const SceneContext& sceneContext)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	XMMATRIX projection{};

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, sceneContext.aspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size > 0) ? m_Size * sceneContext.aspectRatio : sceneContext.windowWidth;
		const float viewHeight = (m_Size > 0) ? m_Size : sceneContext.windowHeight;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const XMMATRIX view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::SetActive(bool active)
{
	if (m_IsActive == active) return;

	const auto pGameObject = GetGameObject();
	ASSERT_IF(!pGameObject, L"Failed to set active camera. Parent game object is null");

	if (!pGameObject) return; //help the compiler... (C6011)
	const auto pScene = pGameObject->GetScene();
	ASSERT_IF(!pScene, L"Failed to set active camera. Parent game scene is null");

	m_IsActive = active;
	pScene->SetActiveCamera(active?this:nullptr); //Switch to default camera if active==false
}

GameObject* CameraComponent::Pick(CollisionGroup ignoreGroups, const SceneContext& sceneContext) const
{

	//Convert Mouse into NDC
	if (InputManager::IsMouseButton(InputState::pressed, VK_RBUTTON))
	{
		const POINT& mousePos = sceneContext.pInput->GetMousePosition();
		float x = static_cast<float>(mousePos.x);
		float y = static_cast<float>(mousePos.y);

		float halfHeight = sceneContext.windowHeight / 2;
		float halfWidth = sceneContext.windowWidth / 2;

		float xNdc = (x - halfWidth) / halfWidth;
		float yNdc = (halfHeight - y) / halfHeight;

		XMFLOAT4 nearPointXMFLOAT4{ xNdc, yNdc, 0.0f, 0.0f };
		DirectX::XMVECTOR nearPointXMVECTOR = DirectX::XMLoadFloat4(&nearPointXMFLOAT4);
		XMFLOAT4 farPointXMFLOAT4{ xNdc, yNdc, 1, 0 };
		DirectX::XMVECTOR farPointXMVECTOR = DirectX::XMLoadFloat4(&farPointXMFLOAT4);

		//InverseProjectionMatrix
		DirectX::XMMATRIX invertedViewProjectionMatrix = DirectX::XMLoadFloat4x4(&GetViewProjectionInverse());

		//multiply both point by the inverse of the view projection matrix to get the world space points
		nearPointXMVECTOR = XMVector3TransformCoord(nearPointXMVECTOR, invertedViewProjectionMatrix);
		farPointXMVECTOR = XMVector3TransformCoord(farPointXMVECTOR, invertedViewProjectionMatrix);
		//XMVector3TransformCoord and XMVector4Transform
		//they differ in how they handle the fourth component (w) of the input vector.
		//XMVector3TransformCoord assumes that the input vector represents a point in 3D space, 
		//and sets the w component to 1 before performing the transformation.After the transformation, 
		//it divides the resulting vector by its w component, which projects the transformed point 
		//back onto the 3D plane.This is useful when you want to transform a point in 3D space, such 
		//as the position of a vertex.
		//XMVector4Transform, on the other hand, assumes that the input vector represents a 4D vector
		//in homogeneous coordinates, where the w component is typically set to 1 for points and 0 for 
		//vectors. It performs the transformation using all four components of the input vector, and does not
		//divide the result by the w component.This is useful when you want to transform a direction 
		//or a normal vector, since the w component is not used in these calculations.

		//ray start
		DirectX::XMFLOAT3 rayStartXMFLOAT3{};
		DirectX::XMStoreFloat3(&rayStartXMFLOAT3, nearPointXMVECTOR);

		//ray direction
		DirectX::XMFLOAT3 rayDirXMFLOAT3{};
		DirectX::XMStoreFloat3(&rayDirXMFLOAT3, DirectX::XMVectorSubtract(farPointXMVECTOR, nearPointXMVECTOR));


		physx::PxVec3 rayStart{ rayStartXMFLOAT3.x, rayStartXMFLOAT3.y, rayStartXMFLOAT3.z };
		physx::PxVec3 rayDir{ rayDirXMFLOAT3.x, rayDirXMFLOAT3.y, rayDirXMFLOAT3.z };

		//dONT FORGET TO NORMALIZE DIRECTION
		rayDir.normalize();


		PxQueryFilterData filterData{};
		filterData.data.word0 = ~UINT(ignoreGroups);
		//In the code snippet you provided, the ~operator is used 
		//to perform bitwise complement or negation of the UINT value ignoreGroups.

		//The ~operator is a bitwise operator that flips the bits of a binary number, 
		//so every 0 becomes a 1 and every 1 becomes a 0. When used with an unsigned 
		//integer type like UINT, the result is a new UINT value where all the bits 
		//of the original value are inverted.

		//In this case, the ~operator is used to create a bit mask where every bit except
		//those set in the ignoreGroups variable is set to 1. This is achieved by inverting 
		//the bits of ignoreGroups using the ~operator,and then assigning the result 
		//to the word0 field of a PxQueryFilterData struct.The word0 field is used 
		//by the PhysX engine to store collision filtering information, so setting its 
		//bits to 1 indicates that collisions with certain groups should be ignored.

		//For example, if ignoreGroups is 0x00000001, which is binary 
		//00000000 00000000 00000000 00000001, then taking the bitwise complement 
		//with ~ignoreGroups would result in 0xFFFFFFFE, which is binary 
		//11111111 11111111 11111111 11111110. This value could be used as a bit mask to
		//ignore collisions with the collision group represented by the least significant bit of ignoreGroups.

		physx::PxRaycastBuffer hit;

		auto pScene = GetGameObject()->GetScene()->GetPhysxProxy()->GetPhysxScene();

		if (pScene->raycast(rayStart, rayDir, PX_MAX_F32, hit, physx::PxHitFlag::eDEFAULT, filterData))
		{
			return ((RigidBodyComponent*)hit.block.actor->userData)->GetGameObject();
		}

		return nullptr;

	}
	return nullptr;
}