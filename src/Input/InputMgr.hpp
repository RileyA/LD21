#ifndef OISSUBSYSTEM_H
#define OISSUBSYSTEM_H

#include "Subsystem.hpp"
#include "OgreVector2.h"

using Ogre::Vector2;

class OISListener;

/** Input subsystem using OIS */
class InputMgr : public Subsystem
{
public:

	InputMgr();
	virtual ~InputMgr();

	/** INTERNAL: inits the system */
	virtual void init();
	/** INTERNAL: deinits the system */
	virtual void deinit();
	/** INTERNAL: updates the system */
	virtual void update(Real delta);
	/** INTERNAL: called at the end of a state */
	virtual void endState();
	/** Gets the name of the system 
		@return The name ("OgreSubsystem") */
	virtual String getName();

	/** Gets whether or not a key is down by keycode */
	bool isKeyDown(uint key);
	/** Gets whether or not a key is down by name */
	bool isKeyDown(String key);

	/** Shortcut for detecting a single key press */
	bool wasKeyPressed(uint key);
	/** Shortcut for detecting a single key press */
	bool wasKeyPressed(String key);

	/** Gets whether or not a mouse button is down by index */
	bool isButtonDown(uint button);
	/** Gets whether or not a mouse button is down by name */
	bool isButtonDown(String button);

	/** Shortcut for handling a single button press (by index) */
	bool wasButtonPressed(uint button);
	/** Shortcut for handling a single button press (by name) */
	bool wasButtonPressed(String button);

	/** Starts up the input system (must be done per state) 
	 *		@param handle Handle of the window from which to track input 
	 *		@param grabMouse Does the application take over the OS mouse cursor	*/	
	void initInput(size_t handle, bool grabMouse);
	/** Shuts the input system down */
	void deinitInput();
	
	/** Toggles whether or not the mouse is "trapped" by the window */
	void toggleMouseGrab();

	/** Called when the mouse moves */
	void _setMousePos(int x,int y,int relx,int rely);
	/** Called on each key event */
	void _key(uint key,bool up,uint val);
	/** Called on mouse button events */
	void _mouseButton(uint button,bool up);

	/** Set up string -> keycode conversions */
	void assignConversions();

protected:

	bool mInitialized;
	size_t mHandle;
	OISListener* mListener;
	std::map<uint,String> mKeys;
	std::map<String,uint> mKeyNames;
	std::vector<String> mButtons;
	std::map<String,uint> mButtonNames;
	std::map<uint, bool> mKeyStates;
	std::map<uint, bool> mKeyPresses;
	std::vector<bool> mButtonStates;
	std::vector<bool> mButtonPresses;

	Vector2 mMousePos;

	bool mGrabbedMouse;

};

#endif
