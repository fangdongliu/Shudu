#pragma once
namespace AnimUI {

	class AnimationObject{

	public:
		virtual void Update(float percent) = 0;

	};

	template<typename T>
	class AnimationTemplate:public AnimationObject {

	public:
		AnimationTemplate(T& dest_, T start_, T end_) :dest(dest_), start(start_), end(end_) {};
		void Update(float percent)override;

	protected:
		T&		dest; 
		T		start;
		T       end;

	};
}