#pragma once

class Resource
{
	friend class ResourceManager;

public:
	Resource();
	virtual ~Resource();

	virtual void Destroy() = 0;

	bool IsDestroyed();
	bool IsActivated();

protected:
	bool _isDestroyed;
	bool _isActivated;

};