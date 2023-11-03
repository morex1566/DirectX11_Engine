#include "PCH.h"
#include "OApplication.h"
#include "OConsole.h"
#include "ODirectX11.h"
#include "OWindow.h"
#include "OShader.h"

OShader::OShader(ODirectX11& InDirectX11, OWindow& InWindow, const std::string& InVertexShaderFilePath, const std::string& InPixelShaderFilePath):
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_layout(nullptr),
	m_matrixBuffer(nullptr)
{
	DirectX11 = &InDirectX11;
	Window = &InWindow;
	VertexShaderFilePath = InVertexShaderFilePath;
	PixelShaderFilePath = InPixelShaderFilePath;
}

OShader::~OShader()
{
}

Object::EHandleResultType OShader::Initialize()
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(ToWString(VertexShaderFilePath).c_str(), nullptr, nullptr, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		OConsole::LogError(L"D3DCompileFromFile() is failed.");
		return EHandleResultType::Failed;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(ToWString(PixelShaderFilePath).c_str(), nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		OConsole::LogError(L"D3DCompileFromFile() is failed.");
		return EHandleResultType::Failed;
	}

	// Create the vertex shader from the buffer.
	result = DirectX11->GetDevice().CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreateVertexShader() is failed.");
		return EHandleResultType::Failed;
	}

	// Create the pixel shader from the buffer.
	result = DirectX11->GetDevice().CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreatePixelShader() is failed.");
		return EHandleResultType::Failed;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = DirectX11->GetDevice().CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreateInputLayout() is failed.");
		return EHandleResultType::Failed;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(FMatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = DirectX11->GetDevice().CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

void OShader::Release()
{
}

OApplication::EHandleResultType OShader::SetShaderParameters(XMMATRIX InWorld, XMMATRIX InView, XMMATRIX InProjection)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	FMatrixBuffer* dataPtr;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	InWorld = XMMatrixTranspose(InWorld);
	InView = XMMatrixTranspose(InView);
	InProjection = XMMatrixTranspose(InProjection);

	// Lock the constant buffer so it can be written to.
	result = DirectX11->GetDeviceContext().Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		OConsole::LogError(L"D3DCompileFromFile() is failed.");
		return EHandleResultType::Failed;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<FMatrixBuffer*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->World = InWorld;
	dataPtr->View = InView;
	dataPtr->Projection = InProjection;

	// Unlock the constant buffer.
	DirectX11->GetDeviceContext().Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	DirectX11->GetDeviceContext().VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return EHandleResultType::Success;
}

void OShader::Process(UINT InIndexCount)
{
	// Set the vertex input layout.
	DirectX11->GetDeviceContext().IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	DirectX11->GetDeviceContext().VSSetShader(m_vertexShader, nullptr, 0);
	DirectX11->GetDeviceContext().PSSetShader(m_pixelShader, nullptr, 0);

	// Render the triangle.
	DirectX11->GetDeviceContext().DrawIndexed(InIndexCount, 0, 0);
}
