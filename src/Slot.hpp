#ifndef SLOT_H
#define SLOT_H

#include "FastDelegate.h"
#include "StdHeaders.hpp"

class Message;
class Signal;

// save some typing...
typedef fastdelegate::FastDelegate1<const Message&> Delegate;

/** A Qt-inspired Slot, recieves messages sent from Signals */
class Slot
{
public:

	// The delegate itself
	Delegate slot;

	/** Default Constructor */
	Slot(){}
	/** Constructor
	 *		@param _slot The delegate to use */
	Slot(Delegate _slot):slot(_slot){}
	~Slot(){stopAllListening();}

	/** Listen to a given Signal 
	 *		@param signal The signal to listen to */
	void listenTo(Signal* signal,bool notify=true);

	/** Stop listening to a given Signal 
	 *		@param signal The signal to stop listening to */
	void stopListeningTo(Signal* signal,bool notify = true);

	/** Stop listening to all Signals */
	void stopAllListening();

	/** Listen to a Signal */
	void operator += (Signal* signal);

	/** Stop listening to a Signal */
	void operator -= (Signal* signal);

private:
	
	// list of Signals being listened to
	std::list<Signal*> mSignals;

};

#endif
