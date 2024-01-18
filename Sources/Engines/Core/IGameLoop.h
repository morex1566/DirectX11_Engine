#pragma once

class IGameLoop
{
public:
	IGameLoop()				= default;
	virtual ~IGameLoop()	= default;

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
};