#include "pch.h"
#include "UnlitShader.h"

#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "D3DManager.h"
#include "Mesh.h"
#include "Texture.h"

UnlitShader::UnlitShader()
	: Shader()
{
	
}

UnlitShader::~UnlitShader()
{
	UnlitShader::Destroy();
}

void UnlitShader::Render(Material* material_, Camera* viewportCamera_, GameObject* owner_)
{
	D3DManager* d3dManager = &D3DManager::Get();

	sendParametersToShader(material_, viewportCamera_, owner_, material_->GetAlbedo());

	for (const auto& mesh : material_->GetMeshs())
	{
		// Set the vertex input layout.
		d3dManager->GetDeviceContext()->IASetInputLayout(_inputLayout.Get());

		// Set the vertex and pixel shaders that will be used to render this triangle.
		d3dManager->GetDeviceContext()->VSSetShader(_vertexShader.Get(), NULL, 0);
		d3dManager->GetDeviceContext()->PSSetShader(_pixelShader.Get(), NULL, 0);

		// Set the sampler state in the pixel shader.
		d3dManager->GetDeviceContext()->PSSetSamplers(0, 1, _sampleState.GetAddressOf());

		// Render the triangle.
		d3dManager->GetDeviceContext()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
	}
}

void UnlitShader::Destroy()
{
}

void UnlitShader::ReadShader(const std::string& vsPath_, const std::string& psPath_)
{
	HRESULT result;

	ComPtr<ID3D10Blob>			vertexShaderBuffer;
	ComPtr<ID3D10Blob>			pixelShaderBuffer;
	ComPtr<ID3D10Blob>			errorMessage;
	D3D11_INPUT_ELEMENT_DESC	polygonLayout[2];
	uint						numElements;
	D3D11_SAMPLER_DESC			samplerDesc;
	D3D11_BUFFER_DESC			matrixBufferDesc;
	D3D11_BUFFER_DESC			cameraBufferDesc;
	D3D11_BUFFER_DESC			lightBufferDesc;

	result = D3DCompileFromFile(GetWString(vsPath_).c_str(), nullptr, nullptr, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		if (errorMessage)
		{
			LogManager::LogError("Vertex shader compile error. " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer())));
			return;
		}
		else
		{
			LogManager::LogError("Missing vertex shader file. ");
			return;
		}
	}

	result = D3DManager::Get().GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogError("Create vertex shader failure.");
		return;
	}

	result = D3DCompileFromFile(GetWString(psPath_).c_str(), nullptr, nullptr, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf());
	if (FAILED(result))
	{
		if (errorMessage)
		{
			LogManager::LogError("Pixel shader compile error. " + std::string(static_cast<const char*>(errorMessage->GetBufferPointer())));
			return;
		}
		else
		{
			LogManager::LogError("Missing pixel shader file. ");
			return;
		}
	}

	result = D3DManager::Get().GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogError("Create pixel shader failure.");
		return;
	}

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

	numElements = std::size(polygonLayout);

	// Create the vertex input layout.
	result = D3DManager::Get().GetDevice()->CreateInputLayout(polygonLayout, numElements,
	vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), _inputLayout.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogError("Create vertex input layout failure.");
		return;
	}

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
	result = D3DManager::Get().GetDevice()->CreateSamplerState(&samplerDesc, _sampleState.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogError("Fail to create the texture sampler state.");
		return;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = D3DManager::Get().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, _matrixBuffer.GetAddressOf());
	if (FAILED(result))
	{
		LogManager::LogError("Fail to create the matrix buffer.");
		return;
	}

	//// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	//cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	//cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cameraBufferDesc.MiscFlags = 0;
	//cameraBufferDesc.StructureByteStride = 0;

	//// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = D3DManager::Get().GetDevice()->CreateBuffer(&cameraBufferDesc, NULL, _cameraBuffer.GetAddressOf());
	//if (FAILED(result))
	//{
	//	LogManager::LogError("Fail to create the camera buffer.");
	//	return;
	//}

	//// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	//// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	//lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	//lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	//lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//lightBufferDesc.MiscFlags = 0;
	//lightBufferDesc.StructureByteStride = 0;

	//// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = D3DManager::Get().GetDevice()->CreateBuffer(&lightBufferDesc, NULL, _lightBuffer.GetAddressOf());
	//if (FAILED(result))
	//{
	//	LogManager::LogError("Fail to create the light buffer.");
	//	return;
	//}
}

