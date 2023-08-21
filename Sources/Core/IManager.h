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

	template <class C, class = IsBaseOf<C, T>, typename... Args>
	std::shared_ptr<C>		Create(Args&&... args_);
	template <typename... Args>
	std::shared_ptr<T>		Create(Args&&... args_);

	void								Dispose();
	
protected:
	std::vector<std::shared_ptr<T>>		_objects;
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
	for (auto it = _objects.begin(); it != _objects.end();)
	{
		it->reset();
		it = _objects.erase(it);
	}
}

template <class T, class T0>
template <class C, class, typename ... Args>
std::shared_ptr<C> IManager<T, T0>::Create(Args&&... args_)
{
	std::shared_ptr<C> object = std::make_shared<C>(std::forward<Args>(args_)...);
	_objects.push_back(object);

	return object;
}

template <class T, class T0>
template <typename ... Args>
std::shared_ptr<T> IManager<T, T0>::Create(Args&&... args_)
{
	std::shared_ptr<T> object = std::make_shared<T>(std::forward<Args>(args_)...);
	_objects.push_back(object);

	return object;
}

template <class T, class T0>
void IManager<T, T0>::Dispose()
{
	for (auto it = _objects.begin(); it != _objects.end();)
	{
		IManageable* current = it->get();

		if (current->IsDestroyed())
		{
			it->reset();
			it = _objects.erase(it);
		}
		else
		{
			++it;
		}
	}
}
