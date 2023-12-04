#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"
#include "OWindow.h"
#include "OInput.h"

uint8				OInput::Keys[256];
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

void OInput::Initialize()
{
	Object::Initialize();

	HRESULT			Result;
	HWND			HWnd;
	HINSTANCE		HInstance;

	HWnd = SApplication::GetWindow()->GetHWnd();
	HInstance = SApplication::GetHInstance();

	// Create IDirectInput8.
	Result = DirectInput8Create(HInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(DirectInput.GetAddressOf()), nullptr);
	if (FAILED(Result))
	{
		throw std::exception("DirectInput8Create() is failed.");
	}

	// Initialize IDirectInputDevice8(keyboard).
	{
		// Create device.
		Result = DirectInput->CreateDevice(GUID_SysKeyboard, KeyboardInputDevice.GetAddressOf(), nullptr);
		if (FAILED(Result))
		{
			throw std::exception("DirectInput->CreateDevice() is failed.");
		}

		// Set pre-defined format.
		Result = KeyboardInputDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(Result))
		{
			throw std::exception("KeyboardInputDevice->SetDataFormat() is failed.");
		}

		// Set the coop level.
		Result = KeyboardInputDevice->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(Result))
		{
			throw std::exception("KeyboardInputDevice->SetCooperativeLevel() is failed.");
		}
	}

	// Initialize IDirectInputDevice8(mouse).
	{
		// Create device.
		Result = DirectInput->CreateDevice(GUID_SysMouse, MouseInputDevice.GetAddressOf(), nullptr);
		if (FAILED(Result))
		{
			throw std::exception("DirectInput->CreateDevice() is failed.");
		}

		// Set pre-defined format.
		Result = MouseInputDevice->SetDataFormat(&c_dfDIMouse);
		if (FAILED(Result))
		{
			throw std::exception("MouseInputDevice->SetDataFormat() is failed.");
		}

		// Set the coop level.
		Result = MouseInputDevice->SetCooperativeLevel(HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(Result))
		{
			throw std::exception("MouseInputDevice->SetCooperativeLevel() is failed.");
		}
	}
}

void OInput::Release()
{
	Object::Release();

	KeyboardInputDevice->Unacquire();
	MouseInputDevice->Unacquire();
}

void OInput::Start()
{
	Object::Start();

	HRESULT Result;

	// Get access to keyboard.
	Result = KeyboardInputDevice->Acquire();
	if (FAILED(Result))
	{
		throw std::exception("KeyboardInputDevice->Acquire() is failed.");
	}

	// Get access to mouse.
	Result = MouseInputDevice->Acquire();
	if (FAILED(Result))
	{
		throw std::exception("MouseInputDevice->Acquire() is failed.");
	}
}

void OInput::Tick()
{
	Object::Tick();

	ReadKeyboard();
	ReadMouse();

	UpdateMousePos();
}

void OInput::ReadKeyboard()
{
	HRESULT Result;

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
