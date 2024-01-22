#include "PCH.h"
#include "Cube.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

Cube::Cube(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: GameObject()
{
	Mesh* mesh = new Mesh(device, deviceContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	{
		mesh->Load(RESOURCE_FILE_PATH("cube.txt"));
	}

	AttachComponent(mesh);


	Texture* texture = new Texture(device, deviceContext);
	{
		texture->Load(RESOURCE_FILE_PATH("seafloor.dds"), TextureType::DDS);
	}

	AttachComponent(texture);


	Shader* shader = new Shader(device, deviceContext);
	{
		shader->Load(SHADER_FILE_PATH("light.vs"), SHADER_FILE_PATH("light.ps"));
	}

	AttachComponent(shader);


}

void Cube::Start()
{
	GameObject::Start();
}

void Cube::Update()
{
	GameObject::Update();
}

void Cube::Shutdown()
{
	GameObject::Shutdown();
}
