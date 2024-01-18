#pragma once
#include "Window.h"

class System
{
public:
	System(HINSTANCE hInstance);
	System(const System&)						= delete;
	System& operator=(const System&)			= delete;
	System(System&&) noexcept					= delete;
	System& operator=(System&&) noexcept		= delete;
	~System()									= default;

public:
	static void WINAPI MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);

	void Init();
	void Start();
	void Update();
	void Shutdown();
	FORCEINLINE bool IsLooping() const { return isLooping; }

private:
	HINSTANCE									hInstance;
	static bool									isLooping;

	std::unique_ptr<Window>						window;
};

