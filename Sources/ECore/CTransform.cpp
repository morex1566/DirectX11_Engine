#include "PCH.h"
#include "CTransform.h"

CTransform::CTransform(const OGameObject& InOwner)
	: OComponent(InOwner),
		Position(XMFLOAT3(0, 0, 0)),
		Rotation(XMFLOAT3(0, 0, 0)),
		Scale(XMFLOAT3(1, 1, 1)),
		Up(XMFLOAT3(0, 1, 0)),
		LookAt(XMFLOAT3(0, 0, 1)),
		WorldMatrix(XMMatrixIdentity())
{
}

CTransform::~CTransform()
{
}

Object::EHandleResultType CTransform::Initialize()
{
	OComponent::Initialize();

	return EHandleResultType::Success;
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

XMFLOAT3 CTransform::GetPosition() const
{
	return Position;
}

XMVECTOR CTransform::GetPositionVector() const
{
	return XMLoadFloat3(&Position);
}

XMFLOAT3 CTransform::GetRotation() const
{
	return Rotation;
}

XMFLOAT3 CTransform::GetScale() const
{
	return Scale;
}

XMFLOAT3 CTransform::GetUp() const
{
	return Up;
}

XMVECTOR CTransform::GetUpVector() const
{
	return XMLoadFloat3(&Up);
}

XMFLOAT3 CTransform::GetLookAt() const
{
	return LookAt;
}

XMVECTOR CTransform::GetLookAtVector() const
{
	return XMLoadFloat3(&LookAt);
}

XMMATRIX CTransform::GetWorldMatrix() const
{
	return WorldMatrix;
}

void CTransform::SetPosition(XMFLOAT3 InPosition)
{
	Position = InPosition;
}

void CTransform::SetRotation(XMFLOAT3 InRotation)
{
	Rotation = InRotation;
}

void CTransform::SetScale(XMFLOAT3 InScale)
{
	Scale = InScale;
}

void CTransform::SetUp(XMFLOAT3 InUp)
{
	Up = InUp;
}

void CTransform::SetLookAt(XMFLOAT3 InLookAt)
{
	LookAt = InLookAt;
}
