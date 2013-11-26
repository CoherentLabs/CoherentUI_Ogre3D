/*
-----------------------------------------------------------------------------
This source file is part of OGRE
	(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2012 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include <CoherentUIManager.h>
#include <CoherentUIView.h>
#include <Coherent/UI/UISystem.h>
#include <Coherent/UI/ViewInfo.h>
#include <Coherent/UI/ViewListener.h>
#include <Coherent/UI/License.h>

#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32)
	#if (OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32)
		#define COUI_HOST_PATH L"Win32\\host"
	#elif (OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64)
		#define COUI_HOST_PATH L"Win64\\host"
	#else
		#error "[COUI] Unsupported architecture!"
	#endif
#elif (OGRE_PLATFORM == OGRE_PLATFORM_APPLE)
	#define COUI_HOST_PATH L"MacOSX\\host"
#else
#error "[COUI] Unsupported platform!"
#endif

template<> _OgreCoherentUIPluginExport Ogre::CoherentUIManager*
	Ogre::Singleton<Ogre::CoherentUIManager>::msSingleton = 0;

//---------------------------------------------------------------------------
namespace
{
	class OgreUIEventListener 
		: public Coherent::UI::EventListener
		, public Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL>>
	{
	public:
		virtual ~OgreUIEventListener() {}

		OgreUIEventListener()
			: mIsReady(false)
		{}

		virtual void SystemReady()
		{
			mIsReady = true;
		}

		bool IsReady() const
		{
			return mIsReady;
		}

		// Overload new/delete due to DLL-export quirks...
		void* operator new(size_t bytes)
		{
			return std::malloc(bytes);
		}

		void operator delete(void* ptr)
		{
			std::free(ptr);
		}

	private:
		bool mIsReady;
	};
}

//---------------------------------------------------------------------------

namespace Ogre
{
	//-----------------------------------------------------------------------
	CoherentUIManager::CoherentUIManager(void)
		: mUISystem(NULL)
		, mUIListener(new OgreUIEventListener())
	{
		InitializeUISystem();
	}

	//-----------------------------------------------------------------------
	CoherentUIManager::~CoherentUIManager(void)
	{
		if (mUISystem)
		{
			mUISystem->Uninitialize();
			mUISystem = NULL;
		}

		if (mUIListener)
		{
			delete mUIListener;
			mUIListener = NULL;
		}
	}

	//-----------------------------------------------------------------------
	Ogre::CoherentUIView* CoherentUIManager::createView(
		const Ogre::String& url,
		int width,
		int height,
		bool transparent,
		Ogre::CoherentUIViewListener* listener
		)
	{
		Ogre::CoherentUIView* result = NULL;

		if (mUISystem)
		{
			Coherent::UI::ViewInfo viewInfo;
			viewInfo.UsesSharedMemory = true;
			viewInfo.Width = width;
			viewInfo.Height = height;
			viewInfo.IsTransparent = transparent;

			// TODO: Make refcounted
			result = OGRE_NEW Ogre::CoherentUIView(listener, width, height);

			std::wstring urlConverted(url.begin(), url.end());

			// Abuse the friendship a little...
			mUISystem->CreateView(viewInfo, urlConverted.c_str(), result->mViewListener);
		}

		return result;
	}

	//-----------------------------------------------------------------------
	void CoherentUIManager::destroyView(Ogre::CoherentUIView* view)
	{
		if (view)
		{
			OGRE_DELETE view;
			view = NULL;
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIManager::update()
	{
		if (mUISystem)
		{
			mUISystem->Update();
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIManager::fetchSurfaces()
	{
		if (mUISystem)
		{
			mUISystem->FetchSurfaces();
		}
	}

	//-----------------------------------------------------------------------
	CoherentUIManager* CoherentUIManager::getSingletonPtr(void)
	{
		return msSingleton;
	}

	//-----------------------------------------------------------------------
	CoherentUIManager& CoherentUIManager::getSingleton(void)
	{
		assert(msSingleton);
		return (*msSingleton);
	}

	//-----------------------------------------------------------------------
	void CoherentUIManager::InitializeUISystem(void)
	{
		Coherent::UI::SystemSettings settings;
		settings.HostDirectory = COUI_HOST_PATH;

		mUISystem = ::InitializeUISystem(COHERENT_UI_SDK_VER, COHERENT_KEY, settings, mUIListener);
		if (!mUISystem)
		{
			Ogre::LogManager::getSingleton().logMessage("Unable to initialize Coherent UI! Check log for details!");
		}
	}
}

