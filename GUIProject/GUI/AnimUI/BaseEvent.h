#pragma once
#include"AnimUITypes.h"
#include"predef.h"
namespace AnimUI {
	class UIWindow;
	class UIElement;
	enum MouseEventType {
		MouseEventLButtonDown,
		MouseEventLButtonUp,
		MouseEventScroll,
		MouseEventRButtonDown,
		MouseEventRButtonUp,
		
		MouseEventMouseMove,
		MouseEventMButtonDown,
		MouseEventMButtonUp
	};
	class BaseEvent{

	private:
		UIWindow * destWindow;
		UIElement* destination;
		char handledId;

	public:
		BaseEvent(UIWindow*dest) {
			destWindow = dest;
			handledId = false;
		}

		inline void EventHandled(char handleid=1) {
			handledId = handleid;
		}
		inline char GetEventHandledId() {
			return handledId;
		}
		inline void SetDestElement(UIElement*dest){
			destination = dest;
		}

		inline UIElement* GetDestElement() {
			return destination;
		}
		inline UIWindow* GetDestWindow() {
			return destWindow;
		}
		
	};
	class MouseEvent:public BaseEvent {

	private:
		MouseEventType eventType;
		PointF point;
		unsigned int wParam;
	public:
		MouseEvent(UIWindow*dest):BaseEvent(dest){}
		MouseEvent(UIWindow* destwindow,MouseEventType type,float x,float y,unsigned int wparam = 0):BaseEvent(destwindow){
			point = { x,y };
			eventType = type;
			wParam = wparam;
		}
		inline MouseEventType GetEventType() {
			return eventType;
		}
		inline PointF& GetPoint() {
			return point;
		}
		inline unsigned int GetWParam() {
			return wParam;
		}
	};
	class KeyEvent:public BaseEvent {
	private:
		bool heldShift;
		bool heldCtrl;
		unsigned int keyCode;

	public:
		KeyEvent(UIWindow* destwindow,unsigned int keycode, bool heldshift, bool heldctrl) :BaseEvent(destwindow) {
			keyCode = keycode;
			heldCtrl = heldctrl;
			heldShift = heldshift;
		}
		inline bool IsHeldShift() {
			return heldShift;
		}
		inline bool IsHeldCtrl() {
			return heldCtrl;
		}
		inline unsigned int GetKeyCode() {
			return keyCode;
		}
	};
	class CharEvent :public BaseEvent {

	private:
		unsigned int charCode;

	public:
		CharEvent(UIWindow* destwindow, unsigned int charcode) :BaseEvent(destwindow) {
			charCode = charcode;
		}
		inline unsigned int GetCharCode(){
			return charCode;
		}

	};
	class MouseEventListener :public AutoDelete
	{
	public:
		virtual void OnMouse(MouseEvent& mouseEvent) = 0;
		virtual void OnMouseOut(UIElement*source) = 0;
	};
	class KeyEventListener:public AutoDelete {
	public:
		virtual void OnKey(KeyEvent& keyEvent) = 0;
	};
	class CharEventListener :public AutoDelete {
	public:
		virtual void OnChar(CharEvent& charEvent) = 0;
	};
}
