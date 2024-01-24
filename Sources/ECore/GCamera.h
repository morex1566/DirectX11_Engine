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

	static void CALLBACK							MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);

	void		 								    Initialize() override;
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
	float											GetFOV() const;
	float											GetScreenAspect() const;
	float											GetScreenNear() const;
	float											GetScreenDepth() const;

private:
	/**
	 * \brief Called when window size is changed.1
	 */
	void											Resize();

	void											Move();

	void											Rotate();

	/*
	 * Read only member variables...
	 */

	const OWindow*									Window;

	float											FOV;
	float											ScreenAspect;
	float											ScreenNear;
	float											ScreenDepth;
	float											MoveSpeed;
	float											RotationSpeed;
	XMMATRIX										ViewMatrix;
	XMMATRIX										ProjectionMatrix;
	XMMATRIX										OrthoMatrix;

	static uint8									bIsScreenSizeChanged;
	static uint8									bIsMoveAvailabled;
};

