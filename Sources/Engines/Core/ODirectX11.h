#pragma once
#include "Object.h"

class OWindow;

class ODirectX11 : public Object
{
public:
	enum class ERenderModeType
	{
		Interface,
		Model
	};


public:
	ODirectX11(const OWindow& InWindow);
	ODirectX11(const ODirectX11&)					= default;
	ODirectX11& operator=(const ODirectX11&)		= default;
	ODirectX11(ODirectX11&&) noexcept				= default;
	ODirectX11& operator=(ODirectX11&&) noexcept	= default;
	~ODirectX11() override;


public:
	static void CALLBACK			MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);


public:
	void							Init() override;
	void							Shutdown() override;
	void							Tick() override;


public:
	void							ClearRenderTargetView(ERenderModeType InType, XMFLOAT4 InClearColor = { 0, 0, 0, 1 }) const;
	void							ClearDepthStencilView(ERenderModeType Type) const;
	void							Draw();
	void							SetRenderTargets(ERenderModeType InType) const;
	void							SetDepthStencilState(ERenderModeType InType) const;
	void							SetRasterizerState(ERenderModeType InType) const;
	void							SetViewport(ERenderModeType InType) const;
	ID3D11Device&					GetDevice() const { return *Device; }
	ID3D11DeviceContext&			GetDeviceContext() const { return *DeviceContext; }


private:
	void							CreateDeviceAndSwapChain(IDXGISwapChain** OutSwapChain, ID3D11Device** OutDevice, ID3D11DeviceContext** OutDeviceContext, const DXGI_SWAP_CHAIN_DESC& InSwapChainDesc);
	void							CreateRenderTargetView(ID3D11RenderTargetView** OutRenderTargetView);
	void							CreateDepthStencilBuffer(ID3D11Texture2D** OutDepthStencilBuffer, const D3D11_TEXTURE2D_DESC& InDepthStencilBufferDesc);
	void							CreateDepthStencilState(ID3D11DepthStencilState** OutDepthStecilState, const D3D11_DEPTH_STENCIL_DESC& InDepthStencilStateDesc);
	void							CreateDepthStencilView(ID3D11Texture2D* InDepthStencilBuffer, ID3D11DepthStencilView** OutDepthStencilView, const D3D11_DEPTH_STENCIL_VIEW_DESC& InDepthStencilViewDesc);
	void							CreateRasterizerState(ID3D11RasterizerState** OutRasterizerState, const D3D11_RASTERIZER_DESC& InRasterStateDesc);
	void							ReleaseDeviceAndSwapChain();
	void							ReleaseRenderTargetView2D();
	void							ReleaseDepthStencilBuffer2D();
	void							ReleaseDepthStencilState2D();
	void							ReleaseDepthStencilView2D();
	void							ReleaseRasterizerState2D();
	void							ReleaseRenderTargetView3D();
	void							ReleaseDepthStencilBuffer3D();
	void							ReleaseDepthStencilState3D();
	void							ReleaseDepthStencilView3D();
	void							ReleaseRasterizerState3D();
	void							Resize();


// 설정값
private:
	static uint8					bIsVsyncEnabled;
	static uint8					bIsScreenSizeChanged;


// 출력할 윈도우
private:
	const OWindow&					Window;


// DirectX11 디바이스 객체
private:
	IDXGISwapChain*					SwapChain;
	ID3D11Device*					Device;
	ID3D11DeviceContext*			DeviceContext;
	DXGI_SWAP_CHAIN_DESC			SwapChainDesc;
	D3D_FEATURE_LEVEL				FeatureLevel;
	DXGI_ADAPTER_DESC				AdapterDesc;
	uint32							DisplayMaxFPS;
	

// 2D 랜더링용 
private:
	ID3D11RenderTargetView*			RenderTargetView2D;
	ID3D11Texture2D*				DepthStencilBuffer2D;
	ID3D11DepthStencilState*		DepthStencilState2D;
	ID3D11DepthStencilView*			DepthStencilView2D;
	ID3D11RasterizerState*			RasterizerState2D;
	D3D11_VIEWPORT					Viewport2D;
	D3D11_TEXTURE2D_DESC			DepthStencilBufferDesc2D;
	D3D11_DEPTH_STENCIL_DESC		DepthStencilStateDesc2D;
	D3D11_DEPTH_STENCIL_VIEW_DESC	DepthStencilViewDesc2D;
	D3D11_RASTERIZER_DESC			RasterizerDesc2D;


// 3D 랜더링용 
private:
	ID3D11RenderTargetView*			RenderTargetView3D;
	ID3D11Texture2D*				DepthStencilBuffer3D;
	ID3D11DepthStencilState*		DepthStencilState3D;
	ID3D11DepthStencilView*			DepthStencilView3D;
	ID3D11RasterizerState*			RasterizerState3D;
	D3D11_VIEWPORT					Viewport3D;
	D3D11_TEXTURE2D_DESC			DepthStencilBufferDesc3D;
	D3D11_DEPTH_STENCIL_DESC		DepthStencilStateDesc3D;
	D3D11_DEPTH_STENCIL_VIEW_DESC	DepthStencilViewDesc3D;
	D3D11_RASTERIZER_DESC			RasterizerDesc3D;
};