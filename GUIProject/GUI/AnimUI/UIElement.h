#pragma once
#include"predef.h"
#include"UIStyle.h"
#include"AnimUITypes.h"
#include<dwrite.h>
#include<vector>
#include"BaseEvent.h"
#include<memory>
namespace AnimUI {
	class UIWindow;
	enum RuntimeId {
		RuntimeIdUILinearContainer,
		RuntimeIdUIRelativeContainer,
		RuntimeIdUITable,
		RuntimeIdUIContainer,
		RuntimeIdUIElement,
		RuntimeIdUIText,
	};
	class UIElement:public AutoDelete
	{
	public:
		UIElement();
		~UIElement();

		static IDWriteFactory *dwriteFactory;

	public:
		bool					isFocusable;
		bool					needMeasure;
		bool					isHidden;
		UIStyle					currentStyle;
		AnimUI::PointF				relativePos;
		AnimUI::RectF				rect;
		
		std::wstring			id;

		CanvasPainter		*	painter;
		KeyEventListener	*	keyEventListener;
		CharEventListener	*	charEventListener;
		MouseEventListener	*	mouseEventListener;
	protected:

		int						animationId;
		UIStateStyle		*	stateStyle;
		UIState					state;
		
		UIElement			*	parentElement;
		

	public:
		
		void				ChangeCurrentStyle(UIWindow*, UIState,float duration=-1.0f);
		void				NeedUpdate();
		void				CaculateSize(float parentWidth, float parentHeight);
		void				Detach();
		inline void				Hidden()		{ isHidden = true; NeedUpdate(); }
		inline void				Show()			{ isHidden = false; NeedUpdate(); }

		virtual RuntimeId	GetRuntimeId()					{return RuntimeId::RuntimeIdUIElement;}
		virtual void		ChildNeedUpdate()				{}
		virtual UIElement*	GetElementById(std::wstring& id);
		
		virtual std::shared_ptr<std::vector<UIElement*>>
							GetElementsByType(RuntimeId runtimeId);
	
		inline void			SetParent(UIElement*parent) { parentElement = parent; }
		inline UIElement* GetParent() { return parentElement; }

		inline void			SetStateStyle(UIStateStyle *stateStyle_) {
								stateStyle = stateStyle_;
									if (stateStyle_) 
										currentStyle.SafeCopy(stateStyle->styles[0]);
		}

	protected:
		void				PaintBorder(UIWindow*window);
		void				PaintBackground(UIWindow*window);
		virtual void				PaintScroller(UIWindow*window){}
		virtual void				PaintChilds(UIWindow*window){}
		
	public:
		virtual void		OnLayout()								{ needMeasure = false; }
		virtual void		OnMeasure(UIWindow*)					{ needMeasure = false; }
		virtual void		OnScroll(MouseEvent &mouseEvent)		{}
		virtual void		OnLoseFocus(UIWindow*)					{}
		virtual void		OnMouse(MouseEvent &mouseEvent);
		virtual void		OnMouseOut(UIWindow* destWindow);
		virtual void		OnKey(KeyEvent &keyEvent);
		virtual void		OnChar(CharEvent &charEvent);
		virtual void		OnPaint(UIWindow *destWindow);
		
	
		
	};
	
}