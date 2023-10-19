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

	bool Initialize(WNDPROC winProc_, const std::wstring& titleName_, int screenWidth_, int screenHeight_);
	void Show();
	void Update();

	static unsigned int		GetWidth();
	static unsigned int		GetHeight();

	HWND			GetHWND() const;
	std::wstring	GetTitleName() const;

private:
	std::wstring				_titleName;
	WNDCLASSEX					_wc;
	static HWND					_hWnd;
	static unsigned int			_width;
	static unsigned int			_height;
	
};