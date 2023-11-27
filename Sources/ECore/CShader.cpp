#include "PCH.h"
#include "CMesh.h"
#include "CShader.h"
#include "CTransform.h"
#include "GCamera.h"
#include "SApplication.h"
#include "SConsole.h"
#include "ODirectX11.h"
#include "OWindow.h"

#include "d3dcompiler.h"

CShader::CShader(const OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CShader::~CShader()
{
}

void CShader::Initialize()
{
	OComponent::Initialize();
}

void CShader::Release()
{
	OComponent::Release();
}

void CShader::Start()
{
	OComponent::Start();
}

void CShader::Tick()
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
	Transform = Owner->GetTransform();
	{
		SetShaderParameters(Transform->GetWorldMatrix(), Camera->GetViewMatrix(), Camera->GetProjectionMatrix());
	}

	// Set index order.
	for (const auto& Mesh : Owner->TFindComponents<CMesh>())
	{
		Mesh->Render();

		this->Render(Mesh->GetIndexCount(), 0, 0);
	}
}

void CShader::End()
{
	OComponent::End();
}

bool CShader::LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath)
{
	HRESULT						Result;
	ComPtr<ID3DBlob>			ErrorMsg;
	ComPtr<ID3DBlob>			VertexShaderBuffer;
	ComPtr<ID3DBlob>			PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC	PolygonLayout[2];
	D3D11_BUFFER_DESC			MatrixBufferDesc;


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

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(FMatrixBuffer);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;


	// Compile vertex shader.
	Result = D3DCompileFromFile(InVSFilePath.c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		VertexShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"D3DCompileFromFile() is failed.");
		return false;
	}

	// Compile pixel shader.
	Result = D3DCompileFromFile(InPSFilePath.c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		PixelShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		SConsole::LogError(L"D3DCompileFromFile() is failed.");
		return false;
	}

	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		// Create the vertex shader from the buffer.
		Result = Device.CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateVertexShader() is failed.");
			return false;
		}

		// Create the pixel shader from the buffer.
		Result = Device.CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreatePixelShader() is failed.");
			return false;
		}

		// Create layout.
		Result = Device.CreateInputLayout(PolygonLayout, static_cast<UINT>(std::size(PolygonLayout)), VertexShaderBuffer->GetBufferPointer(),
			VertexShaderBuffer->GetBufferSize(), Layout.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateInputLayout() is failed.");
			return false;
		}

		// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
		Result = Device.CreateBuffer(&MatrixBufferDesc, nullptr, MatrixBuffer.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateBuffer() is failed.");
			return false;
		}
	}

	return true;
}

void CShader::SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection)
{
	HRESULT							Result;
	D3D11_MAPPED_SUBRESOURCE		MappedResource;
	FMatrixBuffer*					CBuffer;
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
		CBuffer = static_cast<FMatrixBuffer*>(MappedResource.pData);
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
	}
}

void CShader::Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation)
{
	ID3D11DeviceContext& DeviceContext = SApplication::GetDirectX11()->GetDeviceContext();
	{
		DeviceContext.IASetInputLayout(Layout.Get());
		DeviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);
		DeviceContext.PSSetShader(PixelShader.Get(), nullptr, 0);
		DeviceContext.DrawIndexed(InIndexCount, InStartIndexToProcessing, InBaseVertexLocation);
	}
}
