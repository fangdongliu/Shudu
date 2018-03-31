#include<Windows.h>
#include "App.h"
#include"AnimUI.h"
#include"uiwindow.h"
#include"resource.h"
#include"uitable.h"
#include"uitext.h"
#include<algorithm>
#include"uilinearcontainer.h"
#include"resource.h"
using namespace D2D1;
using namespace AnimUI;


class MouseListenerThis:public MouseEventListener {
public:
	App *dest;
	void(*f)(MouseListenerThis* dest, MouseEvent&e);
	MouseListenerThis(App* app,void(*f_)(MouseListenerThis* dest, MouseEvent&e)) {
		dest = app;
		f = f_;
	}
	void OnMouse(MouseEvent&e)override {
		f(this,e);
	}
	void OnMouseOut(UIElement*e)override {

	}
};

inline D2D1_COLOR_F randColor() {
	return ColorF((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
}
App::App():charListener(this)
{
	InitFDUI(LoadIcon(HINST_THISCOMPONET,MAKEINTRESOURCE(IDI_ICON1)));
}


App::~App()
{
	ReleaseFDUI();
}

void App::NewGame() {
	
	if (sd.mapQueue.size() > 0) {
		memset(editable, 0, sizeof(editable));
		bool temp[63] = { 0 };
		for (int i = 0; i < 30; i++) {
			temp[i] = 1;
		}
		for (int i = 0; i < 1000; i++) {
			std::swap(temp[rand()%63], temp[rand()%63]);
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				bool temp2[9] = { 1,1 ,0};
				memcpy(temp2 + 2, temp+(i*3+j)*7, 7);
				for (int k = 0; k < 50; k++) {
					std::swap(temp2[rand() % 9], temp2[rand() % 9]);
				}
				for (int x = 0; x < 3; x++) {
					for (int y = 0; y < 3; y++) {
						editable[i * 3 + x][j * 3 + y] = temp2[x * 3 + y];
					}
				}
			}
		}
		map = sd.mapQueue.front();
		sd.mapQueue.pop();
		wchar_t text[20] = { 0 };
		auto table = (UITable*)((UIContainer*)current)->childs[0];
		for (int i = 0; i < 81; i++) {
			if (editable[i / 9][i % 9]) {
				text[0] = ' ';
				if (i % 2) {
					table->childs[i]->SetStateStyle(&helper.editableTextStyleEven);
				}
				else {
					table->childs[i]->SetStateStyle(&helper.editableTextStyleOdd);
				}
				table->childs[i]->isFocusable = true;
			}
			else {
				text[0] = map.map[i / 9][i % 9] + '1';
				if (i % 2) {
					table->childs[i]->SetStateStyle(&helper.notEditableTextStyleEven);
				}
				else {
					table->childs[i]->SetStateStyle(&helper.notEditableTextStyleOdd);
				}
				table->childs[i]->isFocusable = false;
			}
			auto t = (UIText*)table->childs[i];
			t->contentText = text;
			t->needMeasure = true;
		}
		table->NeedUpdate();

	}
	else {
		sd.GenerateMap(1);
		NewGame();
	}
}
void App::Judge() {
	auto table = (UITable*)((UIContainer*)current)->childs[0];
	FD::SDMap map;
	for (int i = 0; i < 81; i++) {
		if (!map.TrySetElement(i / 9, i % 9, ((UIText*)table->childs[i])->contentText[0] - '1')) {
			Failed();
			return;
		}
	}
	Win();
}

void App::Win() {
	auto element=((UIContainer*)current)->childs[3];
	auto text=(UIText*)((UIContainer*)element)->childs[0];
	text->contentText = L"解题成功";
	text->currentStyle.SetTextColor(D2D1::ColorF(ColorF::LightGreen));
	text->NeedUpdate();
	element->Show();
}

void App::Failed() {
	static int animId = 0;
	auto element = ((UIContainer*)current)->childs[1];
	element->Show();
	auto anim = destWindow->RequestAnimation(element, &animId);
	auto &style = element->currentStyle;
	anim->SetDuration(1.0F);
	anim->Add<D2D1_COLOR_F>(style.background.backgroundColor, ColorF(0xff0000), ColorF(0xff0000, 0.0));
	anim->animationEndCallBack = [](Animation*an) {
		an->GetDest()->Hidden();
	};
}

UIElement* App::CreatePage(UIWindow*window) {

	auto table = new UITable;

	auto stateStyle = new UIStateStyle;
	auto normalStyle = new UIStyle;
	auto captureStyle = new UIStyle;
	auto page = Page::CreateDefaultPage();
	AnimUI::RECTF rect = { 90,40,90,40 };
	table->currentStyle.SetMargin(rect);
	table->currentStyle.SetTop(40);
	table->currentStyle.SetLeft(90);
	AnimUI::UIBorder border = { D2D1::ColorF(0,0.3f),3.0f,1,1,1,1 };

	table->currentStyle.SetBorder(border);
	SizeCaculator sizeCaculator(0, 0, 1.f, 1.f, 0);
	table->currentStyle.SetSizeCaculator(sizeCaculator);

	{
		UITransform transform;
		transform.rotateAngle = 0;
		transform.rotateCenter = D2D1::Point2F(0.5, 0.5);
		transform.transform = D2D1::SizeF(0, 0);
		{
			
			normalStyle->SetSizeCaculator(sizeCaculator);
			transform.scale = D2D1::SizeF(1, 1);
			normalStyle->SetTransform(transform);
			std::wstring imageName(L"pic2.jpg");
			normalStyle->SetBackgroundImage(imageName);
			normalStyle->SetBorderRadius(20.0f);
			normalStyle->SetDuration(1);
			normalStyle->SetOpacity(1.0f);
		}
		{
			captureStyle->SetSizeCaculator(sizeCaculator);
			transform.scale = D2D1::SizeF(1.2, 1.2);
			captureStyle->SetTransform(transform);
			captureStyle->SetDuration(1);
			captureStyle->SetOpacity(0.0f);
			std::wstring imageName(L"pic2.jpg");
			captureStyle->SetBackgroundImage(imageName);
			captureStyle->SetBorderRadius(20.0f);
		}
	}
	stateStyle->SetState(UIStateCapture, captureStyle);
	stateStyle->SetState(UIStateNormal, normalStyle);

	page->SetStateStyle(stateStyle);
	page->currentStyle.SafeCopy(captureStyle);

	page->mouseEventListener = new NoParamMouseListener([](MouseEvent&e)->void {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonDown && !e.GetEventHandledId()) {
			ReleaseCapture();
			e.EventHandled();
			e.GetDestWindow()->SetCaptureElement(0);
			SendMessage(e.GetDestWindow()->GetHWnd(), WM_NCLBUTTONDOWN, HTCAPTION, 0);
		}
	});

	wchar_t buf[20];
	buf[1] = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			auto child = new UIText;
			child->isFocusable = true;
			child->charEventListener = &charListener;
			buf[0] = L'?';
			child->contentText = buf;
			child->textAlignVertical = TextAlignCenter;
			child->textAlignHorizontal = TextAlignCenter;
			
			child->SetStateStyle(
				(i*9+j)%2==0?&helper.editableTextStyleEven:
				&helper.editableTextStyleOdd);

			table->AddChild(child);
		}
		table->EndLine();
	}
	auto& childs = ((UIContainer*)page)->childs;
	childs.insert(childs.begin(), table);
	table->SetParent(page);

	auto dialog = CreateAppDialog(window);
	dialog->currentStyle.SetLeft(150);
	dialog->currentStyle.SetTop(175);
	childs.insert(childs.begin() + 1, dialog);
	dialog->SetParent(page);
	((UIContainer*)dialog)->childs[2]->mouseEventListener = new MouseListenerThis(this, [](MouseListenerThis* dest,MouseEvent&e) {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonUp) {
			if (e.GetDestElement() == e.GetDestWindow()->GetCaptureElement()) {
				dest->dest->NewGame();
				e.GetDestElement()->GetParent()->Hidden();
			}
		}
	});

	auto judgebtn = helper.CreateJudgeBtn();
	judgebtn->mouseEventListener = new MouseListenerThis(this, [](MouseListenerThis* dest, MouseEvent&e) {
		if (e.GetEventType() == MouseEventType::MouseEventLButtonUp) {
			if (e.GetDestElement() == e.GetDestWindow()->GetCaptureElement()) {
				dest->dest->Judge();
			
			}
		}
	});
	childs.insert(childs.begin() + 1, judgebtn);

	auto element = new UIElement;
	auto&style = element->currentStyle;
	style.SetMargin(rect);
	style.SetTop(40);
	style.SetLeft(90);
	style.SetSizeCaculator(sizeCaculator);
	style.SetBackgroundColor(ColorF(0xff0000));
	childs.insert(childs.begin() + 1, element);
	element->Hidden();
	window->SetCurrentPage(page,false);
	int animId = 0;
	auto anim = window->RequestAnimation(page, &animId);
	anim->SetDuration(1.0F);
	page->currentStyle.AddToAnimation(anim, normalStyle);
	current = page;


	return page;
}

