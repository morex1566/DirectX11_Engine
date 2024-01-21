#pragma once

#include "Component.h"

class Texture : public Component
{
public:
	Texture();
	Texture(const Texture&)						= default;
	Texture& operator=(const Texture&)			= default;
	Texture(Texture&&) noexcept					= default;
	Texture& operator=(Texture&&) noexcept		= default;
	~Texture() override  						= default;


public:
	void Shutdown();
	void LoadTexture();
	void ReleaseTexture();
};

