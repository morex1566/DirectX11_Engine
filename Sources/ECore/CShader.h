#pragma once
#include "OComponent.h"

class GCamera;
class ODirectX11;
class OWindow;

class CShader : public OComponent
{
public:
	CShader(const OGameObject& InOwner, const OWindow& InWindow, const ODirectX11& InDirectX11, const GCamera& InCamera);
	CShader(const CShader&)							= default;
	CShader& operator=(const CShader&)				= default;
	CShader(CShader&&) noexcept						= default;
	CShader& operator=(CShader&&) noexcept			= default;
	virtual ~CShader() override;

	virtual EHandleResultType						Initialize() override;
	virtual void									Release() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void									Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void									Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void									End() override;

	EHandleResultType								LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);

private:
	void											SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection);
	void											Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation);

	const ODirectX11*								DirectX11;
	const OWindow*									Window;
	const GCamera*									Camera;

	ComPtr<ID3D11VertexShader>						vertexShader;
	ComPtr<ID3D11PixelShader>						PixelShader;
	ComPtr<ID3D11InputLayout>						Layout;
	ComPtr<ID3D11Buffer>							MatrixBuffer;

	std::wstring									VSFilePath;
	std::wstring									PSFilePath;
};

