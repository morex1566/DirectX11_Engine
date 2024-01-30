#pragma once
#include "Object.h"
#include "OWidget.h"

class ODirectX11;
class OWindow;
class OWorld;

class OGUI : public Object
{
public:
	OGUI(const OWindow& InWindow, const ODirectX11& InDirectX11, const OWorld& InWorld);
	OGUI(const OGUI&)										= default;
	OGUI& operator=(const OGUI&)							= default;
	OGUI(OGUI&&) noexcept									= default;
	OGUI& operator=(OGUI&&) noexcept						= default;
	~OGUI() override;


public:
	static void CALLBACK	MessageHandler(HWND InHWnd, UINT InMsg, WPARAM InWParam, LPARAM InLParam);


public:
	void					Init() override;
	void					Shutdown() override;
	void					Start() override;
	void					Tick() override;
	void					End() override;


public:
	template <typename T, typename ...Args>
	T*						TAddWidget(Args&&... InConstructorArgs);
	template <typename T>
	void					TDeleteComponent_Deprecated();


private:
	const OWindow*							Window; // Readonly
	const OWorld*							World; // Readonly
	const ODirectX11*						DirectX11; // Readonly
	std::vector<std::shared_ptr<OWidget>>	Widgets;

};

template <typename T, typename ... Args>
T* OGUI::TAddWidget(Args&&... InConstructorArgs)
{
	Widgets.emplace_back(std::make_shared<T>(std::move(InConstructorArgs)...));

	return static_cast<T*>(Widgets.back().get());
}

template <typename T>
void OGUI::TDeleteComponent_Deprecated()
{
	auto it = Widgets.begin();
	while (it != Widgets.end())
	{
		if (T* TComponent = dynamic_cast<T*>(it->get()))
		{
			it->reset();
			it = Widgets.erase(it);

			break;
		}
		else
		{
			++it;
		}
	}
}
