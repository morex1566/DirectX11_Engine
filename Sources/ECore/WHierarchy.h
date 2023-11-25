#pragma once
#include "OWidget.h"

class OGameObject;
class OWorld;
class WInspector;

class WHierarchy : public OWidget
{
public:
	WHierarchy(const OWorld& InWorld, WInspector* InInspector);
	WHierarchy(const WHierarchy&)							= default;
	WHierarchy& operator=(const WHierarchy&)				= default;
	WHierarchy(WHierarchy&&) noexcept						= default;
	WHierarchy& operator=(WHierarchy&&) noexcept			= default;
	~WHierarchy() override;

	void													Initialize() override;
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

	void													Render() override;

	const OWorld*											World; // ReadOnly
	WInspector*												Inspector;
	/**
	 * \brief OGameObject* means game object in world, uint8 means is selection flag at hierarchy.
	 */
	std::map<OGameObject*, uint8>							GameObjectBlocks;
};

