#ifndef OISLISTENER_H
#define OISLISTENER_H

#include "StdHeaders.hpp"
#include "OIS.h"

class InputMgr;

/** Internal class that serves as the OIS lsitener
 *		TODO: add joystick support */
class OISListener : public OIS::KeyListener, public OIS::MouseListener
{
public:

	OISListener(size_t windowHandle,InputMgr* sys,bool grabMouse);
	~OISListener();

	void update();
	void setMousePosition(int x,int y);
	void setInputViewSize(int width, int hieght);
	void checkPosition();

protected:

	bool mouseMoved(const OIS::MouseEvent& arg);
	bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent& arg);
	bool keyReleased(const OIS::KeyEvent& arg);

private:

	OIS::InputManager* mInputManager;
	OIS::Keyboard* mKeyboard;
	OIS::Mouse* mMouse;

	int mCursorX;
	int mCursorY;

	InputMgr* mSubsystem;
	
};

#endif
