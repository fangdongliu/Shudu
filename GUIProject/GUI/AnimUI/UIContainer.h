#pragma once
#include"UIElement.h"
#include<unordered_map>
namespace AnimUI {
	class UIContainer:public UIElement
	{
	public:
		UIContainer();
		~UIContainer();
		static float scrollBorderRadius;
		static float scrollWidth;
		static D2D1_COLOR_F scrollColor;
		AnimUI::RectF					childcontentRect;
		AnimUI::RectF					scrollXRect;
		AnimUI::RectF					scrollYRect;
	protected:
		
		
		float						scrollX;
		float						scrollY;
		
		std::unordered_map<std::wstring, UIElement*>	idMap;

	public:
		std::vector<UIElement*>		childs;

		UIElement*	GetElementById(std::wstring& id)override;

	    std::shared_ptr<std::vector<UIElement*>>
					GetElementsByType(RuntimeId runtimeId)override;

		void		DetachChild(UIElement*child);
		void		DeleteChild(UIElement*child);

		void		AddChild(UIElement*child);
		
		RuntimeId	GetRuntimeId()				override	{return RuntimeId::RuntimeIdUIContainer;}

		void ChildNeedUpdate()					override;
		void OnMeasure(UIWindow*destWindow)		override;
		void OnMouse(MouseEvent &mouseEvent)	override;
		void OnScroll(MouseEvent &mouseEvent)	override;
		
		void OnLayout()							override = 0;

		void PaintScroller(UIWindow*destWindow)override;
		

		void PaintChilds(UIWindow*destWindow)override;
		virtual bool DispatchEventToChild(MouseEvent &mouseEvent);
		
	};

}