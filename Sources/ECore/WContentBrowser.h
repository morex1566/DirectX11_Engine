#pragma once
#include "OWidget.h"

class OWorld;

class WContentBrowser : public OWidget
{
public:
	WContentBrowser(const OWorld& InWorld);
	WContentBrowser(const WContentBrowser&)						= default;
	WContentBrowser& operator=(const WContentBrowser&)			= default;
	WContentBrowser(WContentBrowser&&) noexcept					= default;
	WContentBrowser& operator=(WContentBrowser&&) noexcept		= default;
	~WContentBrowser() override;

	EHandleResultType											Initialize() override;
	void														Release() override;
	/**
	 * \brief Called only once before entering the main loop.
	 */
	void														Start() override;
	/**
	 * \brief Called once when the every frame.
	 */
	void														Tick() override;
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	void														End() override;

private:
	void														Render() override;
};

