#pragma once
#include"UIRelativeContainer.h"
namespace AnimUI {
	void WriteMessageToLogFile(const char*  message);
	MouseEventListener* PosMeasureListener();
	
	class LinesPainter :public CanvasPainter {
	public:
		LinesPainter() {
			lineWidth = 1.0f;
			color = D2D1::ColorF(0xffffff);
		}
		void SetColor(D2D1_COLOR_F&color_) { color = color_; };
		void SetWidth(float width) { lineWidth = width; }
		void SetLines(int count, ...);
		std::vector<std::pair<D2D1_POINT_2F, D2D1_POINT_2F>>lineList;
		D2D1_COLOR_F color;
		float lineWidth;
		void Paint(UIElement*element, UIWindow*destWindow)override;
	};
	class NoParamMouseListener :public MouseEventListener {
	public:
		NoParamMouseListener(void(*func_)(MouseEvent&)) {
			func = func_;
		}
		virtual void OnMouse(MouseEvent& mouseEvent) {
			func(mouseEvent);
		};
		virtual void OnMouseOut(UIElement*source) {

		};
		void(*func)(MouseEvent&);
	};
	namespace Page
	{
		UIElement* CreateDefaultPage();
		UIElement* CreateWaitPage();
		UIElement* CreateCloseBtn();
		UIElement* CreateMinimizeBtn();
		UIRelativeContainer* CreateDraggableBackground();
	};
}