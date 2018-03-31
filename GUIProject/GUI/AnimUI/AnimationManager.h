#pragma once
#include<unordered_map>
#include"Animation.h"
#include"timer.h"
namespace AnimUI {
	class AnimationManager{

	private:
		Timer								timer;
		std::unordered_map<int, Animation*>	animationSet;

	public:
		AnimationManager();
		~AnimationManager();
		void								Delete(int animationId);
		bool								Update();
		Animation*							RequestAnimation(UIElement*element,int*animationId);

	};

}