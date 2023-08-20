#pragma once

class Scene : public IManageable
{
public:
	std::string GetName();
	void SetName(const std::string& name_);

private:
	std::string _name;
};

