#include "MessageAny.hpp"
#include "FastDelegate.h"
#include "Signal.hpp"
#include "Slot.hpp"

void Signal::fire(const Message* message) const
{
	std::list<Slot*>::const_iterator it = mListeners.begin();
	for(it;it!=mListeners.end();++it)
	{
		(*it)->slot(*message);
	}
}
//-----------------------------------------------------------------------

void Signal::fire(const Message& message) const
{
	std::list<Slot*>::const_iterator it = mListeners.begin();
	for(it;it!=mListeners.end();++it)
	{
		(*it)->slot(message);
	}
}
//-----------------------------------------------------------------------

void Signal::addListener(Slot* slot,bool notify)
{
	mListeners.push_back(slot);
	if(notify)
		slot->listenTo(this,false);
}
//-----------------------------------------------------------------------

void Signal::removeListener(Slot* slot,bool notify)
{
	std::list<Slot*>::iterator it = mListeners.begin();
	for(it;it!=mListeners.end();++it)
	{
		if((*it)==slot)
		{
			if(notify)
				(*it)->stopListeningTo(this,false);
			mListeners.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------

void Signal::removeAllListeners()
{
	std::list<Slot*>::iterator it = mListeners.begin();
	for(it;it!=mListeners.end();++it)
	{
		(*it)->stopListeningTo(this,false);
	}
	mListeners.clear();
}
void Signal::operator +=(Slot* slot)
{
	addListener(slot);
}
//-----------------------------------------------------------------------

void Signal::operator -=(Slot* slot)
{
	removeListener(slot);
}
//-----------------------------------------------------------------------
