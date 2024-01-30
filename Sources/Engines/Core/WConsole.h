#pragma once
#include "OWidget.h"

class OWorld;

class WConsole : public OWidget
{
public:
	WConsole();
	WConsole(const WConsole&)									= default;
	WConsole& operator=(const WConsole&)						= default;
	WConsole(WConsole&&) noexcept								= default;
	WConsole& operator=(WConsole&&) noexcept					= default;
	~WConsole() override;

public:
	void			Init() override {}
	void			Shutdown() override {}
	void			Start() override {}
	void			Tick() override {}
	void			End() override {}


public:
	void			Render() override;


private:
	const OWorld*	World;


};

