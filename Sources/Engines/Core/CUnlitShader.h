#pragma once
#include "OComponent.h"

class CUnlitShader : public OComponent
{
public:
	CUnlitShader(OGameObject* InOwner);

	CUnlitShader()									 = default;
	CUnlitShader(const CUnlitShader&)				 = default;
	CUnlitShader& operator=(const CUnlitShader&)	 = default;
	CUnlitShader(CUnlitShader&&) noexcept			 = default;
	CUnlitShader& operator=(CUnlitShader&&) noexcept = default;
	virtual ~CUnlitShader() override;


public:
	virtual void 								    Init() override;
	virtual void									Shutdown() override;
	virtual void									Start() override;
	virtual void									Tick() override;
	virtual void									End() override;


public:
	bool											Load(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);


private:
	void											SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection);
	void											Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation);


private:
	ComPtr<ID3D11VertexShader>						VertexShader;
	ComPtr<ID3D11PixelShader>						PixelShader;
	ComPtr<ID3D11InputLayout>						Layout;
	ComPtr<ID3D11Buffer>							MatrixBuffer;

	std::wstring									VSFilePath;
	std::wstring									PSFilePath;

};