void UnlitShader::sendParametersToShader(Material* material_, Camera* viewportCamera_, GameObject* owner_, DirectionalLight* light_)
{
	D3DManager* d3dManager = &D3DManager::Get();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	// Send the matrix constant buffer to shader.
	{
		Transform* transform = owner_->GetTransform();

		XMMATRIX worldTM = XMMatrixTranspose(transform->GetWorldMatrix());
		XMMATRIX viewTM = XMMatrixTranspose(viewportCamera_->GetViewMatrix());
		XMMATRIX projectionTM = XMMatrixTranspose(d3dManager->GetProjectionMatrix());

		// Lock the constant buffer so it can be written to.
		result = d3dManager->GetDeviceContext()->Map(_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			LogManager::LogError("Fail to lock constant buffer at shader parameters");
			return;
		}

		// Set matrix constant buffer parameters.
		auto* matrixBufferParam = static_cast<MatrixBufferType*>(mappedResource.pData);
		matrixBufferParam->world = worldTM;
		matrixBufferParam->view = viewTM;
		matrixBufferParam->projection = projectionTM;

		d3dManager->GetDeviceContext()->Unmap(_matrixBuffer.Get(), 0);
		d3dManager->GetDeviceContext()->VSSetConstantBuffers(0, 1, _matrixBuffer.GetAddressOf());
	}

	//// Send the camera constant buffer to shader.
	//{
	//	XMFLOAT3 cameraPosition = viewportCamera_->GetTransform()->GetPosition();

	//	// Lock the constant buffer so if can be written to.
	//	result = d3dManager->GetDeviceContext()->Map(_cameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//	if (FAILED(result))
	//	{
	//		LogManager::LogError("Fail to lock constant buffer at shader parameters");
	//		return;
	//	}

	//	// Set camera constant buffer parameters.
	//	auto* cameraBufferParam = static_cast<CameraBufferType*>(mappedResource.pData);
	//	cameraBufferParam->cameraPosition = cameraPosition;
	//	cameraBufferParam->padding = 0.0f;

	//	d3dManager->GetDeviceContext()->Unmap(_cameraBuffer.Get(), 0);
	//	d3dManager->GetDeviceContext()->VSSetConstantBuffers(1, 1, _cameraBuffer.GetAddressOf());
	//}

	//// Send the light constant buffer to shader.
	//{
	//	// Lock the light constant buffer so it can be written to.
	//	result = d3dManager->GetDeviceContext()->Map(_lightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//	if (FAILED(result))
	//	{
	//		LogManager::LogError("Fail to lock constant buffer at shader parameters");
	//		return;
	//	}

	//	// Set light constant buffer parameter.
	//	auto* lightBufferParams = static_cast<LightBufferType*>(mappedResource.pData);
	//	lightBufferParams->diffuseColor = light_->GetDiffuseColor();
	//	lightBufferParams->ambientColor = light_->GetAmbientColor();
	//	lightBufferParams->specularColor = light_->GetSpecularColor();
	//	lightBufferParams->lightDirection = light_->GetDirection();
	//	lightBufferParams->specularIntensity = light_->GetSpecularIntensity();

	//	d3dManager->GetDeviceContext()->Unmap(_lightBuffer.Get(), 0);
	//	d3dManager->GetDeviceContext()->PSSetConstantBuffers(0, 1, _lightBuffer.GetAddressOf());
	//}

	// Send texture resources to shader.
	{
		std::vector<Texture*> textures = material_->GetTexture();

		ID3D11ShaderResourceView* srv = textures[0]->GetTextureView();
		d3dManager->GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
	}
}
