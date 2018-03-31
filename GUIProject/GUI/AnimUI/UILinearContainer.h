#pragma once
#include"UIContainer.h"
namespace AnimUI {
	enum class LinearFloat {
		LinearFloatNone,
		LinearFloatLeft,
		LinearFloatRight
	};
	class UILinearContainer:public UIContainer
	{
	public:
		UILinearContainer();
		
	private:
		LinearFloat				linearFloat;
		std::vector<float>		lineHeights;
		std::vector<int>		lineBreaks;
		int						firstVisibleChild;
		int						lastVisibleChild;

	public:
		RuntimeId	GetRuntimeId()								override { return RuntimeId::RuntimeIdUILinearContainer; }
		void		OnLayout()									override;
		void		PaintChilds(UIWindow*destWindow)			override;
		bool		DispatchEventToChild(MouseEvent &mouseEvent)override;
		void		ChildNeedUpdate()							override;
		inline void Float(LinearFloat dir) { linearFloat = dir; };

	private:
		
		void			AlignToCenter(float maxHeight, int start, int count);
		void			LayoutFloatLeft();
		void			LayoutFloatRight();
		void			LayoutFloatNone();
		
	};

}