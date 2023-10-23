#pragma once
class GUI : public Object
{
public:
	GUI()										= default;
	GUI(const GUI&)								= default;
	GUI& operator=(const GUI&)					= default;
	GUI(GUI&&) noexcept							= default;
	GUI& operator=(GUI&&) noexcept				= default;
	~GUI();

	bool Initialize(HWND hWnd_, ID3D11Device* device_, ID3D11DeviceContext* deviceContext_);
	void Render();
};

