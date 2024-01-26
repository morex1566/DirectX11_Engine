#include "PCH.h"
#include <d3dcompiler.h>

#include "CTransform.h"
#include "CLitShader.h"
#include "CMesh.h"
#include "CTexture.h"

CLitShader::CLitShader(const OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CLitShader::~CLitShader()
{
}

void CLitShader::Initialize()
{
	OComponent::Initialize();
}

void CLitShader::Release()
{
	OComponent::Release();
}

void CLitShader::Start()
{
	OComponent::Start();
}

void CLitShader::Tick()
{
	OComponent::Tick();

	const GCamera* Camera;
	const CTransform* Transform;

	auto DirectX11 = SApplication::GetDirectX11();
	{
		DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Model);
		DirectX11->SetViewport(ODirectX11::ERenderModeType::Model);
	}

	// Set shader essential params.
	Camera = SApplication::GetCamera();
	CTexture* texture = Owner->TFindComponent<CTexture>();

	Transform = Owner->GetTransform();
	{
		SetShaderParameters(Transform->GetWorldMatrix(), Camera->GetViewMatrix(), Camera->GetProjectionMatrix(),
							texture->GetResource());
	}

	// Set index order.
	for (const auto& Mesh : Owner->TFindComponents<CMesh>())
	{
		Mesh->Render();
		this->Render(Mesh->GetIndexCount(), 0, 0);
	}
}

void CLitShader::End()
{
	OComponent::End();
}

void CLitShader::LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath)
{
	HRESULT						Result;
	ComPtr<ID3DBlob>			ErrorMsg;
	ComPtr<ID3DBlob>			VertexShaderBuffer;
	ComPtr<ID3DBlob>			PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC	PolygonLayout[4];
	D3D11_BUFFER_DESC			MatrixBufferDesc;
	D3D11_SAMPLER_DESC			SamplerStateDesc;


	// Initialize layout desc.
	// Must be equal to shader constant buffer type.
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "COLOR";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	PolygonLayout[2].SemanticName = "TEXCOORD";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

	PolygonLayout[3].SemanticName = "NORMAL";
	PolygonLayout[3].SemanticIndex = 0;
	PolygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[3].InputSlot = 0;
	PolygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[3].InstanceDataStepRate = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(FWorldViewProjection);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// Create a texture sampler state description.
	SamplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerStateDesc.MipLODBias = 0.0f;
	SamplerStateDesc.MaxAnisotropy = 1;
	SamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerStateDesc.BorderColor[0] = 0;
	SamplerStateDesc.BorderColor[1] = 0;
	SamplerStateDesc.BorderColor[2] = 0;
	SamplerStateDesc.BorderColor[3] = 0;
	SamplerStateDesc.MinLOD = 0;
	SamplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Compile vertex shader.
	Result = D3DCompileFromFile(InVSFilePath.c_str(), nullptr, nullptr, "LitVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		VertexShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"D3DCompileFromFile() is failed.");
		throw std::exception();
	}

	// Compile pixel shader.
	Result = D3DCompileFromFile(InPSFilePath.c_str(), nullptr, nullptr, "LitPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		PixelShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"D3DCompileFromFile() is failed.");
		throw std::exception();
	}

	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		// Create the vertex shader from the buffer.
		Result = Device.CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, VertexShader.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateVertexShader() is failed.");
			throw std::exception();
		}

		// Create the pixel shader from the buffer.
		Result = Device.CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreatePixelShader() is failed.");
			throw std::exception();
		}

		// Create layout.
		Result = Device.CreateInputLayout(PolygonLayout, static_cast<UINT>(std::size(PolygonLayout)), VertexShaderBuffer->GetBufferPointer(),
			VertexShaderBuffer->GetBufferSize(), Layout.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateInputLayout() is failed.");
			throw std::exception();
		}

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		Result = Device.CreateBuffer(&MatrixBufferDesc, nullptr, MatrixBuffer.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateBuffer() is failed.");
			throw std::exception();
		}

		// Create the texture sampler state.
		Result = Device.CreateSamplerState(&SamplerStateDesc, SamplerState.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateSamplerState() is failed.");
			throw std::exception();
		}
	}

}

void CLitShader::SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection, ID3D11ShaderResourceView* texture)
{
	HRESULT							Result;
	D3D11_MAPPED_SUBRESOURCE		MappedResource;
	FWorldViewProjection*					CBuffer;
	UINT							BufferNum;

	ID3D11DeviceContext& DeviceContext = SApplication::GetDirectX11()->GetDeviceContext();
	{
		// Lock cbuffer.
		Result = DeviceContext.Map(MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (FAILED(Result))
		{
			SConsole::LogError(L"Map() is failed.");
			throw std::exception();
		}

		// Set cbuffer's parameters.
		CBuffer = static_cast<FWorldViewProjection*>(MappedResource.pData);
		{
			CBuffer->World = XMMatrixTranspose(InWorld);
			CBuffer->View = XMMatrixTranspose(InView);
			CBuffer->Projection = XMMatrixTranspose(InProjection);
		}

		// Unlock cbuffer.
		DeviceContext.Unmap(MatrixBuffer.Get(), 0);

		// Set position of cbuffer in vs.
		BufferNum = 0;

		// Set cbuffer in vs.
		DeviceContext.VSSetConstantBuffers(BufferNum, 1, MatrixBuffer.GetAddressOf());

		// Set shader resource.
		DeviceContext.PSSetShaderResources(0, 1, &texture);
	}
}

void CLitShader::Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation)
{
	ID3D11DeviceContext& DeviceContext = SApplication::GetDirectX11()->GetDeviceContext();
	{
		DeviceContext.IASetInputLayout(Layout.Get());
		DeviceContext.VSSetShader(VertexShader.Get(), nullptr, 0);
		DeviceContext.PSSetShader(PixelShader.Get(), nullptr, 0);
		DeviceContext.PSSetSamplers(0, 1, SamplerState.GetAddressOf());
		DeviceContext.DrawIndexed(InIndexCount, InStartIndexToProcessing, InBaseVertexLocation);
	}
}
