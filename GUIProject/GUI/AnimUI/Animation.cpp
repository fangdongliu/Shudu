#include "Animation.h"
#include"UIElement.h"

using namespace D2D1;
using namespace AnimUI;

Animation::Animation(float(*interpolater_)(float)){

	isRequireMeasure = false;
	interpolater = interpolater_;
	duration = 0.0f;
	animationEndCallBack = nullptr;
	Restart();

}
Animation::~Animation(){

	Release();

}
void Animation::Release() {

	for (auto i : animList)
		delete i;

	animList.clear();
	isRequireMeasure = false;
}

bool Animation::Update(float total) {

	float percent = (total-startTime)/ duration;
	
	if (percent > 1.0f)
		percent = 1.0f;
	else
		percent = interpolater(percent);
	
	for (auto i : animList)
		i->Update(percent);

	if(percent >= 1.0f && animationEndCallBack)
		animationEndCallBack(this);

	if (isRequireMeasure) 
		dest->NeedUpdate();

	return percent>=1.0f;

}

