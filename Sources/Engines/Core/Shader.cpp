#include "PCH.h"
#include "Shader.h"

Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
    : device(device), deviceContext(deviceContext)
{
}

void Shader::Start()
{
	Component::Start();
}

void Shader::Update()
{
	Component::Update();
}

void Shader::Shutdown()
{
	Component::Shutdown();

    Release();
}

void Shader::Load(const std::wstring& vsFilePath, const std::wstring& psFilePath)
{
    Release();
    loadVertexShader(vsFilePath);
    loadPixelShader(psFilePath);
}

void Shader::loadVertexShader(const std::wstring& filePath)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessage;


	// 버텍스 쉐이더를 컴파일합니다.
    result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "LightVertexShader", "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            Console::LogError(readShaderErrorMsg(errorMessage), __FILE__, __LINE__);
        }
        else
        {
            Console::LogError(L"Missing Shader File.", __FILE__, __LINE__);
        }

        return;
    }


    // 버텍스 쉐이더를 생성합니다.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
    if (FAILED(result))
    {
        Console::LogError(L"device->CreateVertexShader() is failed.", __FILE__, __LINE__);
        return;
    }


    // 버텍스 쉐이더에 넘길 vertex input layout를 설정합니다.
    // Vertex 타입과 shader에 전달할 cbuffer 타입이 일치해야합니다.
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    {
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
    }


    // 버텍스 쉐이더에 넘길 input layout를 생성합니다.
    result = device->CreateInputLayout(polygonLayout, sizeof(polygonLayout) / sizeof(polygonLayout[0]),
        vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
        &layout);
    if (FAILED(result))
    {
        Console::LogError(L"device->CreateInputLayout() is failed.", __FILE__, __LINE__);
        return;
    }


    // World, View, Projection이 담긴 cbuffer를 설정합니다.
    D3D11_BUFFER_DESC matrixBufferDesc;
    {
        matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDesc.ByteWidth = sizeof(MVPType);
        matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDesc.MiscFlags = 0;
        matrixBufferDesc.StructureByteStride = 0;
    }


    // cbuffer를 생성합니다.
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
    if (FAILED(result))
    {
        Console::LogError(L"matrixBuffer, device->CreateBuffer() is failed.", __FILE__, __LINE__);
        return;
    }


    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;
}

void Shader::loadPixelShader(const std::wstring& filePath)
{
    HRESULT     result;
    ID3D10Blob* pixelShaderBuffer;
    ID3D10Blob* errorMessage;


    // 픽셀 쉐이더를 컴파일합니다.
    result = D3DCompileFromFile(filePath.c_str(), NULL, NULL, "LightPixelShader", "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        if (errorMessage)
        {
            Console::LogError(readShaderErrorMsg(errorMessage), __FILE__, __LINE__);
        }
        else
        {
            Console::LogError(L"Missing Shader File.", __FILE__, __LINE__);
        }

        return;
    }


    // 픽셀 쉐이더를 생성합니다.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
    if (FAILED(result))
    {
        Console::LogError(L"device->CreatePixelShader() is failed.", __FILE__, __LINE__);
        return;
    }


    // 텍스처를 어떻게 넘겨줄건지 설정합니다.
    D3D11_SAMPLER_DESC samplerDesc;
    {
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
    }


    // 텍스처 설정을 생성합니다.
    result = device->CreateSamplerState(&samplerDesc, &sampleState);
    if (FAILED(result))
    {
        Console::LogError(L"device->CreateSamplerState() is failed.", __FILE__, __LINE__);
        return;
    }


    // diffuse, direction이 담긴 cbuffer를 설정합니다.
    D3D11_BUFFER_DESC lightBufferDesc;
    {
        lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        lightBufferDesc.ByteWidth = sizeof(LightType);
        lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        lightBufferDesc.MiscFlags = 0;
        lightBufferDesc.StructureByteStride = 0;
    }


    // cbuffer를 생성합니다.
    result = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
    if (FAILED(result))
    {
        Console::LogError(L"lightBuffer, device->CreateBuffer() is failed.", __FILE__, __LINE__);
        return;
    }


    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;
}

void Shader::Render()
{
}

void Shader::Release()
{
    if (vertexShader)
    {
        vertexShader->Release();
        vertexShader = nullptr;
    }

    if (pixelShader)
    {
        pixelShader->Release();
        pixelShader = nullptr;
    }

    if (layout)
    {
        layout->Release();
        layout = nullptr;
    }

    if (sampleState)
    {
        sampleState->Release();
        sampleState = nullptr;
    }

    if (matrixBuffer)
    {
        matrixBuffer->Release();
        matrixBuffer = nullptr;
    }

    if (lightBuffer)
    {
        lightBuffer->Release();
        lightBuffer = nullptr;
    }
}

void Shader::SetShaderParams()
{
}

std::wstring Shader::readShaderErrorMsg(ID3D10Blob* errorMessage)
{
    // Get the length of the message.
    unsigned __int64 bufferSize = errorMessage->GetBufferSize();

    // Allocate memory for the error text.
    char* compileErrors = new char[bufferSize + 1];

    // Get a pointer to the error message text buffer.
    memcpy(compileErrors, errorMessage->GetBufferPointer(), bufferSize);
    compileErrors[bufferSize] = '\0'; // Ensure null termination for safe conversion.

    // Convert the char* to wstring
    std::string errorText(compileErrors);

    return Utls::ToWString(errorText);
}
