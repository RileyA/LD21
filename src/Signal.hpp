#ifndef SIGNAL_H
#define SIGNAL_H

#include "StdHeaders.hpp"
#include "MessageAny.hpp"

class Message;
class Slot;

/** A Qt-inspired Signal, maintains a list of listeners (Slots)
 *		and calls their delegates when fired */
class Signal
{
public:

	/** Constructor */
	Signal(){}
	/** Destructor */
	~Signal(){removeAllListeners();}

	/** Fires this signal (calls all listeners' delegates) with a given message
	 *		@param message The message to send */
	void fire(const Message* message) const;

	/** Fires this signal (calls all listeners' delegates) with a given message
	 *		@param message The message to send */
	void fire(const Message& message) const;

	/** Fires this signal
	 *		@param data The message to be packed and sent */
	template<class T> void send(const T& data) const
	{
		fire(MessageAny<T>(data));
	}

	/** Adds a listener to this Signal
	 *		@param Slot Pointer to the Slot */
	void addListener(Slot* slot,bool notify=true);

	/** Removes a Listener from this Signal
	 *		@param slot Pointer to the desired slot */
	void removeListener(Slot* slot,bool notify=false);

	/** Removes all listeners from this Signal */
	void removeAllListeners();

	/** Adds a listener */
	void operator += (Slot* slot);
	/** Removes a Listener */
	void operator -= (Slot* slot);

private:

	// This Signal's list of listeners
	std::list<Slot*> mListeners;

};

#endif
