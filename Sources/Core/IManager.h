#pragma once

template <class T, class = IsBaseOf<T, IManageable>>
class IManager
{
	friend class IManageable;
public:
	IManager();
	virtual ~IManager();

	virtual void Initialize();
	virtual void Update();
	virtual void ClearMemory();

	template <typename... Args> T*		Create(Args&&... args);
	void								Dispose();
	
protected:
	std::vector<std::shared_ptr<T>> _windows;
};

template <class T, class T0>
IManager<T, T0>::IManager()
{
	IManager<T, T0>::Initialize();
}

template <class T, class T0>
IManager<T, T0>::~IManager()
{
	IManager<T, T0>::ClearMemory();
}

template <class T, class T0>
void IManager<T, T0>::Initialize()
{
}

template <class T, class T0>
void IManager<T, T0>::Update()
{
}

template <class T, class T0>
void IManager<T, T0>::ClearMemory()
{
	for (auto it = _windows.begin(); it != _windows.end();)
	{
		it->reset();
		it = _windows.erase(it);
	}
}

template <class T, class T0>
template <typename... Args>
T* IManager<T, T0>::Create(Args&&... args)
{
	std::shared_ptr<T> object = std::make_shared<T>(std::forward<Args>(args)...);
	_windows.push_back(object);

	return object.get();
}

template <class T, class T0>
void IManager<T, T0>::Dispose()
{
	for (auto it = _windows.begin(); it != _windows.end();)
	{
		IManageable* curr = it->get();

		if (curr->IsDestroyed())
		{
			it->reset();
			it = _windows.erase(it);
		}
		else
		{
			++it;
		}
	}
}
