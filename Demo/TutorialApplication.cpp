/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
#include "stdafx.h"
#include "TutorialApplication.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>

#include <CoherentUIManager.h>
#include <CoherentUIView.h>
#include <CoherentUIViewListener.h>


//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
	if (mView)
	{
		Ogre::CoherentUIManager::getSingleton().destroyView(mView);
	}
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if (!BaseApplication::keyPressed(arg))
	{
		return false;
	}

	mView->injectKeyDown(arg);

	if (arg.key == OIS::KC_H)
	{
		mView->getInternalView()->TriggerEvent("OnPlayerHealthChanged", std::rand() % 100);
	}

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::keyReleased( const OIS::KeyEvent &arg )
{
	if (!BaseApplication::keyReleased(arg))
	{
		return false;
	}

	mView->injectKeyUp(arg);

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::mouseMoved(const OIS::MouseEvent& arg)
{
	if (!BaseApplication::mouseMoved(arg))
	{
		return false;
	}
//	mTrayMgr->injectMouseMove(arg);

	mView->injectMouseMove(arg);

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (!BaseApplication::mousePressed(arg, id))
	{
		return false;
	}

	mView->injectMouseDown(arg, id);

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (!BaseApplication::mouseReleased(arg, id))
	{
		return false;
	}

	mView->injectMouseUp(arg, id);

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::frameStarted(const Ogre::FrameEvent& evt)
{
	if (!BaseApplication::frameStarted(evt))
	{
		return false;
	}

	Ogre::CoherentUIManager::getSingleton().fetchSurfaces();

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if (!BaseApplication::frameRenderingQueued(evt))
	{
		return false;
	}

	Ogre::CoherentUIManager::getSingleton().update();

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::frameEnded(const Ogre::FrameEvent& evt)
{
	if (!BaseApplication::frameEnded(evt))
	{
		return false;
	}

	return true;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::setup()
{
	if (!BaseApplication::setup())
	{
		return false;
	}

	mTrayMgr->hideAll();
	mTrayMgr->showCursor();

	return true;
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	// Set the scene's ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 
	// Create an Entity
	Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
 
	// Create a SceneNode and attach the Entity to it
	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
	headNode->attachObject(ogreHead);
 
	// Create a Light and set its position
	Ogre::Light* light = mSceneMgr->createLight("MainLight");
	light->setPosition(20.0f, 80.0f, 50.0f);

	// Coherent UI
	Ogre::Viewport* viewport = mWindow->getViewport(0);

	mView = Ogre::CoherentUIManager::getSingleton().createView(
		"coui://uiresources/hud/hud.html",
		viewport->getActualWidth(),
		viewport->getActualHeight(),
		true,
		NULL);

	if (mView)
	{
		mMiniScreen.reset(new Ogre::Rectangle2D(true));
		mMiniScreen->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
		mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));
		mMiniScreen->setMaterial(mView->getTextureMaterial()->getName());

		Ogre::SceneNode* miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode");
		miniScreenNode->attachObject(mMiniScreen.get());
	}
	else
	{
		Ogre::LogManager::getSingleton().logMessage("Error creating Coherent UI View!");
	}
}

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		TutorialApplication app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
