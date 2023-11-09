#pragma once
#include "Object.h"

class OWindow : public Object
{
public:
	OWindow();
	OWindow(const OWindow&)						= default;
	OWindow& operator=(const OWindow&)			= default;
	OWindow(OWindow&&) noexcept					= default;
	OWindow& operator=(OWindow&&) noexcept		= default;
	~OWindow() override;

	static LRESULT WINAPI WindowEventHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);

	EHandleResultType							Initialize() override;
	void										Release() override;
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void										Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void										Tick() override;

	const HWND&									GetHWnd() const;
	uint32										GetClientScreenWidth() const;
	uint32										GetClientScreenHeight() const;
	uint32										GetWindowScreenWidth() const;
	uint32										GetWindowScreenHeight() const;

private:
	/**
	 * \brief Called when WM_SIZE is handled.
	 */
	void										Resize();

	static uint8								bIsScreenSizeChanged;
	uint32										ClientScreenWidth;
	uint32										ClientScreenHeight;
	uint32										WindowScreenWidth;
	uint32										WindowScreenHeight;
	HWND										HWnd;
	WNDCLASSEX									WCEX;
};

