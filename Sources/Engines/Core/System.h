#pragma once
#include "Window.h"

class System : public IGameLoop
{
public:
	System(HINSTANCE hInstance);
	System(const System&)						= delete;
	System& operator=(const System&)			= delete;
	System(System&&) noexcept					= delete;
	System& operator=(System&&) noexcept		= delete;
	~System() override							= default;

	void Init() override;
	void Update() override;
	void Shutdown() override;

private:
	std::shared_ptr<Window>						window;
	HINSTANCE									hInstance;
};

