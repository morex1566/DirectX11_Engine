#pragma once

template <class T>
class ISingleton
{
public:
    ISingleton(const ISingleton&)               = delete;
    ISingleton(ISingleton&&)                    = delete;
    ISingleton& operator=(const ISingleton&)    = delete;
    ISingleton& operator=(ISingleton&&)         = delete;
    virtual ~ISingleton()                       = default;

    static T& GetInstance();

protected:
	ISingleton() = default;
};

template <class T>
T& ISingleton<T>::GetInstance()
{
    static T instance;
    return instance;
}