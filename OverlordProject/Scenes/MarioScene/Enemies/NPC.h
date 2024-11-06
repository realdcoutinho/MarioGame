#pragma once

class NPC : public GameObject
{
public:
	NPC();
	~NPC() override = default;

	NPC(const NPC& other) = delete;
	NPC(NPC&& other) noexcept = delete;
	NPC& operator=(const NPC& other) = delete;
	NPC& operator=(NPC&& other) noexcept = delete;


protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;
	void Movement();
	void Killed();

	std::wstring m_ModelComponentPath{}; //Model Component
	std::wstring m_ConvexMeshPath{}; //ConvexMesh
	std::wstring m_TriangleMeshPath{}; //TriangleMesh
	std::wstring m_TexturePath{};

	XMFLOAT3 m_StartPos{};
	std::vector<XMFLOAT3> m_StartPosVector;

private:




//	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
//	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
//		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
//		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)
//
//	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
//	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)
};

