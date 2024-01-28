#include "PCH.h"
#include "SApplication.h"
#include "OWidget.h"


OWidget::OWidget()
	: Object()
{
}

OWidget::~OWidget()
{
	Shutdown();
}