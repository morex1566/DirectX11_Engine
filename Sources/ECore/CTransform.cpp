#include "PCH.h"
#include "CTransform.h"

CTransform::CTransform(const OGameObject* InOwner)
	: OComponent(InOwner)
{
	Position = XMFLOAT3(0, 0, 0);
	Rotation = XMFLOAT3(0, 0, 0);
	Scale = XMFLOAT3(1, 1, 1);
	Up = XMFLOAT3(0, 1, 0);
	LookAt = XMFLOAT3(0, 0, 1);
	WorldMatrix = XMMatrixIdentity();
}

CTransform::~CTransform()
{
}

void CTransform::Initialize()
{
	OComponent::Initialize();
}

void CTransform::Release()
{
	OComponent::Release();
}

void CTransform::Start()
{
	OComponent::Start();
}

void CTransform::Tick()
{
	OComponent::Tick();

	WorldMatrix = XMMatrixIdentity();

	// Update world matrix.
	XMMATRIX TranslationMatrix ;
	XMMATRIX RotationMatrix;
	XMMATRIX ScaleMatrix;
	{
		TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
		RotationMatrix = XMMatrixRotationRollPitchYaw(ToRadian(Rotation.x), ToRadian(Rotation.y), ToRadian(Rotation.z));
		ScaleMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);

		WorldMatrix = XMMatrixMultiply(WorldMatrix, ScaleMatrix);
		WorldMatrix = XMMatrixMultiply(WorldMatrix, RotationMatrix);
		WorldMatrix = XMMatrixMultiply(WorldMatrix, TranslationMatrix);
	}

	// Update lookAt.
	XMVECTOR LookAtVector;
	{
		LookAtVector = XMVector3TransformCoord(GetLookAtVector(), RotationMatrix);
		LookAtVector = XMVectorAdd(GetPositionVector(), LookAtVector);
		XMStoreFloat3(&LookAt, XMVector3Normalize(LookAtVector));
	}

	// Update up.
	XMVECTOR UpVector;
	{
		UpVector = XMVector3TransformCoord(GetUpVector(), RotationMatrix);
		XMStoreFloat3(&Up, XMVector3Normalize(UpVector));
	}
}

void CTransform::End()
{
	OComponent::End();
}