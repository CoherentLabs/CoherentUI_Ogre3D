/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
	  |___/                              
	  Tutorial Framework
	  http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

namespace Ogre
{
class CoherentUIView;
}

class TutorialApplication
	: public BaseApplication
{
public:
	TutorialApplication(void);
	virtual ~TutorialApplication(void);

protected:
	virtual void createScene(void);
	virtual bool setup();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual bool keyReleased(const OIS::KeyEvent& arg);

	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

private:
	std::unique_ptr<Ogre::Rectangle2D> mMiniScreen;
	Ogre::CoherentUIView* mView;
};

#endif // #ifndef __TutorialApplication_h_
