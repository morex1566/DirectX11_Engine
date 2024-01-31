#pragma once
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include "Object.h"

/**
 * \brief Manage keyboard, mouse input and setting.
 * \remark OInput class must be initialize and start after OWindow class.
 */
class OInput : public Object
{
public:
	enum class EMouseButton
	{
		Left,
		Right,
		Wheel
	};

public:
	OInput();
	OInput(const OInput&)						= default;
	OInput& operator=(const OInput&)			= default;
	OInput(OInput&&) noexcept					= default;
	OInput& operator=(OInput&&) noexcept		= default;
	~OInput() override							= default;


public:
	static void CALLBACK			MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);


public:
	void							Init() override;
	void							Shutdown() override;
	void							Start() override;
	void							Tick() override;
	void							End() override;

	FORCEINLINE	static bool			GetKey(uint8 InDIKCode) { return Keys[InDIKCode] & 0x80 ? true : false; }
	FORCEINLINE static bool			GetKeyDown(uint8 InVKCode) { return Keydowns[InVKCode] ? true : false; }
	FORCEINLINE static bool			GetMouseButtonDown(EMouseButton InMouseButtonType) { return MouseState.rgbButtons[static_cast<int>(InMouseButtonType)] & 0x80 ? true : false; }
	FORCEINLINE static int			GetMousePosX() { return MousePosX; }
	FORCEINLINE static int			GetMousePosY() { return MousePosY; }
	FORCEINLINE static int			GetMouseAxisX() { return MouseState.lX; }
	FORCEINLINE static int			GetMouseAxisY() { return MouseState.lY; }

private:
	void							ReadKeyboard();
	void							ReadMouse();
	void							UpdateMousePos();
	void							ClearKeys();

	/**
	 * \brief '1' means key down, '0' means key up. (누름)
	 */
	static uint8								Keys[256];
	/**
	 * \brief '1' means key down, '0' means key up. (토글)
	 */
	static uint8								Keydowns[256];

	/**
	 * \brief History of pressed key at frame.
	 */
	static std::queue<uint8>					History;
	static DIMOUSESTATE							MouseState;
	ComPtr<IDirectInput8>						DirectInput;
	ComPtr<IDirectInputDevice8>					KeyboardInputDevice;
	ComPtr<IDirectInputDevice8>					MouseInputDevice;
	static int									MousePosX;
	static int									MousePosY;
};

