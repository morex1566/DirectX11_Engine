#pragma once
#include "OGameObject.h"

class OWindow;

class GCamera : public OGameObject
{
public:
	GCamera(const OWindow& InWindow);
	GCamera(const GCamera&)							= default;
	GCamera& operator=(const GCamera&)				= default;
	GCamera(GCamera&&) noexcept						= default;
	GCamera& operator=(GCamera&&) noexcept			= default;
	~GCamera() override;

	static EHandleResultType CALLBACK				MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);

	EHandleResultType								Initialize() override;
	void											Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	void											Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void											Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void											End() override;

	XMMATRIX										GetViewMatrix() const;
	XMMATRIX										GetProjectionMatrix() const;
	XMMATRIX										GetOrthoMatrix() const;
	XMFLOAT3										GetPosition() const;
	XMFLOAT3										GetRotation() const;
	float											GetFOV() const;
	float											GetScreenAspect() const;
	float											GetScreenNear() const;
	float											GetScreenDepth() const;


	void											SetPosition(XMFLOAT3 InPosition);

private:
	/**
	 * \brief Called when window size is changed.
	 */
	void											Resize();

	/*
	 * Read only member variables...
	 */

	const OWindow*									Window;

	XMFLOAT3										Position;
	XMFLOAT3										Rotation;
	float											FOV;
	float											ScreenAspect;
	float											ScreenNear;
	float											ScreenDepth;
	XMMATRIX										ViewMatrix;
	XMMATRIX										ProjectionMatrix;
	XMMATRIX										OrthoMatrix;

	static uint8									bIsScreenSizeChanged;
};

