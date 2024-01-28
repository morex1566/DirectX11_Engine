#include "PCH.h"
#include "SApplication.h"
#include "OWidget.h"


OWidget::OWidget()
	: Object()
{
}

OWidget::~OWidget()
{
}

void OWidget::Init()
{
	Object::Init();
}

void OWidget::Shutdown()
{
	Object::Shutdown();
}

void OWidget::Start()
{
	Object::Start();
}

void OWidget::Tick()
{
	Object::Tick();

	Render();
} 

void OWidget::End()
{
	Object::End();
}