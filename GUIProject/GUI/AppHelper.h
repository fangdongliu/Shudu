#pragma once
#include"UIStyle.h"
#include"UIElement.h"
#define DECLARE_STYLE auto stateStyle = new UIStateStyle;auto normalStyle = new UIStyle;auto hoverStyle = new UIStyle;
class Helper
{
public:
	Helper();
	~Helper();
	AnimUI::UIStateStyle editableTextStyleOdd;
	AnimUI::UIStateStyle notEditableTextStyleOdd;
	AnimUI::UIStateStyle editableTextStyleEven;
	AnimUI::UIStateStyle notEditableTextStyleEven;
	AnimUI::UIElement* CreateJudgeBtn();
};

