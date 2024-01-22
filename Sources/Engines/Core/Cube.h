#pragma once

#include "GameObject.h"

class Cube : public GameObject
{
public:
	Cube();
	Cube(const Cube&)						= default;
	Cube& operator=(const Cube&)			= default;
	Cube(Cube&&) noexcept					= default;
	Cube& operator=(Cube&&) noexcept		= default;
	virtual ~Cube() override  				= default;


public:
	void Start() override;
	void Update() override;
	void Shutdown() override;
};

