#ifndef EventHandler_H
#define EventHandler_H

#include "Slot.hpp"
#include "Signal.hpp"
#include "MessageAny.hpp"

/** An implementation of the Observer Pattern, based heavily on Qt's
 *		signals and slots. */
class EventHandler
{
public:

	/** Constructor */
	EventHandler(){}
	/** Destructor */
	virtual ~EventHandler(){};

	/** Creates a named Signal object 
	 *		@param name What this Signal will be called*/
	Signal* createSignal(String name);

	/** Gets whether or not a named Signal exists
	 *		@param name The name to look for 
	 *		@return Whether or not the Signal was found*/
	bool hasSignal(String name) const;

	/** Returns a named signal
	 *		@param name The name to look for
	 *		@return The Signal (NULL otherwise) */
	Signal* getSignal(String name);

	/** Creates a Slot
	 *		@param name What to call the new Slot 
	 *		@param del The Delegate corresponding to the callback function */
	Slot* createSlot(String name,Delegate del);

	/** Creates a Slot
	 *		@param name What to call the new Slot 
	 *		@param context The context from which to call the function
	 *		@param function The function to call */
	template <typename obj,typename func> Slot* createSlot(String name, obj context, func function)
	{
		return createSlot(name,fastdelegate::MakeDelegate(context,function));
	}
	
	/** Gets whether or not a named Slot exists
	 *		@param name The name to look for
	 *		@return Whether or not it was found */
	bool hasSlot(String name) const;

	/** Retrieves a named Slot 
	 *		@param name The name to look for
	 *		@return The Slot, or NULL if not found */
	Slot* getSlot(String name);

private:

	// This handler's Signals
	std::map<String,Signal> mSignals;
	// This handler's Slots
	std::map<String,Slot> mSlots;

};

#endif
