#include "StdHeaders.hpp"
#include "Slot.hpp"
#include "Signal.hpp"

void Slot::listenTo(Signal* signal,bool notify)
{
	mSignals.push_back(signal);
	if(notify)
		signal->addListener(this,false);
}
//-----------------------------------------------------------------------

void Slot::stopListeningTo(Signal* signal,bool notify)
{
	std::list<Signal*>::iterator it = mSignals.begin();
	for(it;it!=mSignals.end();++it)
	{
		if(signal==(*it))
		{
			if(notify)
				(*it)->removeListener(this,false);
			mSignals.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------

void Slot::stopAllListening()
{
	std::list<Signal*>::iterator it = mSignals.begin();
	for(it;it!=mSignals.end();++it)
	{
		(*it)->removeListener(this);
	}
	mSignals.clear();
}
//-----------------------------------------------------------------------

void Slot::operator += (Signal* signal)
{
	listenTo(signal);
}
//-----------------------------------------------------------------------

void Slot::operator -= (Signal* signal)
{
	stopListeningTo(signal);
}
//-----------------------------------------------------------------------
