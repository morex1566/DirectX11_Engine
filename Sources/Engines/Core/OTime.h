#pragma once
#include "Object.h"

#include <chrono>

class OTime : public Object
{
public:
	OTime();
	OTime(const OTime&)								= default;
	OTime& operator=(const OTime&)					= default;
	OTime(OTime&&) noexcept							= default;
	OTime& operator=(OTime&&) noexcept				= default;
	~OTime() override;

	void											Init() override;
	void											Shutdown() override;
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void											Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void											Tick() override;

	FORCEINLINE static float						GetDeltaTime() { return DeltaTime; }

private:
	static float									DeltaTime;
	std::chrono::steady_clock::time_point			PrevTime;
};

