#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"
#include "OWindow.h"
#include "OInput.h"

uint8				OInput::Keys[256];
uint8				OInput::Keydowns[256];
std::queue<uint8>   OInput::History;
DIMOUSESTATE		OInput::MouseState;
int					OInput::MousePosX;
int					OInput::MousePosY;

OInput::OInput()
	: Object()
{
	MouseState = {};
	MousePosX = 0;
	MousePosY = 0;
}

void OInput::MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam)
{
	if (InMsg == WM_KEYDOWN)
	{
		Keydowns[InWParam] = 1;
	}
}

void OInput::Init()
{
	Object::Init();

	HRESULT			Result;
	HWND			HWnd;
	HINSTANCE		HInstance;

	HWnd = SApplication::GetWindow()->GetHWnd();
	HInstance = SApplication::GetHInstance();

	// Create IDirectInput8.
	Result = DirectInput8Create(HInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(DirectInput.GetAddressOf()), nullptr);
	if (FAILED(Result))
	{
		SConsole::LogError(L"DirectInput8Create() is failed.", __FILE__, __LINE__);
	}

	// Initialize IDirectInputDevice8(keyboard).
	{
		// Create device.
		Result = DirectInput->CreateDevice(GUID_SysKeyboard, KeyboardInputDevice.GetAddressOf(), nullptr);
		if (FAILED(Result))
		{
			SConsole::LogError(L"DirectInput->CreateDevice() is failed.", __FILE__, __LINE__);
		}

		// Set pre-defined format.
		Result = KeyboardInputDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(Result))
		{
			SConsole::LogError(L"KeyboardInputDevice->SetDataFormat() is failed.", __FILE__, __LINE__);
		}

		// Set the coop level.
		Result = KeyboardInputDevice->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(Result))
		{
			SConsole::LogError(L"KeyboardInputDevice->SetCooperativeLevel() is failed.", __FILE__, __LINE__);
		}
	}

	// Initialize IDirectInputDevice8(mouse).
	{
		// Create device.
		Result = DirectInput->CreateDevice(GUID_SysMouse, MouseInputDevice.GetAddressOf(), nullptr);
		if (FAILED(Result))
		{
			SConsole::LogError(L"DirectInput->CreateDevice() is failed.", __FILE__, __LINE__);
		}

		// Set pre-defined format.
		Result = MouseInputDevice->SetDataFormat(&c_dfDIMouse);
		if (FAILED(Result))
		{
			SConsole::LogError(L"MouseInputDevice->SetDataFormat() is failed.", __FILE__, __LINE__);
		}

		// Set the coop level.
		Result = MouseInputDevice->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(Result))
		{
			SConsole::LogError(L"MouseInputDevice->SetCooperativeLevel() is failed.", __FILE__, __LINE__);
		}
	}
}

void OInput::Shutdown()
{
	Object::Shutdown();

	KeyboardInputDevice->Unacquire();
	MouseInputDevice->Unacquire();
}

void OInput::Start()
{
	Object::Start();

	HRESULT Result;

	// Get access to keyboard.
	while (true)
	{
		Result = KeyboardInputDevice->Acquire();

		if (FAILED(Result))
		{
			SConsole::LogWarning(L"KeyboardInputDevice->Acquire() is failed... try again.", __FILE__, __LINE__);
			continue;
		}
		else
		if (SUCCEEDED(Result))
		{
			break;
		}
	}

	// Get access to mouse.
	while (true)
	{
		Result = MouseInputDevice->Acquire();

		if (FAILED(Result))
		{
			SConsole::LogWarning(L"MouseInputDevice->Acquire() is failed... try again.", __FILE__, __LINE__);
			continue;
		}
		else
		if (SUCCEEDED(Result))
		{
			break;
		}
	}
}

void OInput::Tick()
{
	Object::Tick();

	ReadKeyboard();
	ReadMouse();

	UpdateMousePos();
}

void OInput::End()
{
	Object::End();

	ClearKeys();
}

void OInput::ReadKeyboard()
{
	HRESULT Result;

	// 토글구현에 사용
	uint8 BeforeKeys[256];
	std::memcpy(BeforeKeys, Keys, sizeof(uint8) * 256);

	// Read the keyboard device.
	Result = KeyboardInputDevice->GetDeviceState(sizeof(Keys), &Keys);
	if (FAILED(Result))
	{
		// If the keyboard lost focus or was not acquired...
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
		{
			KeyboardInputDevice->Acquire();
		}
	}
}

void OInput::ReadMouse()
{
	HRESULT Result;

	// Read the mouse device.
	Result = MouseInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState);
	if (FAILED(Result))
	{
		// If the mouse lost focus or was not acquired...
		if ((Result == DIERR_INPUTLOST) || (Result == DIERR_NOTACQUIRED))
		{
			MouseInputDevice->Acquire();
		}
	}
}

void OInput::UpdateMousePos()
{
	MousePosX += MouseState.lX;
	MousePosY += MouseState.lY;
}

void OInput::ClearKeys()
{
	uint8 EmptyKeys[256] = {0,};

	std::memcpy(Keys, EmptyKeys, sizeof(uint8) * 256);
	std::memcpy(Keydowns, EmptyKeys, sizeof(uint8) * 256);
}
