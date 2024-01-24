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
	/**
	 * \brief Move Instance from current position.
	 * \param InDistance : Move displacement.
	 */
	void												Move(XMFLOAT3 InDistance);
	void												Rotate(XMFLOAT3 InRotation);

	FORCEINLINE XMFLOAT3								GetPosition() const { return Position; }
	FORCEINLINE XMVECTOR								GetPositionVector() const { return XMLoadFloat3(&Position); }
	FORCEINLINE XMFLOAT3								GetRotation() const { return Rotation; }
	FORCEINLINE XMFLOAT3								GetScale() const { return Scale; }
	FORCEINLINE XMFLOAT3								GetUp() const { return Up; }
	FORCEINLINE XMVECTOR								GetUpVector() const { return XMLoadFloat3(&Up); }
	FORCEINLINE XMFLOAT3								GetLookAt() const { return LookAt; }
	FORCEINLINE XMVECTOR								GetLookAtVector() const { return XMLoadFloat3(&LookAt); }
	FORCEINLINE XMFLOAT3								GetForward() const { return Forward; }
	FORCEINLINE XMVECTOR								GetForwardVector() const { return XMLoadFloat3(&Forward); }
	FORCEINLINE XMFLOAT3								GetBack() const { return Back; }
	FORCEINLINE XMVECTOR								GetBackVector() const { return XMLoadFloat3(&Back); }
	FORCEINLINE XMFLOAT3								GetLeft() const { return Left; }
	FORCEINLINE XMVECTOR								GetLeftVector() const { return XMLoadFloat3(&Left); }
	FORCEINLINE XMFLOAT3								GetRight() const { return Right; }
	FORCEINLINE XMVECTOR								GetRightVector() const { return XMLoadFloat3(&Right); }
	FORCEINLINE XMMATRIX								GetWorldMatrix() const { return WorldMatrix; }

	FORCEINLINE void									SetPosition(XMFLOAT3 InPosition) { Position = InPosition; }
	FORCEINLINE void									SetRotation(XMFLOAT3 InRotation) { Rotation = InRotation; }
	FORCEINLINE void									SetScale(XMFLOAT3 InScale) { Scale = InScale; }

private:
	XMFLOAT3											Position;
	XMFLOAT3											Rotation;
	XMFLOAT3											Scale;
	XMFLOAT3											Up;
	XMFLOAT3											LookAt;
	XMFLOAT3											Forward;
	XMFLOAT3											Back;
	XMFLOAT3											Left;
	XMFLOAT3											Right;
	XMMATRIX											WorldMatrix;
};