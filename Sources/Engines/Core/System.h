#pragma once
#include "Window.h"
#include "DirectX11.h"
#include "GameObject.h"

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
	static void Quit();


public:
	void Init();
	void Start();
	void Update();
	void Shutdown();
	FORCEINLINE bool GetIsLooping() const { return isLooping; }


private:
	static bool									isLooping;


private:
	HINSTANCE									hInstance;
	std::unique_ptr<Window>						window;
	std::unique_ptr<DirectX11>					directX11;
	std::vector<GameObject*>					gameObjects;


};

