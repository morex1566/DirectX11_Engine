#include "PCH.h"
#include "CMesh.h"
#include "CShader.h"
#include "CTransform.h"
#include "GCamera.h"
#include "OConsole.h"
#include "ODirectX11.h"
#include "OWindow.h"

#include "d3dcompiler.h"

CShader::CShader(const OGameObject& InOwner, const OWindow& InWindow, const ODirectX11& InDirectX11, const GCamera& InCamera)
	: OComponent(InOwner)
{
	DirectX11 = &InDirectX11;
	Window = &InWindow;
	Camera = &InCamera;
}

CShader::~CShader()
{
}

Object::EHandleResultType CShader::Initialize()
{
	OComponent::Initialize();

	return EHandleResultType::Success;
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

	if (const CTransform* Transform = Owner->GetTransform())
	{
		SetShaderParameters(Transform->GetWorldMatrix(), Camera->GetViewMatrix(), Camera->GetProjectionMatrix());
	}

	for (const auto& Mesh : Owner->TFindComponents<CMesh>())
	{
		Render(Mesh->GetIndexCount(), 0, 0);
	}
}

void CShader::End()
{
	OComponent::End();
}

Object::EHandleResultType CShader::LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath)
{
	HRESULT						Result;
	ComPtr<ID3DBlob>			ErrorMsg;
	ComPtr<ID3DBlob>			VertexShaderBuffer;
	ComPtr<ID3DBlob>			PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC	PolygonLayout[2];
	D3D11_BUFFER_DESC			MatrixBufferDesc;


	// Compile vertex shader.
	Result = D3DCompileFromFile(InVSFilePath.c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		VertexShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"D3DCompileFromFile() is failed.");
		return EHandleResultType::Failed;
	}

	// Compile pixel shader.
	Result = D3DCompileFromFile(InPSFilePath.c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		PixelShaderBuffer.GetAddressOf(), ErrorMsg.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"D3DCompileFromFile() is failed.");
		return EHandleResultType::Failed;
	}

	// Create the vertex shader from the buffer.
	Result = DirectX11->GetDevice().CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"CreateVertexShader() is failed.");
		return EHandleResultType::Failed;
	}

	// Create the pixel shader from the buffer.
	Result = DirectX11->GetDevice().CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"CreatePixelShader() is failed.");
		return EHandleResultType::Failed;
	}

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

	// Create layout.
	Result = DirectX11->GetDevice().CreateInputLayout(PolygonLayout,  static_cast<UINT>(std::size(PolygonLayout)), VertexShaderBuffer->GetBufferPointer(),
		VertexShaderBuffer->GetBufferSize(), Layout.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"CreateInputLayout() is failed.");
		return EHandleResultType::Failed;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(FMatrixBuffer);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	Result = DirectX11->GetDevice().CreateBuffer(&MatrixBufferDesc, nullptr, MatrixBuffer.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"CreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

void CShader::SetShaderParameters(const XMMATRIX& InWorld, const XMMATRIX& InView, const XMMATRIX& InProjection)
{
	HRESULT							Result;
	D3D11_MAPPED_SUBRESOURCE		MappedResource;
	FMatrixBuffer*					CBuffer;
	UINT							BufferNum;

	// Lock cbuffer.
	Result = DirectX11->GetDeviceContext().Map(MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		OConsole::LogError(L"Map() is failed.");
		throw std::exception();
	}

	// Set cbuffer's parameters.
	{
		CBuffer = static_cast<FMatrixBuffer*>(MappedResource.pData);

		CBuffer->World = XMMatrixTranspose(InWorld);
		CBuffer->View = XMMatrixTranspose(InView);
		CBuffer->Projection = XMMatrixTranspose(InProjection);
	}

	// Unlock cbuffer.
	DirectX11->GetDeviceContext().Unmap(MatrixBuffer.Get(), 0);

	// Set position of Cbuffer in vs.
	BufferNum = 0;

	// Set cbuffer in vs.
	DirectX11->GetDeviceContext().VSSetConstantBuffers(BufferNum, 1, MatrixBuffer.GetAddressOf());
}

void CShader::Render(UINT InIndexCount, UINT InStartIndexToProcessing, INT InBaseVertexLocation)
{
	ID3D11DeviceContext& DeviceContext = DirectX11->GetDeviceContext();
	{
		DeviceContext.IASetInputLayout(Layout.Get());
		DeviceContext.VSSetShader(vertexShader.Get(), nullptr, 0);
		DeviceContext.PSSetShader(PixelShader.Get(), nullptr, 0);
		DeviceContext.DrawIndexed(InIndexCount, InStartIndexToProcessing, InBaseVertexLocation);
	}
}
