/*
-----------------------------------------------------------------------------
This source file is part of OGRE
	(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#ifndef __CoherentUIView_H__
#define __CoherentUIView_H__

#include <OgreCoherentUIPrerequisites.h>
#include <OIS/OISMouse.h>

namespace Coherent
{
namespace UI
{
	class View;
	class ViewListener;
	class IEventHandler;
}
}

namespace OIS
{
	class KeyEvent;
}

namespace Ogre
{
	class CoherentUIViewListener;
	class ICoherentEventHandler;

	// A minimalistic wrapper for Coherent::UI::View
	class _OgreCoherentUIPluginExport CoherentUIView
		: public Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL>>
	{
	public:
		virtual ~CoherentUIView();

		void destroy();
		
		int getId() const;
		void loadUrl(const Ogre::String& url);
		void resize(int width, int height);

		void injectKeyDown(const OIS::KeyEvent& arg);
		void injectKeyUp(const OIS::KeyEvent& arg);

		void injectMouseMove(const OIS::MouseEvent& arg);
		void injectMouseDown(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
		void injectMouseUp(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

		void registerEvent(const Ogre::String& eventName, Coherent::UI::IEventHandler* handler);

		Ogre::TexturePtr getTexture() const;
		Ogre::MaterialPtr getTextureMaterial() const;
		void setTextureMaterial(Ogre::MaterialPtr material);
		
		Coherent::UI::View* getInternalView() const { return mView; }

	private:
		friend class CoherentUIViewListenerBridge;
		friend class CoherentUIManager;
		CoherentUIView(Ogre::CoherentUIViewListener* listener, int width, int height, bool enableDepthWrite);

		CoherentUIView(const CoherentUIView&);
		CoherentUIView& operator=(const CoherentUIView&);

		void processMouseEvent(const OIS::MouseEvent& arg, OIS::MouseButtonID id, int state);

	private:
		Ogre::TexturePtr mTexture;
		Ogre::MaterialPtr mTextureMaterial;
		Coherent::UI::ViewListener* mViewListener;
		Coherent::UI::View* mView;
	};

}


#endif
