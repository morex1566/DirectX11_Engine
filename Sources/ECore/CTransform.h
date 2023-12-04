#pragma once
#include "OComponent.h"

ECLASS()
class CTransform : public OComponent
{
public:
	CTransform(const OGameObject* InOwner);
	CTransform(const CTransform&)						= default;
	CTransform& operator=(const CTransform&)			= default;
	CTransform(CTransform&&) noexcept					= default;
	CTransform& operator=(CTransform&&) noexcept		= default;
	virtual ~CTransform() override;

	virtual void		 								Initialize() override;
	virtual void										Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void										Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void										Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void										End() override;

	void												Move(XMFLOAT3 InPosition);
	void												Rotate(XMFLOAT3 InRotation);

	FORCEINLINE XMFLOAT3								GetPosition() const { return Position; }
	FORCEINLINE XMVECTOR								GetPositionVector() const { return XMLoadFloat3(&Position); }
	FORCEINLINE XMFLOAT3								GetRotation() const { return Rotation; }
	FORCEINLINE XMFLOAT3								GetScale() const { return Scale; }
	FORCEINLINE XMFLOAT3								GetUp() const { return Up; }
	FORCEINLINE XMVECTOR								GetUpVector() const { return XMLoadFloat3(&Up); }
	FORCEINLINE XMFLOAT3								GetLookAt() const { return LookAt; }
	FORCEINLINE XMVECTOR								GetLookAtVector() const { return XMLoadFloat3(&LookAt); }
	FORCEINLINE XMMATRIX								GetWorldMatrix() const { return WorldMatrix; }

	XMFLOAT3											GetLeft() const;
	XMFLOAT3											GetRight() const;
	XMFLOAT3											GetForward() const { return LookAt; }
	XMFLOAT3											GetBack() const;

	FORCEINLINE void									SetPosition(XMFLOAT3 InPosition) { Position = InPosition; }
	FORCEINLINE void									SetRotation(XMFLOAT3 InRotation) { Rotation = InRotation; }
	FORCEINLINE void									SetScale(XMFLOAT3 InScale) { Scale = InScale; }
	FORCEINLINE void									SetUp(XMFLOAT3 InUp) { Up = InUp;}
	FORCEINLINE void									SetLookAt(XMFLOAT3 InLookAt) { LookAt = InLookAt; }

private:
	EPROPERTY()
	XMFLOAT3											Position;
	EPROPERTY()
	XMFLOAT3											Rotation;
	EPROPERTY()
	XMFLOAT3											Scale;
	EPROPERTY()
	XMFLOAT3											Up;
	EPROPERTY()
	XMFLOAT3											LookAt;
	XMMATRIX											WorldMatrix;
};

