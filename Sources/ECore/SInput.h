#pragma once
class SInput
{
public:
	SInput(const SInput&)						= delete;
	SInput& operator=(const SInput&)			= delete;
	SInput(SInput&&) noexcept					= delete;
	SInput& operator=(SInput&&) noexcept		= delete;
	~SInput()									= default;

	static SInput&								GetInstance();

	void										Initialize();
	void										Release();
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void										Start();
	/**
	 * \brief Called once when the every frame.
	 */
	void										Tick();
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void										End();
	/**
	 * \brief Clear to '0' all of keys.
	 */
	void										Clear();

	FORCEINLINE	static bool						GetKeyDown(uint8 InKeyCode) { return Keys[InKeyCode]; }
	// TODO : This is quite dangerous 
	FORCEINLINE static void						SetKeyDown(uint8 InKeyCode) { Keys[InKeyCode] = 1; History.push(InKeyCode); }

private:
	SInput()									= default;

	/**
	 * \brief '1' means key down, '0' means key up.
	 */
	static uint8								Keys[256];
	/**
	 * \brief History of pressed key at frame.
	 */
	static std::queue<uint8>					History;
};

