#include "AnimationManager.h"
using namespace AnimUI;
namespace {

	int animId = 0;

}

AnimationManager::AnimationManager() {

	timer.Reset();

}
AnimationManager::~AnimationManager() {

	for (auto i : animationSet)
		delete i.second;

}

void AnimationManager::Delete(int animationId) {
	if (animationSet.count(animationId)) {
		auto animLast = animationSet[animationId];
		if (animLast)
			delete animLast;

		animationSet.erase(animationId);
	}
}
Animation* AnimationManager::RequestAnimation(UIElement*element,int*animationId) {

	Animation* animLast=nullptr;

	if (animationSet.count(*animationId)) {
		animLast = animationSet[*animationId];

		if (animLast) {
			animLast->Release();
			animLast->Restart();
			return animLast;
		}
	}

	animLast = new Animation([](float in)->float {
		return (sinf((in - 0.5f)*3.1415926f) + 1.0f) / 2.0f;
	});
	*animationId = ++animId;
	animationSet[animId] = animLast;
	animLast->dest = element;
	return animLast;

}

bool AnimationManager::Update() {

	timer.Tick();
	
	std::vector<int>deleteList;

	for (auto i : animationSet) {

		if (i.second->startTime == 0.0f) {
			i.second->startTime = timer.TotalTime();
		}
		else if (i.second->Update(timer.TotalTime())) {
			delete i.second;
			deleteList.push_back(i.first);
		}

	}

	for (auto i : deleteList) {
		animationSet.erase(i);
	}

	return animationSet.size();

}