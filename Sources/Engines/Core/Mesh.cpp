#include "PCH.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY primitiveType)
    : device(device), deviceContext(deviceContext), primitiveType(primitiveType)
{
}

void Mesh::Start()
{
}

void Mesh::Update()
{
}

void Mesh::Shutdown()
{
    Release();
}

void Mesh::Render()
{
    unsigned int stride;
    unsigned int offset;


    // Set vertex buffer stride and offset.
    stride = sizeof(Vertex);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(primitiveType);
}

void Mesh::Load(const std::wstring& Filename)
{
    Release();

    std::ifstream fin;
    char input;
    int i;


    // Open the model file.
    fin.open(Filename);

    // If it could not open the file then exit.
    if (fin.fail())
    {
        Console::LogWarning(L"Fail to load mesh.", __FILE__, __LINE__);
        return;
    }

    // Read up to the value of vertex count.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }

    // Read in the vertex count.
    fin >> vertexCount;

    // Set the number of indices to be the same as the vertex count.
    indexCount = vertexCount;

    // Create the model using the vertex count that was read in.
    vertices = new Vertex[vertexCount];
    indices = new unsigned long[indexCount];

    // Read up to the beginning of the data.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // Read in the vertex data.
    for (i = 0; i < vertexCount; i++)
    {
        fin >> vertices[i].position.x >> vertices[i].position.y >> vertices[i].position.z;
        fin >> vertices[i].texture.x >> vertices[i].texture.y;
        fin >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;

        indices[i] = i;
    }

    // Close the model file.
    fin.close();

    createBuffer();
}

void Mesh::Release()
{
    if (vertexBuffer)
    {
        vertexBuffer->Release();
        vertexBuffer = nullptr;
    }
    
    if (indexBuffer)
    {
        indexBuffer->Release();
        indexBuffer = nullptr;
    }

    if (vertices)
    {
        delete[] vertices;
        vertices = nullptr;
    }

    if (indices)
    {
        delete[] indices;
        indices = nullptr;
    }
}

void Mesh::createBuffer()
{
    HRESULT result;

    // 버택스 버퍼의 설정
    D3D11_BUFFER_DESC vertexBufferDesc;
    {
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

        // Set up the description of the static vertex buffer.
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;
    }


    // 버택스 데이터 설정
    D3D11_SUBRESOURCE_DATA vertexData;
    {
        // Give the subresource structure a pointer to the vertex data.
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;
    }


    // 버택스 버퍼 생성
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(result))
    {
        Console::LogWarning(L"create vertex buffer is failed.", __FILE__, __LINE__);
        return;
    }


    // 인덱스 버퍼의 설정
    D3D11_BUFFER_DESC indexBufferDesc;
    {
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

        // Set up the description of the static index buffer.
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;
    }


    // 인덱스 데이터 설정
    D3D11_SUBRESOURCE_DATA indexData;
    {
        // Give the subresource structure a pointer to the index data.
        indexData.pSysMem = indices;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;
    }


    // 인덱스 버퍼 생성
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    if (FAILED(result))
    {
        Console::LogWarning(L"create index buffer is failed.", __FILE__, __LINE__);
        return;
    }
}
