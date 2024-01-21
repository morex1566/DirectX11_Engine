#pragma once
class DirectX11
{
public:
	DirectX11(HINSTANCE hInstance);
	DirectX11(const DirectX11&)						= delete;
	DirectX11& operator=(const DirectX11&)			= delete;
	DirectX11(DirectX11&&) noexcept					= delete;
	DirectX11& operator=(DirectX11&&) noexcept		= delete;
	~DirectX11()									= default;


public:
	static void WINAPI MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam);


public:
	void Init(unsigned int windowWidth, unsigned int windowHeight,
				HWND hOutputWindow, bool outputWindowFullScreenEnabled);
	void Shutdown();
	void ClearRenderTargetView();
	void ClearDepthStencilView();
	void Draw();

	FORCEINLINE ID3D11Device* GetDevice() const { return device; }
	FORCEINLINE ID3D11DeviceContext* GetDeviceContext() const { return deviceContext; }
	FORCEINLINE XMMATRIX GetProjection() const { return projectionMatrix; }
	FORCEINLINE XMMATRIX GetWorld() const { return worldMatrix; }
	FORCEINLINE XMMATRIX GetOrtho() const { return orthoMatrix; }

private:
	static bool						isVsyncEnabled;


private:
	HINSTANCE						hInstance;

    IDXGISwapChain*					swapChain;
    ID3D11Device*					device;
    ID3D11DeviceContext*			deviceContext;
    ID3D11RenderTargetView*			renderTargetView;
    ID3D11Texture2D*				depthStencilBuffer;
    ID3D11DepthStencilState*		depthStencilState;
    ID3D11DepthStencilView*			depthStencilView;
    ID3D11RasterizerState*			rasterState;
    XMMATRIX						projectionMatrix;
    XMMATRIX						worldMatrix;
    XMMATRIX						orthoMatrix;
    D3D11_VIEWPORT					viewport;


};

