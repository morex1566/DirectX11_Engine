#pragma once
class Management
{
public:
	Management();
	Management(const Management&)						= default;
	Management& operator=(const Management&)			= default;
	Management(Management&&) noexcept					= default;
	Management& operator=(Management&&) noexcept		= default;
	virtual ~Management();

	bool				CheckIsEnabled() const;
	bool				CheckIsActivated() const;

	void				SetActive(bool toggle_);

protected:
	bool				_isEnabled;
	bool				_isActivated;

};

