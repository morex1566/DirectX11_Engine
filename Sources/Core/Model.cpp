#include "pch.h"
#include "Model.h"

#include "Light.h"

void Model::Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, HWND hWnd_,
                       const std::string& meshFilePath_, const std::string& textureFilePath_, const std::string& vsFilePath_, const std::string& psFilePath_)
{
	// Clear all of member before initialization.
	ClearMemory();

	// Initialize member variables.
	{
		_device = device_;
		_deviceContext = deviceContext_;
		_hWnd = hWnd_;
	}

	// Load datas from files.
	{
		LoadMesh(meshFilePath_);
		LoadTexture(textureFilePath_);
		LoadShader(vsFilePath_, psFilePath_);
	}
}

void Model::Render(const XMMATRIX& worldMatrix_, const XMMATRIX& viewMatrix_, const XMMATRIX& projectionMatrix_, Light* light_)
{
	for (auto& mesh : _meshes)
	{
		mesh.SendBufferToAssembly();

		_shader->Render(_deviceContext,
						_indexCount,
						worldMatrix_, viewMatrix_, projectionMatrix_,
						_texture->GetShaderResourceView(),
						light_->GetDirection(),
						light_->GetDiffuseColor()
		);

	}
}

void Model::ClearMemory()
{
}

void Model::ClearMeshes()
{
}

void Model::ClearTexture()
{
}

void Model::ClearShader()
{
}

std::vector<Mesh> Model::GetMeshes()
{
	return _meshes;
}

Texture* Model::GetTexture()
{
	return _texture.get();
}

Shader* Model::GetShader()
{
	return _shader.get();
}

void Model::LoadMesh(const std::string& filePath_)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filePath_, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::LoadTexture(const std::string& filePath_)
{
	Texture* texture = new Texture;

	texture->Initialize(_device, _deviceContext, filePath_);

	_texture = std::make_unique<Texture>(*texture);
}

void Model::LoadShader(const std::string& vsFilePath_, const std::string& psFilePath_)
{
	Shader* shader = new Shader;

	shader->Initialize(_device, _hWnd);

	_shader = std::make_unique<Shader>(*shader);
}

void Model::processNode(const aiNode* node, const aiScene* scene)
{
	for (uint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.emplace_back(processMesh(mesh, node));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(const aiMesh* mesh, const aiNode* node)
{
	vector<VertexType>		vertices;
	vector<DWORD>			indices;

	// Get vertices.
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		VertexType vertexType;
		{
			// Set positions.
			vertexType.position.x = mesh->mVertices[i].x;
			vertexType.position.y = mesh->mVertices[i].y;
			vertexType.position.z = mesh->mVertices[i].z;

			// Set texture coordinates.
			// Index '0' means that the main texture of model.
			// If the model has another textures, ex. normal textures..., change the index.
			if (mesh->mTextureCoords[0])
			{
				vertexType.texture.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
				vertexType.texture.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
			}

			// Set normals.
			vertexType.normal.x = mesh->mNormals[i].x;
			vertexType.normal.y = mesh->mNormals[i].y;
			vertexType.normal.z = mesh->mNormals[i].z;
		}

		vertices.emplace_back(vertexType);
		_vertexCount++;
	}

	// Get indices.
	for (uint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (uint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
			_indexCount++;
		}
	}

	return Mesh(_device, _deviceContext, vertices, indices);
}
