#pragma once
#include "Object.h"
#include "OGameObject.h"

class OWorld : public Object
{
public:
	OWorld()												= default;
	OWorld(const OWorld&)									= default;
	OWorld& operator=(const OWorld&)						= default;
	OWorld(OWorld&&) noexcept								= default;
	OWorld& operator=(OWorld&&) noexcept					= default;
	~OWorld() override;

	EHandleResultType										Initialize() override;
	void													Release() override;

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

protected:

private:
	/**
	 * \brief All of gameobject in world are here.
	 */
	std::vector<std::shared_ptr<OGameObject>>				GameObjects;
};

template <typename T, typename... Args>
T& OWorld::TCreateGameObject(Args&&... Arguments)
{
	std::shared_ptr<T> TGameObject = std::make_shared<T>(std::move(Arguments)...);
	GameObjects.push_back(TGameObject);

	return *TGameObject;
}
