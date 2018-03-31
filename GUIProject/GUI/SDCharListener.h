#pragma once
#include"baseEvent.h"
class App;
class SDCharListener :public AnimUI::CharEventListener
{
public:
	App*dest;
	SDCharListener(App* app) {
		dest = app;
		needAutoDelete = false;
	}
	void OnChar(AnimUI::CharEvent&e)override;
};
