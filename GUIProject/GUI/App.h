#pragma once
#include"SD.h"
#include"SDCharListener.h"
#include"AppHelper.h"
#include<wincodec.h>
namespace AnimUI {
	class UIElement;
	class UIWindow;
}
class App
{
public:
	App();
	~App();
	AnimUI::UIElement*current;
	AnimUI::UIWindow*destWindow;
	FD::SDMap map;
	FD::SD sd;
	SDCharListener charListener;
	bool editable[9][9];
	Helper helper;
public:
	void NewGame();
	void Win();
	void Failed();
	void Judge();
	bool Init();
	AnimUI::UIElement* CreateAppDialog(AnimUI::UIWindow*);
	AnimUI::UIElement* CreatePage(AnimUI::UIWindow*);
	void Run();
};

