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
	Forward = LookAt;
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
	XMMATRIX TranslationMatrix;
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
		LookAtVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // Default lookat vector in object space.
		LookAtVector = XMVector3Transform(LookAtVector, RotationMatrix);
		LookAtVector = XMVectorAdd(GetPositionVector(), LookAtVector);
		XMStoreFloat3(&LookAt, LookAtVector);
	}

	// Update up.
	XMVECTOR UpVector;
	{
		UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Default up vector in object space.
		UpVector = XMVector3Transform(UpVector, RotationMatrix);
		XMStoreFloat3(&Up, UpVector);
	}

	// Update forward.
	// IMPORTANT : This vector is the basis for other dir vectors. Must be defined before other dir vector defines.
	XMVECTOR ForwardVector;
	{
		ForwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // Default forward vector in object space.
		ForwardVector = XMVector3Transform(ForwardVector, RotationMatrix);
		XMStoreFloat3(&Forward, XMVector3Normalize(ForwardVector));
	}

	// Update back.
	XMVECTOR BackVector;
	{
		BackVector = XMVector3Transform(ForwardVector, XMMatrixRotationX(ToRadian(Rotation.x * 2 * -1)));
		BackVector = XMVector3Transform(BackVector, XMMatrixRotationY(ToRadian(180)));
		XMStoreFloat3(&Back, BackVector);
	}

	// Update left.
	XMVECTOR LeftVector;
	{
		LeftVector = XMVector3Transform(ForwardVector, XMMatrixRotationY(ToRadian(270)));
		XMStoreFloat3(&Left, LeftVector);
	}

	// Update right.
	XMVECTOR RightVector;
	{
		RightVector = XMVector3Transform(ForwardVector, XMMatrixRotationY(ToRadian(90)));
		XMStoreFloat3(&Right, RightVector);
	}
}

void CTransform::End()
{
	OComponent::End();
}

void CTransform::Move(XMFLOAT3 InDistance)
{
	Position = XMFLOAT3(Position.x + InDistance.x, 
						Position.y + InDistance.y,
						Position.z + InDistance.z);
}

void CTransform::Rotate(XMFLOAT3 InRotation)
{
	Rotation = XMFLOAT3(Rotation.x + InRotation.x,
						Rotation.y + InRotation.y,
						Rotation.z + InRotation.z);
}