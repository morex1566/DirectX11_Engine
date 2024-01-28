#pragma once
#include "Object.h"
#include "OGameObject.h"

class GCamera;
class OWindow;
class ODirectX11;

class OWorld : public Object
{
public:
	OWorld();
	OWorld(const OWorld&)									= default;
	OWorld& operator=(const OWorld&)						= default;
	OWorld(OWorld&&) noexcept								= default;
	OWorld& operator=(OWorld&&) noexcept					= default;
	~OWorld() override;

	void													Init() override;
	void													Shutdown() override;

	/**
	 * \brief Called only once before entering the main loop.
	 */
	void													Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void													Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void													End() override;

	template <typename T, typename... Args>
	T&														TCreateGameObject(Args&&... Arguments);
	/**
	 * \brief 
	 * \return World's game object vector. 
	 */
	const std::vector<std::shared_ptr<OGameObject>>&		GetGameObjects() const { return GameObjects; }

private:
	/**
	 * \brief All of gameobject in world are here.
	 */
	std::vector<std::shared_ptr<OGameObject>>				GameObjects;
};

template <typename T, typename... Args>
T& OWorld::TCreateGameObject(Args&&... Arguments)
{
	// Create and attach.
	std::shared_ptr<T> TGameObject = std::make_shared<T>(std::move(Arguments)...);
	GameObjects.push_back(TGameObject);

	// name as default.
	bool IsDone = false;
	int Index = 0;
	while (!IsDone)
	{
		bool Found = false;

		// Find same name.
		// When is found, raise the index.
		for (const auto& GameObject : GameObjects)
		{
			if (GameObject->GetName() == ToWString(GetTypeToString<T>() + std::to_string(Index)))
			{
				Found = true;
				Index++;
				break;
			}
		}

		// Not found, set current index to name.
		if (!Found)
		{
			std::static_pointer_cast<Object>(TGameObject)->SetName(ToWString(GetTypeToString<T>() + std::to_string(Index)));
			IsDone = true;
		}
	}

	return *TGameObject;
}
