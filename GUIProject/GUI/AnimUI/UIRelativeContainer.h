#pragma once
#include"UIContainer.h"
namespace AnimUI {
	class UIRelativeContainer:public UIContainer
	{
	public:
		UIRelativeContainer();
		~UIRelativeContainer();
		void OnLayout()									override;
		void PaintChilds(UIWindow*destWindow)			override;
		virtual RuntimeId GetRuntimeId() {
			return RuntimeId::RuntimeIdUIRelativeContainer;
		}
	};

}