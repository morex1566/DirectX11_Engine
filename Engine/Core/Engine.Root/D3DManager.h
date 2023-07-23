#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>

class D3DManager
{
public:
	D3DManager(const D3DManager&)					= delete;
	D3DManager(const D3DManager&&)					= delete;
	D3DManager& operator=(const D3DManager&)		= delete;

	static D3DManager& Get();

	bool Initialize(const HWND& hWnd_);
	void Shutdown();

	void BeginScene();
	void EndScene();
	void Clear();

	const ID3D11Device*			GetDevice() const;
	const ID3D11DeviceContext*	GetDeviceContext() const;

	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();

private:
	D3DManager();

private:
	ID3D11Device*								_device;
	ID3D11DeviceContext*						_deviceContext;
	IDXGISwapChain*								_swapChain;
	ID3D11RenderTargetView*						_renderTargetView;
	ID3D11Texture2D*							_depthStencilBuffer;
	ID3D11DepthStencilView*						_depthStencilView;
	ID3D11DepthStencilState*					_depthStencilState;
	ID3D11RasterizerState*						_rasterizerState;
	char										_videoCardName[128];
	unsigned int								_videoCardMemory;
	size_t										_videoCardNameLength;
	D3D_FEATURE_LEVEL							_versionFeature;
	D3D11_VIEWPORT								_viewport{};
	XMMATRIX									_projectionMatrix{};
	XMMATRIX									_worldMatrix{};
	XMMATRIX									_orthoMatrix{};
};