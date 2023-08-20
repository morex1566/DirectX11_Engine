#pragma once

class IManageable
{
public:
	IManageable();
	virtual ~IManageable();

	virtual void		Destroy();
	void				Disable();
	bool				IsDestroyed();
	bool				IsDisabled();

private:
	bool _isEnabled;
	bool _isDestroyed;
};