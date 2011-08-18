#include "EventHandler.hpp"

Signal* EventHandler::createSignal(String name)
{
	mSignals[name] = Signal();
	return &mSignals[name];
}
//-----------------------------------------------------------------------

bool EventHandler::hasSignal(String name) const
{
	return mSignals.find(name)!=mSignals.end();
}
//-----------------------------------------------------------------------

Signal* EventHandler::getSignal(String name)
{
	if(hasSignal(name))
		return &(mSignals.find(name)->second);
	return 0;
}
//-----------------------------------------------------------------------

Slot* EventHandler::createSlot(String name,Delegate del)
{
	mSlots[name] = Slot(del);
	return &mSlots[name];
}
//-----------------------------------------------------------------------

bool EventHandler::hasSlot(String name) const
{
	return mSlots.find(name)!=mSlots.end();
}
//-----------------------------------------------------------------------

Slot* EventHandler::getSlot(String name)
{
	if(hasSlot(name))
		return &(mSlots.find(name)->second);
	return 0;
}
//-----------------------------------------------------------------------
