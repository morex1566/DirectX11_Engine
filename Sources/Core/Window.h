#pragma once

class Window : public Object
{
public:
	Window()										= default;
	Window(const Window&)							= default;
	Window& operator=(const Window&)				= default;
	Window(Window&&) noexcept						= default;
	Window& operator=(Window&&) noexcept			= default;
	~Window();

	bool Initialize(WNDPROC winProc_, const std::wstring& titleName_, int windowWidth_, int windowHeight_, bool isFullScreenEnabled_);
	void Show();
	void Update();

	static void					CalculateClientScreen();
	static void					CalculateWindowScreen();

	HWND						GetHWND();
	std::wstring				GetTitleName();
	static unsigned int			GetWindowWidth();
	static unsigned int			GetWindowHeight();
	static unsigned int			GetClientWidth();
	static unsigned int			GetClientHeight();
	static bool					GetIsFullScreenEnabled();

	static void					SetTitleName(const std::wstring& titleName_);
	static void					SetResolutionWidth(unsigned int width_);
	static void					SetResolutionHeight(unsigned int height_);
	static void					SetIsFullScreenEnabled(bool toggle_);

private:
	static HWND					_hWnd;
	static std::wstring			_titleName;
	static unsigned int			_windowWidth;
	static unsigned int			_windowHeight;
	static unsigned int			_clientWidth;
	static unsigned int			_clientHeight;
	static bool					_isFullScreenEnabled;

	WNDPROC						_winProc;
	WNDCLASSEX					_wc;
	
};