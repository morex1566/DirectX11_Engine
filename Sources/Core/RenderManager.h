#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <dxgi.h>

class RenderManager
{
public:
	RenderManager()			= default;
	~RenderManager()		= default;

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Update();
	void Dispose();
	void Clear();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetDeviceContext();
	XMMATRIX				GetProjectionMatrix();
	XMMATRIX				GetWorldMatrix();
	XMMATRIX				GetOrthoMatrix();

	void SetVideoCardInfo(char*, int&);
	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool									_isVsyncEnabled;
	int										_videoCardMemory;
	char									_videoCardDescription[128];
	XMMATRIX								_projectionMatrix;
	XMMATRIX								_worldMatrix;
	XMMATRIX								_orthoMatrix;
	D3D11_VIEWPORT							_viewport;

	ComPtr<IDXGISwapChain>					_swapChain;
	ComPtr<ID3D11Device>					_device;
	ComPtr<ID3D11DeviceContext>				_deviceContext;
	ComPtr<ID3D11RenderTargetView>			_renderTargetView;
	ComPtr<ID3D11Texture2D>					_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState>			_depthStencilState;
	ComPtr<ID3D11DepthStencilView>			_depthStencilView;
	ComPtr<ID3D11RasterizerState>			_rasterState;
};
