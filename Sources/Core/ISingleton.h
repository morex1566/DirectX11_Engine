// ReSharper disable CppUseAssociativeContains
#pragma once

template <class T>
class ISingleton
{
public:
    ISingleton(const ISingleton&)               = delete;
    ISingleton(ISingleton&&)                    = delete;
    virtual ~ISingleton()                       = default;

    static T& GetInstance();

    template <typename... Args>
    static T& GetInstance(Args... args_);

protected:
	ISingleton() = default;

private:
    static std::unordered_map<std::type_index, std::shared_ptr<T>>     _instances;
};

template <class T>
std::unordered_map<std::type_index, std::shared_ptr<T>> ISingleton<T>::_instances;

template <class T>
T& ISingleton<T>::GetInstance()
{
	std::type_index instanceHash = std::type_index(typeid(T));

	if (_instances.find(instanceHash) != _instances.end())
	{
        return *_instances[instanceHash];
	}
    else
    {
	    std::shared_ptr<T> instance = std::make_shared<T>();
        _instances[instanceHash] = instance;

        return *instance;
    }
}

template <class T>
template <typename... Args>
T& ISingleton<T>::GetInstance(Args... args_)
{
    std::type_index instanceHash = std::type_index(typeid(T));

    if (_instances.find(instanceHash) != _instances.end())
    {
        return *_instances[instanceHash];
    }
    else
    {
        std::shared_ptr<T> instance = std::make_shared<T>(std::forward<Args>(args_)...);
        _instances[instanceHash] = instance;

        return *instance;
    }
}