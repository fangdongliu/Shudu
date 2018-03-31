#include "timer.h"
#include<windows.h>
using namespace AnimUI;
Timer::Timer() :mBaseTime(0), mStopped(false), mStopTime(0), mPausedTime(0), mDeltaTime(0.0), mCurrTime(0), mPrevTime(0)
{
	__int64 mSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&mSec);
	mSecondsPerCount = (double)1.0 / mSec;
}
float Timer::TotalTime()const {
	if (mStopped)
		return (mStopTime - mBaseTime - mPausedTime)*mSecondsPerCount;
	return (mCurrTime - mBaseTime - mPausedTime)*mSecondsPerCount;
}
float Timer::DeltaTime()const {
	return mDeltaTime;
}
void Timer::Reset() {
	__int64 mNow;
	QueryPerformanceCounter((LARGE_INTEGER*)&mNow);
	mBaseTime = mNow;
	mPrevTime = mNow;
	mCurrTime = mNow;
	mStopped = false;
	mPausedTime = 0;
}
void Timer::Tick() {
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}
	__int64 mNow;
	QueryPerformanceCounter((LARGE_INTEGER*)&mNow);
	mCurrTime = mNow;
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;
	mPrevTime = mCurrTime;
	if (mDeltaTime < 0.0) {
		mDeltaTime = 0.0;
	}
}
void Timer::Stop() {
	if (!mStopped) {
		mStopped = true;
		__int64 stopTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
		mStopTime = stopTime;
		mDeltaTime = 0.0;
	}
}
void Timer::Start() {
	if (mStopped) {
		mStopped = false;
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
		mPausedTime += startTime - mStopTime;
		mCurrTime = startTime;
		mPrevTime = startTime;
	}
}