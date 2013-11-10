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
#include <CoherentUIViewListenerBridge.h>
#include <CoherentUIView.h>
#include <CoherentUIViewListener.h>
#include <SharedMemoryHelper.h>
#include <Coherent/UI/View.h>
#include <OgreHardwarePixelBuffer.h>

namespace Ogre
{
	//-----------------------------------------------------------------------
	CoherentUIViewListenerBridge::CoherentUIViewListenerBridge(
		Ogre::CoherentUIView* view,
		Ogre::CoherentUIViewListener* userViewListener
		)
		: mView(view)
		, mUserViewListener(userViewListener)
	{
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::OnViewCreated(Coherent::UI::View* view)
	{
		assert(!mView->mView);
		view->SetFocus();
		mView->mView = view;

		if (mUserViewListener)
		{
			mUserViewListener->onViewCreated(mView);
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::OnNavigateTo(const wchar_t* path)
	{
		Ogre::String pathConverted(path, path + wcslen(path));
		if (mUserViewListener)
		{
			mUserViewListener->onUrlLoaded(mView, pathConverted);
		}		
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::OnReadyForBindings(int frameId, const wchar_t* path, bool isMainFrame)
	{
		if (isMainFrame)
		{
			Ogre::String pathConverted(path, path + wcslen(path));
			if (mUserViewListener)
			{
				mUserViewListener->onReadyForBindings(mView, pathConverted);
			}
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::OnDraw(Coherent::UI::CoherentHandle handle, bool usesSharedMemory, int width, int height)
	{
		assert(mView);
		assert(usesSharedMemory);

		Ogre::TexturePtr texture = mView->getTexture();

		if (texture->getWidth() != width || texture->getHeight() != height)
		{
			assert(!"Texture dimensions of Coherent UI and OGRE textures do not match!");
			return;
		}

		const size_t singlePixelSize = sizeof(Ogre::uint32);
		const Ogre::uint32 surfaceWidth = Ogre::uint32(width);
		const Ogre::uint32 surfaceHeight = Ogre::uint32(height);

		const size_t size = surfaceWidth * surfaceHeight * singlePixelSize;
		Ogre::uint32* originalSrc = (Ogre::uint32*)SharedMemoryHelper::Map(handle, size);

		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();

		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		Ogre::uint32* dest = static_cast<Ogre::uint32*>(pixelBox.data);
		Ogre::uint32* src = originalSrc;

		for (size_t i = 0; i < texture->getHeight(); i++)
		{
			std::memcpy(dest, src, texture->getWidth() * singlePixelSize);

			src += width;
			dest += texture->getWidth() + pixelBox.getRowSkip();
		}

		// Unlock the pixel buffer
		pixelBuffer->unlock();


		SharedMemoryHelper::Unmap(originalSrc, size);
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::CreateSurface(bool sharedMemory, unsigned width, unsigned height, Coherent::UI::SurfaceResponse* response)
	{
		assert(sharedMemory);
		const size_t singlePixelSize = sizeof(unsigned);
		const size_t sizeInBytes = width * height * singlePixelSize;
		Coherent::UI::CoherentHandle sharedHandle = SharedMemoryHelper::CreateSegment(sizeInBytes);

		response->Signal(sharedHandle);
	}

	//-----------------------------------------------------------------------
	void CoherentUIViewListenerBridge::DestroySurface(Coherent::UI::CoherentHandle surface, bool usesSharedMemory)
	{
		assert(usesSharedMemory);
		SharedMemoryHelper::DestroySegment(surface);
	}
}