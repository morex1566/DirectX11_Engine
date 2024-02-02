#pragma once
#include "OComponent.h"

class CLitShader : public OComponent
{
public:
	CLitShader(OGameObject* InOwner);

	CLitShader()											= default;
	CLitShader(const CLitShader&)							= default;
	CLitShader& operator=(const CLitShader&)				= default;
	CLitShader(CLitShader&&) noexcept						= default;
	CLitShader& operator=(CLitShader&&) noexcept			= default;
	~CLitShader() override;


public:
	virtual void 					Init() override;
	virtual void					Shutdown() override;
	virtual void					Start() override;
	virtual void					Tick() override;
	virtual void					End() override;


public:
	void							Load(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);
	void							SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection, 
														FBoneTransform* InBoneTransforms, ID3D11ShaderResourceView* texture);
	void							Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation);


private:
	ComPtr<ID3D11VertexShader>		VertexShader;
	ComPtr<ID3D11PixelShader>		PixelShader;
	ComPtr<ID3D11InputLayout>		Layout;
	ComPtr<ID3D11Buffer>			MatrixBuffer;
	ComPtr<ID3D11SamplerState>		SamplerState;

	std::wstring					VSFilePath;
	std::wstring					PSFilePath;
};

