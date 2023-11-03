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

	ODirectX11()									= delete;
	ODirectX11(const OWindow& Window);
	ODirectX11(const ODirectX11&)					= delete;
	ODirectX11& operator=(const ODirectX11&)		= delete;
	ODirectX11(ODirectX11&&) noexcept				= delete;
	ODirectX11& operator=(ODirectX11&&) noexcept	= delete;
	~ODirectX11() override;

	static EHandleResultType CALLBACK				MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);

	EHandleResultType								Initialize() override;
	void											Release() override;

	/**
	 * \brief Called once when the every frame.
	 */
	void											Tick() override;
	/**
	 * \brief Clear current render target view.
	 * \param InClearColor Clear color that erase render target view.
	 * \param InType ERenderModeType.
	 */
	void											ClearRenderTargetView(ERenderModeType InType, XMFLOAT4 InClearColor = { 0, 0, 0, 1 });
	/**
	 * \brief Clear current depth stencil view.
	 * \param Type ERenderModeType.
	 */
	void											ClearDepthStencilView(ERenderModeType Type);
	/**
	 * \brief Present the back buffer to the monitor since rendering is complete.
	 */
	void											Draw();

	void											SetRenderTargets(ERenderModeType InType);
	void											SetDepthStencilState(ERenderModeType InType);
	void											SetRasterizerState(ERenderModeType InType);
	void											SetViewport(ERenderModeType InType);

	ID3D11Device&									GetDevice() const;
	ID3D11DeviceContext&							GetDeviceContext() const;

private:
	EHandleResultType								CreateDeviceAndSwapChain(IDXGISwapChain** OutSwapChain, ID3D11Device** OutDevice, ID3D11DeviceContext** OutDeviceContext, const DXGI_SWAP_CHAIN_DESC& InSwapChainDesc);
	EHandleResultType								CreateRenderTargetView(ID3D11RenderTargetView** OutRenderTargetView);
	EHandleResultType								CreateDepthStencilBuffer(ID3D11Texture2D** OutDepthStencilBuffer, const D3D11_TEXTURE2D_DESC& InDepthStencilBufferDesc);
	EHandleResultType								CreateDepthStencilState(ID3D11DepthStencilState** OutDepthStecilState, const D3D11_DEPTH_STENCIL_DESC& InDepthStencilStateDesc);
	EHandleResultType								CreateDepthStencilView(ID3D11Texture2D* InDepthStencilBuffer, ID3D11DepthStencilView** OutDepthStencilView, const D3D11_DEPTH_STENCIL_VIEW_DESC& InDepthStencilViewDesc);
	EHandleResultType								CreateRasterizerState(ID3D11RasterizerState** OutRasterizerState, const D3D11_RASTERIZER_DESC& InRasterStateDesc);

	void											ReleaseDeviceAndSwapChain();
	void											ReleaseRenderTargetView2D();
	void											ReleaseDepthStencilBuffer2D();
	void											ReleaseDepthStencilState2D();
	void											ReleaseDepthStencilView2D();
	void											ReleaseRasterizerState2D();
	void											ReleaseRenderTargetView3D();
	void											ReleaseDepthStencilBuffer3D();
	void											ReleaseDepthStencilState3D();
	void											ReleaseDepthStencilView3D();
	void											ReleaseRasterizerState3D();
	/**
	 * \brief Called when window size is changed.
	 */
	void											Resize();

	/*
	 * Read only member variables...
	 */

	const OWindow*									Window;

	/*
	 * Core directx instance member variables...
	 */

	ComPtr<IDXGISwapChain>							SwapChain;
	ComPtr<ID3D11Device>							Device;
	ComPtr<ID3D11DeviceContext>						DeviceContext;
	DXGI_SWAP_CHAIN_DESC							SwapChainDesc;
	D3D_FEATURE_LEVEL								FeatureLevel;

	/*
	 * 2D rendering directx materials...
	 */

	ComPtr<ID3D11RenderTargetView>					RenderTargetView2D;
	ComPtr<ID3D11Texture2D>							DepthStencilBuffer2D;
	ComPtr<ID3D11DepthStencilState>					DepthStencilState2D;
	ComPtr<ID3D11DepthStencilView>					DepthStencilView2D;
	ComPtr<ID3D11RasterizerState>					RasterizerState2D;
	D3D11_VIEWPORT									Viewport2D;
	D3D11_TEXTURE2D_DESC							DepthStencilBufferDesc2D;
	D3D11_DEPTH_STENCIL_DESC						DepthStencilStateDesc2D;
	D3D11_DEPTH_STENCIL_VIEW_DESC					DepthStencilViewDesc2D;
	D3D11_RASTERIZER_DESC							RasterizerDesc2D;

	/*
	 * 3D rendering directx materials...
	 */

	ComPtr<ID3D11RenderTargetView>					RenderTargetView3D;
	ComPtr<ID3D11Texture2D>							DepthStencilBuffer3D;
	ComPtr<ID3D11DepthStencilState>					DepthStencilState3D;
	ComPtr<ID3D11DepthStencilView>					DepthStencilView3D;
	ComPtr<ID3D11RasterizerState>					RasterizerState3D;
	D3D11_VIEWPORT									Viewport3D;
	D3D11_TEXTURE2D_DESC							DepthStencilBufferDesc3D;
	D3D11_DEPTH_STENCIL_DESC						DepthStencilStateDesc3D;
	D3D11_DEPTH_STENCIL_VIEW_DESC					DepthStencilViewDesc3D;
	D3D11_RASTERIZER_DESC							RasterizerDesc3D;

	/*
	 * On/Off member variables...
	 */

	static uint8									bIsVsyncEnabled;
	static uint8									bIsScreenSizeChanged;

	/*
	 * GPU information member variables...
	 */

	DXGI_ADAPTER_DESC								AdapterDesc;
	uint32											DisplayMaxFPS;
};