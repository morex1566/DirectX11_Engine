#pragma once
#include "OComponent.h"

class CTransform : public OComponent
{
public:
	CTransform(const OGameObject& InOwner);
	CTransform(const CTransform&)							= default;
	CTransform& operator=(const CTransform&)				= default;
	CTransform(CTransform&&) noexcept						= default;
	CTransform& operator=(CTransform&&) noexcept			= default;
	virtual ~CTransform() override;

	virtual EHandleResultType								Initialize() override;
	virtual void											Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void											Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void											Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void											End() override;

	XMFLOAT3												GetPosition() const;
	XMVECTOR												GetPositionVector() const;
	XMFLOAT3												GetRotation() const;
	XMFLOAT3												GetScale() const;
	XMFLOAT3												GetUp() const;
	XMVECTOR												GetUpVector() const;
	XMFLOAT3												GetLookAt() const;
	XMVECTOR												GetLookAtVector() const;
	XMMATRIX												GetWorldMatrix() const;

	void													SetPosition(XMFLOAT3 InPosition);
	void													SetRotation(XMFLOAT3 InRotation);
	void													SetScale(XMFLOAT3 InScale);
	void													SetUp(XMFLOAT3 InUp);
	void													SetLookAt(XMFLOAT3 InLookAt);

private:
	EPROPERTY()
	XMFLOAT3												Position;
	EPROPERTY()
	XMFLOAT3												Rotation;
	EPROPERTY()
	XMFLOAT3												Scale;
	EPROPERTY()
	XMFLOAT3												Up;
	EPROPERTY()
	XMFLOAT3												LookAt;
	XMMATRIX												WorldMatrix;
};

