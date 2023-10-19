#include "PCH.h"
#include "Shader.h"

Shader::~Shader()
{
}

bool Shader::Initialize(ID3D11Device* device_, HWND hWnd_, const std::wstring& shaderFilePath_)
{
	HRESULT						result;
	ID3DBlob*					errorMsg;
	ID3DBlob*					vertexShaderBuffer;
	ID3DBlob*					pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[2];
	D3D11_BUFFER_DESC			matrixBufferDesc;

	std::string vertexShaderEntryPoint = ToString(GetFileNameToWString(shaderFilePath_) + L"VertexShader").c_str();
	std::string pixelShaderEntryPoint = ToString(GetFileNameToWString(shaderFilePath_) + L"PixelShader").c_str();

	// Compile the vertex shader code.
	result = D3DCompileFromFile(shaderFilePath_.c_str(), nullptr, nullptr, vertexShaderEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMsg);
	if (FAILED(result))
	{
		// If the shader failed to compile.
		if (errorMsg)
		{
			OutputShaderErrorMessage(errorMsg, hWnd_, shaderFilePath_);
		}
		// If simply could not find the shader file itself.
		else
		{
			MessageBox(hWnd_, shaderFilePath_.c_str(), L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(shaderFilePath_.c_str(), nullptr, nullptr, pixelShaderEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMsg);
	if (FAILED(result))
	{
		// If the shader failed to compile.
		if (errorMsg)
		{
			OutputShaderErrorMessage(errorMsg, hWnd_, shaderFilePath_);
		}
		// If simply could not find the shader file itself.
		else
		{
			MessageBox(hWnd_, shaderFilePath_.c_str(), L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader.
	result = device_->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader.
	result = device_->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
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

	// Create the vertex input layout.
	result = device_->CreateInputLayout(polygonLayout, std::size(polygonLayout), vertexShaderBuffer->GetBufferPointer(),
					vertexShaderBuffer->GetBufferSize(), _inputLayout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the matrix cbuffer.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the cbuffer pointer.
	result = device_->CreateBuffer(&matrixBufferDesc, nullptr, _matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Setup the flag as true.
	{
		_isEnabled = true;
		_isActivated = true;
	}

	return true;
}

void Shader::ZeroInstanceMemory()
{
}

void Shader::Render()
{

}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMsg_, HWND hWnd_, const std::wstring& wstring_)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = static_cast<char*>(errorMsg_->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMsg_->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hWnd_, L"Error compiling shader.  Check shader-error.txt for message.", wstring_.c_str(), MB_OK);

	return;
}
