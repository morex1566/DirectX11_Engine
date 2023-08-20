#pragma once

class Window : public IManageable
{
public:
	Window(const HINSTANCE& hInstance_, const std::string& serialName_, const std::string& titleName_, WinProc* winProc_,
		   int posX_, int posY_, int width_, int height_);
	Window() = default;
	~Window() override;

	void Initialize(const HINSTANCE& hInstance_, const std::string& serialName_, const std::string& titleName_, WinProc* winProc_,
					int posX_, int posY_, int width_, int height_);
	void Update();
	void ClearMemory();

	HWND GetHWnd();

private:
	HINSTANCE				_hInstance;
	std::string				_serialName;
	std::string				_titleName;
	WinProc*				_winProc;
	HWND					_hWnd;
};

