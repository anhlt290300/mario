#include "Timer.h"

CTimer::CTimer(bool useGameTime, long timeout) {
	this->useGameTime = useGameTime;
	this->timeleft = this->timeout = timeout;
	this->state = TimerState::STOPPED;
	this->startPoint = GetTickCount64();
}

void CTimer::SetTimeOut(long timeout) {
	AddTimeOut(timeout - this->timeout);
}

void CTimer::AddTimeOut(long timeout) {
	this->timeleft += timeout;
	this->timeout += timeout;
}

TimerState CTimer::GetState()
{
	return state;
}

void CTimer::Start() {
	this->state = TimerState::RUNNING;
	this->startPoint = GetTickCount64();
}

void CTimer::Stop() {
	this->state = TimerState::STOPPED;
}

void CTimer::Reset() {
	this->state = TimerState::STOPPED;
	this->timeleft = timeout;
}

void CTimer::Update(unsigned long dt) {
	if (state != TimerState::RUNNING) return;

	if (useGameTime) {
		this->timeleft -= dt;
	}
	else {
		this->timeleft = timeout - (GetTickCount64() - startPoint);
	}

	if (timeleft < 0) {
		state = TimerState::TIMEOVER;
		timeleft = 0;
	}
}
