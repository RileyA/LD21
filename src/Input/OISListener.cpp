#include "StdHeaders.hpp"
#include "OISListener.hpp"
#include "InputMgr.hpp"
#include "Game.hpp"

OISListener::OISListener(size_t handle,InputMgr* sys,bool grabMouse)
{
	mSubsystem = sys;

	std::ostringstream windowHndStr;
	windowHndStr << handle;

	OIS::ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	if(!grabMouse)
	{
	#ifdef OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	#elif defined OIS_LINUX_PLATFORM
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
	#endif
	}

	mInputManager = OIS::InputManager::createInputSystem(pl);
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = Game::getPtr()->getGfx()->getWindow()->getWidth();
	ms.height = Game::getPtr()->getGfx()->getWindow()->getHeight();
	setMousePosition(ms.width/2,ms.height/2);
}
//-----------------------------------------------------------------------

OISListener::~OISListener()
{
	if (mInputManager)
	{
		if (mMouse)
		{
			mInputManager->destroyInputObject(mMouse);
			mMouse = 0;
		}
		if (mKeyboard)
		{
			mInputManager->destroyInputObject(mKeyboard);
			mKeyboard = 0;
		}
		OIS::InputManager::destroyInputSystem(mInputManager);
		mInputManager = 0;
	}
}
//-----------------------------------------------------------------------

void OISListener::update()
{
	mMouse->capture();
	mKeyboard->capture();
}
//-----------------------------------------------------------------------

void OISListener::setMousePosition(int x,int y)
{
	mCursorX = x;
	mCursorY = y;
	checkPosition();
}
//-----------------------------------------------------------------------

void OISListener::setInputViewSize(int width,int height)
{
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
	checkPosition();
}
//-----------------------------------------------------------------------

void OISListener::checkPosition()
{
	const OIS::MouseState &ms = mMouse->getMouseState();
	if (mCursorX < 0)
		mCursorX = 0;
	else if (mCursorX >= ms.width)
		mCursorX = ms.width - 1;
	if (mCursorY < 0)
		mCursorY = 0;
	else if (mCursorY >= ms.height)
		mCursorY = ms.height - 1;
	mSubsystem->_setMousePos(mCursorX,mCursorY,ms.X.rel,ms.Y.rel);
}
//-----------------------------------------------------------------------

bool OISListener::mouseMoved(const OIS::MouseEvent& arg)
{
	const OIS::MouseState &ms = mMouse->getMouseState();
	mCursorX = ms.X.abs;
	mCursorY = ms.Y.abs;
	checkPosition();
	return true;
}
//-----------------------------------------------------------------------

bool OISListener::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	mSubsystem->_mouseButton(id,true);
	return true;
}
//-----------------------------------------------------------------------

bool OISListener::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	mSubsystem->_mouseButton(id,false);
	return true;
}
//-----------------------------------------------------------------------

bool OISListener::keyPressed(const OIS::KeyEvent& arg)
{
	mSubsystem->_key(arg.key,true,arg.text);
	return true;
}
//-----------------------------------------------------------------------

bool OISListener::keyReleased(const OIS::KeyEvent& arg)
{
	mSubsystem->_key(arg.key,false,arg.text);
	return true;
}
//-----------------------------------------------------------------------
