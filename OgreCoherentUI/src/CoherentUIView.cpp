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
#include <CoherentUIView.h>
#include <CoherentUIViewListenerBridge.h>
#include <CoherentUIInputConvertor.h>
#include <Coherent/UI/View.h>
#include <Coherent/UI/InputEvents.h>
#include <Coherent/UI/Binding/String.h>
#include <Coherent/UI/Binding/Map.h>
#include <OgreHardwarePixelBuffer.h>

#if OGRE_WCHAR_T_STRINGS
#error "[COUI] Currently OGRE wchar_t strings are not supported!"
#endif

namespace Ogre
{
	//-----------------------------------------------------------------------
	CoherentUIView::CoherentUIView(Ogre::CoherentUIViewListener* listener, int width, int height, bool enableDepthWrite)
		: mView(NULL)
	{
		mViewListener = OGRE_NEW CoherentUIViewListenerBridge(this, listener);

		static int _textureCounter = 0;
		Ogre::StringStream ss;
		ss << "CoherentDynamicTexture" << ++_textureCounter;
		Ogre::String textureName = ss.str();
		ss << "_Mat";
		Ogre::String materialName = ss.str();

		// Create a texture
		mTexture = TextureManager::getSingleton().createManual(
			textureName,
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D,
			width,
			height,
			1,
			PF_BYTE_BGRA,
			TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		// Clear the texture
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = mTexture->getBuffer();
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
		Ogre::uint32* dest = static_cast<Ogre::uint32*>(pixelBox.data);
		std::memset(dest, 0, (width * 4 + pixelBox.getRowSkip()) * height);
		pixelBuffer->unlock();

		// Create a material using the texture
		mTextureMaterial = MaterialManager::getSingleton().create(
			materialName,
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		mTextureMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(textureName);
		mTextureMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SBF_ONE, SBF_ONE_MINUS_SOURCE_ALPHA);
		mTextureMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(enableDepthWrite);
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::destroy()
	{
		if (mViewListener)
		{
			OGRE_DELETE mViewListener;
			mViewListener = NULL;
		}

		if (mView)
		{
			mView->Destroy();
			mView = NULL;
		}
	}

	//-----------------------------------------------------------------------
	int CoherentUIView::getId() const
	{
		if (mView)
		{
			return int(mView->GetId());
		}

		return -1;
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::loadUrl(const Ogre::String& url)
	{
		if (mView)
		{
			std::wstring urlConverted(url.begin(), url.end());
			mView->Load(urlConverted.c_str());
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::resize(int width, int height)
	{
		if (mView)
		{
			mView->Resize(width, height);
		}

		// Recreate backing texture
		if (mTexture.get())
		{
			mTexture = TextureManager::getSingleton().createManual(
				mTexture->getName(),
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				TEX_TYPE_2D,
				width,
				height,
				1,
				PF_BYTE_BGRA,
				TU_DEFAULT);
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::injectKeyDown(const OIS::KeyEvent& arg)
	{
		if (mView)
		{
			Coherent::UI::KeyEventData keyData;
			if (CoherentUIInputConvertor::ConvertToCoherentKeyEvent(arg, CoherentUIInputConvertor::KeyDown, keyData))
			{
				mView->KeyEvent(keyData);

				// Send a Char event if needed
				if (arg.text != 0)
				{
					keyData.Type = Coherent::UI::KeyEventData::Char;
					keyData.KeyCode = arg.text;
					mView->KeyEvent(keyData);
				}
			}
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::injectKeyUp(const OIS::KeyEvent& arg)
	{
		if (mView)
		{
			Coherent::UI::KeyEventData keyData;
			if (CoherentUIInputConvertor::ConvertToCoherentKeyEvent(arg, CoherentUIInputConvertor::KeyUp, keyData))
			{
				mView->KeyEvent(keyData);
			}
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::processMouseEvent(const OIS::MouseEvent& arg, OIS::MouseButtonID id, int state)
	{
		if (mView)
		{
			Coherent::UI::MouseEventData mouseData;
			if (CoherentUIInputConvertor::ConvertToCoherentMouseEvent(arg, id, CoherentUIInputConvertor::MouseState(state), mouseData))
			{
				mView->MouseEvent(mouseData);
			}
		}
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::injectMouseMove(const OIS::MouseEvent& arg)
	{
		processMouseEvent(arg, OIS::MB_Button7, CoherentUIInputConvertor::MoveOrWheel);
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::injectMouseDown(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
	{
		processMouseEvent(arg, id, CoherentUIInputConvertor::MouseDown);
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::injectMouseUp(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
	{
		processMouseEvent(arg, id, CoherentUIInputConvertor::MouseUp);
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::registerEvent(const Ogre::String& eventName, Coherent::UI::IEventHandler* handler)
	{
		if (mView)
		{
			mView->RegisterForEvent(eventName.c_str(), handler);
		}
	}

	//-----------------------------------------------------------------------
	Ogre::TexturePtr CoherentUIView::getTexture() const
	{
		return mTexture;
	}

	//-----------------------------------------------------------------------
	Ogre::MaterialPtr CoherentUIView::getTextureMaterial() const
	{
		return mTextureMaterial;
	}

	//-----------------------------------------------------------------------
	void CoherentUIView::setTextureMaterial(Ogre::MaterialPtr material)
	{
		mTextureMaterial = material;
	}
}