void App::Run() {
	MessageLoop();
}

UIElement* App::CreateAppDialog(UIWindow*window) {
	auto layout = new UILinearContainer;
	auto &style = layout->currentStyle;
	SizeCaculator size(400, 250, 0, 0, 0);
	style.SetSizeCaculator(size);
	style.SetBorderRadius(30.0f);
	AnimUI::RECTF padding = { 100,0,100,0 };
	style.SetPadding(padding);
	style.SetBackgroundColor(ColorF(ColorF::White,0.7));
	UIBorder border = { ColorF(0,0.3),10.0f,1,1,1,1 };
	style.SetBorder(border);
	auto text = new UIText;
	SizeCaculator size3(0, 150, 1, 0, 0);
	text->currentStyle.SetSizeCaculator(size3);
	text->textAlignHorizontal = TextAlignCenter;
	text->textAlignVertical = TextAlignCenter;
	text->currentStyle.SetFontSize(40.0f);
	text->contentText = L"新的开始";
	auto button = new UIText;
	auto normalStyle = new UIStyle;
	normalStyle->SetDuration(0.5f);
	button->contentText = L"开始游戏";
	button->textAlignHorizontal = TextAlignCenter;
	button->textAlignVertical = TextAlignCenter;
	normalStyle->SetBorderRadius(20.0f);
	SizeCaculator size2(0, 50, 1, 0, 0);
	normalStyle->SetSizeCaculator(size2);
	normalStyle->SetBackgroundColor(ColorF(ColorF::LightBlue));
	auto hoverStyle = new UIStyle;
	hoverStyle->SetBorderRadius(20.0f);
	hoverStyle->SetSizeCaculator(size2);
	hoverStyle->SetBackgroundColor(ColorF(ColorF::Gray));
	hoverStyle->SetTextColor(ColorF(ColorF::White));

	auto captureStyle = new UIStyle;
	captureStyle->SetBorderRadius(20.0f);
	captureStyle->SetSizeCaculator(size2);
	captureStyle->SetBackgroundColor(ColorF(ColorF::Black));
	captureStyle->SetTextColor(ColorF(ColorF::White));

	auto stateStyle = new UIStateStyle;
	stateStyle->SetState(UIStateHover, hoverStyle);
	stateStyle->SetState(UIStateNormal, normalStyle);
	stateStyle->SetState(UIStateCapture, captureStyle);
	button->SetStateStyle(stateStyle);
	layout->AddChild(text);

	text = new UIText;
	SizeCaculator size4(0, 50, 1, 0, 0);
	text->currentStyle.SetSizeCaculator(size4);
	RECTF margin = { 0,-80,0,0 };
	text->currentStyle.SetMargin(margin);
	text->textAlignHorizontal = TextAlignCenter;
	text->textAlignVertical = TextAlignCenter;
	text->currentStyle.SetFontSize(20.0f);
	text->currentStyle.SetTextColor(D2D1::ColorF(0x999999));
	text->contentText = L"(数字键输入)";
	layout->AddChild(text);
	layout->AddChild(button);
	return layout;
}

bool App::Init() {
	
	auto window = UIWindow::CreateMainWindow(0, 0, 700, 600, true);
	destWindow = window;
	window->Center();
	CreatePage(window);
	
	window->Show();

	return true;
}