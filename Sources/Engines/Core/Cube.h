#pragma once

#include "GameObject.h"

class Cube : public GameObject
{
public:
	Cube(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Cube(const Cube&)						= default;
	Cube& operator=(const Cube&)			= default;
	Cube(Cube&&) noexcept					= default;
	Cube& operator=(Cube&&) noexcept		= default;
	~Cube() override  						= default;


public:
	void Start() override;
	void Update() override;
	void Shutdown() override;


};

