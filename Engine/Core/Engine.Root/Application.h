#pragma once

#include "pch.h"

class Window;
class D3DManager;

class Application
{
public:
	Application(const Application&)					= delete;
	Application(const Application&&)				= delete;
	Application& operator=(const Application&)		= delete;
	~Application() = default;

	static Application& Get();

	bool Initialize(HINSTANCE hInstance_);
	void Start();
	void Update();
	void Shutdown();

	bool IsStarted() const;
	bool IsExited() const;

private:
	Application();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void onUpdate();
	void onRender();
	void onDispose();

private:
	HINSTANCE										_HInstance;
	bool											_isExited;
	bool											_isStarted;

};