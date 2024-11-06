#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	TODO_W6_();


	float elapsedSeconds = sceneContext.pGameTime->GetElapsed();
	float durationTicks = m_CurrentClip.duration;
	float ticksPerSecond = m_CurrentClip.ticksPerSecond;

	//elapsedSeconds = 0.205f;
	//durationTicks = 400.0f;
	//ticksPerSecond = 2800.0f;


	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1.
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		float passedTicks = elapsedSeconds * ticksPerSecond * m_AnimationSpeed;
		if (passedTicks > durationTicks)
		{
			passedTicks = fmodf(passedTicks, durationTicks);
		}

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}



		//3.
		//Find the enclosing keys
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		AnimationKey keyA, keyB;

		for (size_t i = 0; i < m_CurrentClip.keys.size(); i++) {

			if (m_CurrentClip.keys[i].tick >= m_TickCount) {
				keyB = m_CurrentClip.keys[i];
				keyA = m_CurrentClip.keys[i - 1];
				break;
			}
		}

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		float blendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (int i{ 0 }; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			XMMATRIX transformA = DirectX::XMLoadFloat4x4(&keyA.boneTransforms[i]);
			// 	Retrieve the transform from keyB (transformB)
			XMMATRIX transformB = DirectX::XMLoadFloat4x4(&keyB.boneTransforms[i]);
			//	Decompose both transforms
			XMVECTOR scaleA{};
			XMVECTOR translationA{};
			XMVECTOR rotationQuaternionA{};
			XMMatrixDecompose(&scaleA, &rotationQuaternionA, &translationA, transformA);

			XMVECTOR scaleB{};
			XMVECTOR translationB{};
			XMVECTOR rotationQuaternionB{};
			XMMatrixDecompose(&scaleB, &rotationQuaternionB, &translationB, transformB);
			//	Lerp between all the transformations (Position, Scale, Rotation)
			XMVECTOR lerpScale = DirectX::XMVectorLerp(scaleA, scaleB, blendFactor);
			//XMVECTOR lerpScale{ 1, 1, 1, 1 };
			XMVECTOR lerpRotation = DirectX::XMQuaternionSlerp(rotationQuaternionA, rotationQuaternionB, blendFactor);
			//lerpRotation = { 0, 0, 0, 0 };
			XMVECTOR lerpTranslation = DirectX::XMVectorLerp(translationA, translationB, blendFactor);
			//lerpTranslation = { 0, 0, 0, 0 };
			//	Compose a transformation matrix with the lerp-results
			// Create a rotation matrix from the rotation vector
			XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(lerpRotation);

			// Create a scaling matrix from the scale vector
			XMMATRIX scaleMatrix = XMMatrixScalingFromVector(lerpScale);

			// Create a translation matrix from the translation vector
			XMMATRIX translationMatrix = XMMatrixTranslationFromVector(lerpTranslation);

			// Compose the final matrix by multiplying the three component matrices in the correct order
			XMMATRIX finalMatrix = scaleMatrix * rotationMatrix * translationMatrix;
			//	Add the resulting matrix to the m_Transforms vector

			DirectX::XMFLOAT4X4 composeTransform{};
			DirectX::XMStoreFloat4x4(&composeTransform, finalMatrix);
			//	Add the resulting matrix to the m_Transforms vector
			m_Transforms.push_back(composeTransform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//TODO_W6_()
		//Set m_ClipSet to false
		m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)

	// Traverse through the vector and print each element
	std::vector<AnimationClip> animationClips = m_pMeshFilter->GetAnimationClips();
	/*std::vector<AnimationClip>::iterator it;*/



	for (size_t i{ 0 }; i < animationClips.size(); ++i)
	{
		if(animationClips[i].name == clipName)
		{
			//If found,
			//	Call SetAnimation(Animation Clip) with the found clip
			SetAnimation(animationClips[i]);
		}
		else
		{
			//Else
			//	Call Reset
			//	Log a warning with an appropriate message
			Reset();
			Logger::LogWarning(L"Animation with the folllwing name not found: ", clipName);
		}
	}

	//for (it = animationClips.begin(); it != animationClips.end(); it++)
	//{
	//	AnimationClip currentAnim = *it;
	//
	//	if (currentAnim.name == clipName)
	//	{
	//		//If found,
	//		//	Call SetAnimation(Animation Clip) with the found clip
	//		SetAnimation(*it);
	//	}
	//	else
	//	{
	//		//Else
	//		//	Call Reset
	//		//	Log a warning with an appropriate message
	//		Reset();
	//		Logger::LogWarning(L"Animation with the folllwing name not found: ", clipName);
	//	}
	//}

}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//TODO_W6_()
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	std::vector<AnimationClip> animationClips = m_pMeshFilter->GetAnimationClips();
	if (clipNumber > animationClips.size())
	{
		//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
		Reset();
		Logger::LogWarning(L"Animation with the folllwing clipNumber not found: ", clipNumber);
		return;
	}
	else
	{
		//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
		SetAnimation(animationClips[clipNumber]);
	}


}

UINT ModelAnimator::GetClipById() const
{
	std::vector<AnimationClip> animationClips = m_pMeshFilter->GetAnimationClips();
	for (UINT id{ 0 }; 0 < animationClips.size(); ++id)
	{
		if (animationClips[id].name == m_CurrentClip.name)
		{
			return id;
		}
	}
	return 0;
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//TODO_W6_()
	//Set m_ClipSet to true
	m_ClipSet = true;

	//Set m_CurrentClip
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//TODO_W6_()
	//If pause is true, set m_IsPlaying to false
	m_IsPlaying = true;
	if (pause)
	{
			m_IsPlaying = false;
	}
	
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		std::vector<XMFLOAT4X4> boneTransform = m_CurrentClip.keys[0].boneTransforms;
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(boneTransform.begin(), boneTransform.end());
	}
	else 	//Else
	{
		//	Create an IdentityMatrix 
		XMMATRIX identityMatrix = XMMatrixIdentity();
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		XMFLOAT4X4 identityMatrix4x4;
		XMStoreFloat4x4(&identityMatrix4x4, identityMatrix);
		for (int i{ 0 }; i < m_Transforms.size(); ++i)
		{
			m_Transforms[i] = identityMatrix4x4;
		}
	}
}
