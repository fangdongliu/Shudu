#pragma once
#include"UIContainer.h"
#include<vector>
namespace AnimUI {
	class UITable:public UIContainer
	{

	private:
		int firstVisibleChild, lastVisibleChild;
		std::vector<int>lineStops;
		std::vector<float>offsetHeights;
		std::vector<float>offsetWidths;

	public:
		void			ChildNeedUpdate()								override;
		void			OnLayout()										override;
		void			PaintChilds(UIWindow*destWindow)				override;
		bool			DispatchEventToChild(MouseEvent &mouseEvent)	override;
		RuntimeId		GetRuntimeId()									override {return RuntimeId::RuntimeIdUITable;}
		
		inline void EndLine() {
			lineStops.push_back(childs.size());
		}
	
	};
}
