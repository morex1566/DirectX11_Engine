#include "pch.h"
#include "Shader.h"

bool Shader::Initialize(ID3D11Device* device_, HWND hWnd_)
{
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int error;
	bool result;


	// Set the filename of the vertex shader.
	error = wcscpy_s(vsFilename, 128, Utils::ToWString(FROM_SOLUTION_PATH_TO("Shaders/light.vs")).c_str());
	if (error != 0)
	{
		return false;
	}

	// Set the filename of the pixel shader.
	error = wcscpy_s(psFilename, 128, Utils::ToWString(FROM_SOLUTION_PATH_TO("Shaders/light.ps")).c_str());
	if (error != 0)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device_, hWnd_, vsFilename, psFilename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Shader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool Shader::Render(ID3D11DeviceContext* deviceContext_, int indexCount_, XMMATRIX worldMatrix_, XMMATRIX viewMatrix_, XMMATRIX projectionMatrix_,
	ID3D11ShaderResourceView* texture_, XMFLOAT3 lightDirection_, XMFLOAT4 diffuseColor_)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext_, worldMatrix_, viewMatrix_, projectionMatrix_, texture_, lightDirection_, diffuseColor_);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext_, indexCount_);

	return true;
}


bool Shader::InitializeShader(ID3D11Device* device_, HWND hWnd_, const std::wstring& vsFilePath_, const std::wstring& psFilePath_)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilePath_.c_str(), NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd_, vsFilePath_);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hWnd_, Utils::ToString(vsFilePath_).c_str(), "Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilePath_.c_str(), NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hWnd_, psFilePath_);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hWnd_, Utils::ToString(psFilePath_).c_str(), "Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device_->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device_->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(result))
	{
		return false;
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

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device_->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device_->CreateSamplerState(&samplerDesc, &_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device_->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device_->CreateBuffer(&lightBufferDesc, NULL, &_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void Shader::ShutdownShader()
{
	// Release the light constant buffer.
	if (_lightBuffer)
	{
		_lightBuffer->Release();
		_lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (_sampleState)
	{
		_sampleState->Release();
		_sampleState = 0;
	}

	// Release the layout.
	if (_layout)
	{
		_layout->Release();
		_layout = 0;
	}

	// Release the pixel shader.
	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = 0;
	}

	// Release the vertex shader.
	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = 0;
	}

	return;
}


void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage_, HWND hWnd_, const std::wstring& shaderFilename_)
{
	char* compileErrors;
	unsigned __int64 bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage_->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage_->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage_->Release();
	errorMessage_ = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hWnd_, "Error compiling shader.  Check shader-error.txt for message.", Utils::ToString(shaderFilename_).c_str(), MB_OK);

	return;
}


bool Shader::SetShaderParameters(ID3D11DeviceContext* deviceContext_, XMMATRIX worldMatrix_, XMMATRIX viewMatrix_, XMMATRIX projectionMatrix_,
	ID3D11ShaderResourceView* texture_, XMFLOAT3 lightDirection_, XMFLOAT4 diffuseColor_)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;


	// Transpose the matrices to prepare them for the shader.
	worldMatrix_ = XMMatrixTranspose(worldMatrix_);
	viewMatrix_ = XMMatrixTranspose(viewMatrix_);
	projectionMatrix_ = XMMatrixTranspose(projectionMatrix_);

	// Lock the constant buffer so it can be written to.
	result = deviceContext_->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix_;
	dataPtr->view = viewMatrix_;
	dataPtr->projection = projectionMatrix_;

	// Unlock the constant buffer.
	deviceContext_->Unmap(_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext_->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext_->PSSetShaderResources(0, 1, &texture_);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext_->Map(_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->diffuseColor = diffuseColor_;
	dataPtr2->lightDirection = lightDirection_;
	dataPtr2->padding = 0.0f;

	// Unlock the constant buffer.
	deviceContext_->Unmap(_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext_->PSSetConstantBuffers(bufferNumber, 1, &_lightBuffer);

	return true;
}


void Shader::RenderShader(ID3D11DeviceContext* deviceContext_, int indexCount_)
{
	// Set the vertex input layout.
	deviceContext_->IASetInputLayout(_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext_->VSSetShader(_vertexShader, NULL, 0);
	deviceContext_->PSSetShader(_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext_->PSSetSamplers(0, 1, &_sampleState);

	// Render the triangle.
	deviceContext_->DrawIndexed(indexCount_, 0, 0);

	return;
}