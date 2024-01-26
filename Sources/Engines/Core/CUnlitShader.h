#pragma once
#include "OComponent.h"

class CUnlitShader : public OComponent
{
public:
	CUnlitShader(const OGameObject* InOwner);
	CUnlitShader(const CUnlitShader&)							= default;
	CUnlitShader& operator=(const CUnlitShader&)				= default;
	CUnlitShader(CUnlitShader&&) noexcept						= default;
	CUnlitShader& operator=(CUnlitShader&&) noexcept			= default;
	virtual ~CUnlitShader() override;

	virtual void 								    Initialize() override;
	virtual void									Release() override;
	virtual void									Start() override;
	virtual void									Tick() override;
	virtual void									End() override;
	bool											LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);

private:
	void											SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection);
	void											Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation);

	ComPtr<ID3D11VertexShader>						VertexShader;
	ComPtr<ID3D11PixelShader>						PixelShader;
	ComPtr<ID3D11InputLayout>						Layout;
	ComPtr<ID3D11Buffer>							MatrixBuffer;

	std::wstring									VSFilePath;
	std::wstring									PSFilePath;
};

