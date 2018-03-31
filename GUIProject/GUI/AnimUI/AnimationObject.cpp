#include "AnimationObject.h"
#include<d2d1.h>
using namespace AnimUI;

void AnimationTemplate<int>::Update(float percent){

	if (percent >= 1.0f)
		dest = end;
	else 
		dest = (end - start)*percent + start;

}

void AnimationTemplate<float>::Update(float percent) {

	if (percent >= 1.0f)
		dest = end;
	else 
		dest = (end - start)*percent + start;

}
void AnimationTemplate<double>::Update(float percent) {

	if (percent >= 1.0f)
		dest = end;
	else 
		dest = (end - start)*percent + start;

}
void AnimationTemplate<D2D1_COLOR_F>::Update(float percent) {

	if (percent >= 1.0f)
		dest = end;
	else {
		dest.a= (end.a - start.a)*percent + start.a;
		dest.r = (end.r - start.r)*percent + start.r;
		dest.g = (end.g - start.g)*percent + start.g;
		dest.b = (end.b - start.b)*percent + start.b;
	}
}
