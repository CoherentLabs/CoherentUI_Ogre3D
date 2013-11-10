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

#ifndef __CoherentUIViewListenerBridge_H__
#define __CoherentUIViewListenerBridge_H__

#include <Coherent/UI/ViewListener.h>

namespace Ogre
{
	class CoherentUIView;
	class CoherentUIViewListener;

	class CoherentUIViewListenerBridge
		: public Coherent::UI::ViewListener
		, public Ogre::AllocatedObject<Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL>>
	{
	public:
		CoherentUIViewListenerBridge(Ogre::CoherentUIView* view, Ogre::CoherentUIViewListener* userViewListener);

		virtual void OnViewCreated(Coherent::UI::View* view);
		virtual void OnNavigateTo(const wchar_t* path);
		virtual void OnReadyForBindings(int frameId, const wchar_t* path, bool isMainFrame);
		virtual void OnDraw(Coherent::UI::CoherentHandle handle, bool usesSharedMemory, int width, int height);
		virtual void CreateSurface(bool sharedMemory, unsigned width, unsigned height, Coherent::UI::SurfaceResponse* response);
		virtual void DestroySurface(Coherent::UI::CoherentHandle surface, bool usesSharedMemory);

	private:
		Ogre::CoherentUIView* mView;
		Ogre::CoherentUIViewListener* mUserViewListener;
	};

}

#endif