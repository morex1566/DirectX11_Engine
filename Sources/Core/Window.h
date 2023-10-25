#pragma once

class Window
{
public:
	Window()										= default;
	Window(const Window&)							= default;
	Window& operator=(const Window&)				= default;
	Window(Window&&) noexcept						= default;
	Window& operator=(Window&&) noexcept			= default;
	~Window();

	bool Initialize(WNDPROC winProc_,
					const std::wstring& titleName_, 
					int windowStartPosX_, 
					int windowStartPosY_, 
					int windowWidth_,
					int windowHeight_,
					bool isFullScreenEnabled_);
	void Show();
	void Update();
	void Resize();

	HWND						GetHWND();
	std::wstring				GetTitleName();
	unsigned int				GetWindowWidth();
	unsigned int				GetWindowHeight();
	unsigned int				GetClientWidth();
	unsigned int				GetClientHeight();
	bool						GetIsFullScreenEnabled();

	void						SetTitleName(const std::wstring& titleName_);
	void						SetResolutionWidth(unsigned int width_);
	void						SetResolutionHeight(unsigned int height_);
	void						SetIsFullScreenEnabled(bool toggle_);

private:
	HWND						_hWnd;
	std::wstring				_titleName;
	unsigned int				_windowWidth;
	unsigned int				_windowHeight;
	unsigned int				_clientWidth;
	unsigned int				_clientHeight;
	bool						_isFullScreenEnabled;

	WNDPROC						_winProc;
	WNDCLASSEX					_wc;
	
};