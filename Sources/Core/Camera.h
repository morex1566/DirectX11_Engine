#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera()									= default;
	Camera(const Camera&)						= default;
	Camera& operator=(const Camera&)			= default;
	Camera(Camera&&) noexcept					= default;
	Camera& operator=(Camera&&) noexcept		= default;
	~Camera() override;

	void Start() override;
	void Update() override;

	XMMATRIX									GetViewMatrix();

private:
	XMMATRIX									_viewMatrix;
};

