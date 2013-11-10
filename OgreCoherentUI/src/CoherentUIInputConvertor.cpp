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
#include <CoherentUIInputConvertor.h>

#include <Coherent/UI/InputEvents.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
bool PerformAltGRCheck()
{
	HKL keylayout = GetKeyboardLayout(0);
	SHORT s = 0;

	// We go through all keys and check if any of them maps to CTRL+ALT which is AltGR according to MSDN
	for(auto i = 32u; i < 256u; ++i) 
	{
		s = VkKeyScanEx(static_cast<char>(i), keylayout);
		if(s != -1 && (s & 0x600) == 0x600) 
		{
			return true;
		}
	}

	return false;
}
#else
bool PerformAltGRCheck()
{
	return false;
}
#endif

static const bool HAS_ALTGR = PerformAltGRCheck();

static const int OIS_TO_WIN_KEYMAP[] =
{
	0, // KC_UNASSIGNED  
	27, // KC_ESCAPE      
	49, // KC_1           
	50, // KC_2           
	51, // KC_3           
	52, // KC_4           
	53, // KC_5           
	54, // KC_6           
	55, // KC_7           
	56, // KC_8           
	57, // KC_9           
	48, // KC_0           
	189, // KC_MINUS       
	187, // KC_EQUALS      
	8, // KC_BACK        
	9, // KC_TAB         
	81, // KC_Q           
	87, // KC_W           
	69, // KC_E           
	82, // KC_R           
	84, // KC_T           
	89, // KC_Y           
	85, // KC_U           
	73, // KC_I           
	79, // KC_O           
	80, // KC_P           
	219, // KC_LBRACKET    
	221, // KC_RBRACKET    
	13, // KC_RETURN      
	162, // KC_LCONTROL    
	65, // KC_A           
	83, // KC_S           
	68, // KC_D           
	70, // KC_F           
	71, // KC_G           
	72, // KC_H           
	74, // KC_J           
	75, // KC_K           
	76, // KC_L           
	186, // KC_SEMICOLON   
	222, // KC_APOSTROPHE  
	192, // KC_GRAVE       
	160, // KC_LSHIFT      
	220, // KC_BACKSLASH   
	90, // KC_Z           
	88, // KC_X           
	67, // KC_C           
	86, // KC_V           
	66, // KC_B           
	78, // KC_N           
	77, // KC_M           
	188, // KC_COMMA       
	190, // KC_PERIOD      
	191, // KC_SLASH       
	161, // KC_RSHIFT      
	106, // KC_MULTIPLY    
	164, // KC_LMENU       
	32, // KC_SPACE       
	20, // KC_CAPITAL     
	112, // KC_F1          
	113, // KC_F2          
	114, // KC_F3          
	115, // KC_F4          
	116, // KC_F5          
	117, // KC_F6          
	118, // KC_F7          
	119, // KC_F8          
	120, // KC_F9          
	121, // KC_F10         
	144, // KC_NUMLOCK     
	145, // KC_SCROLL      
	36, // KC_NUMPAD7     
	38, // KC_NUMPAD8     
	33, // KC_NUMPAD9     
	109, // KC_SUBTRACT    
	37, // KC_NUMPAD4     
	12, // KC_NUMPAD5     
	39, // KC_NUMPAD6     
	107, // KC_ADD         
	35, // KC_NUMPAD1     
	40, // KC_NUMPAD2     
	34, // KC_NUMPAD3     
	45, // KC_NUMPAD0     
	46, // KC_DECIMAL     
	0,
	0,
	226, // KC_OEM_102     
	122, // KC_F11         
	123, // KC_F12         
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	124, // KC_F13         
	125, // KC_F14         
	126, // KC_F15         
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0x15, // KC_KANA        
	0,
	0,
	193, // KC_ABNT_C1     
	0,
	0,
	0,
	0,
	0,
	0x1C, // KC_CONVERT     
	0,
	235, // KC_NOCONVERT   
	0,
	0, // KC_YEN         
	194, // KC_ABNT_C2     
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0, // KC_NUMPADEQUALS
	0,
	0,
	0, // KC_PREVTRACK   
	0, // KC_AT          
	0, // KC_COLON       
	0, // KC_UNDERLINE   
	0, // KC_KANJI       
	0, // KC_STOP        
	0, // KC_AX          
	0, // KC_UNLABELED   
	0,
	0, // KC_NEXTTRACK   
	0,
	0,
	0, // KC_NUMPADENTER 
	0xA3, // KC_RCONTROL    
	0,
	0,
	0, // KC_MUTE        
	0, // KC_CALCULATOR  
	0, // KC_PLAYPAUSE   
	0,
	0, // KC_MEDIASTOP   
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0, // KC_VOLUMEDOWN  
	0,
	0, // KC_VOLUMEUP    
	0,
	0, // KC_WEBHOME     
	0x6E, // KC_NUMPADCOMMA 
	0,
	0x6F, // KC_DIVIDE      
	0,
	0x2C, // KC_SYSRQ       
	0xA5, // KC_RMENU       
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0x13, // KC_PAUSE       
	0,
	0x24, // KC_HOME        
	0x26, // KC_UP          
	0x21, // KC_PGUP        
	0,
	0x25, // KC_LEFT        
	0,
	0x27, // KC_RIGHT       
	0,
	0x23, // KC_END         
	0x28, // KC_DOWN        
	0x22, // KC_PGDOWN      
	0x2D, // KC_INSERT      
	0x2E, // KC_DELETE      
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0x5B, // KC_LWIN        
	0x5C, // KC_RWIN        
	0x5D, // KC_APPS        
	0, // KC_POWER       
	0x5F, // KC_SLEEP       
	0,
	0,
	0,
	0, // KC_WAKE        
	0,
	0, // KC_WEBSEARCH   
	0, // KC_WEBFAVORITES
	0, // KC_WEBREFRESH  
	0, // KC_WEBSTOP     
	0, // KC_WEBFORWARD  
	0, // KC_WEBBACK     
	0, // KC_MYCOMPUTER  
	0, // KC_MAIL        
	0, // KC_MEDIASELECT 
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

static bool IsKeyPad(const OIS::KeyEvent& evt)
{
	switch (evt.key)
	{
	case OIS::KC_NUMLOCK:
	case OIS::KC_NUMPAD0:
	case OIS::KC_NUMPAD1:
	case OIS::KC_NUMPAD2:
	case OIS::KC_NUMPAD3:
	case OIS::KC_NUMPAD4:
	case OIS::KC_NUMPAD5:
	case OIS::KC_NUMPAD6:
	case OIS::KC_NUMPAD7:
	case OIS::KC_NUMPAD8:
	case OIS::KC_NUMPAD9:
	case OIS::KC_NUMPADCOMMA:
	case OIS::KC_NUMPADENTER:
	case OIS::KC_NUMPADEQUALS:
	case OIS::KC_ADD:
	case OIS::KC_SUBTRACT:
	case OIS::KC_MULTIPLY:
	case OIS::KC_DIVIDE:
		return true;
	default:
		return false;
	}
}

namespace Ogre
{

	bool CoherentUIInputConvertor::ConvertToCoherentKeyEvent(const OIS::KeyEvent& evt, CoherentUIInputConvertor::KeyState state, Coherent::UI::KeyEventData& outKeyData)
	{
		if (evt.key < 0 || evt.key >= sizeof(OIS_TO_WIN_KEYMAP) / sizeof(OIS_TO_WIN_KEYMAP[0]))
		{
			return false;
		}

		int kc = OIS_TO_WIN_KEYMAP[evt.key];
		if (kc == 0)
		{
			return false;
		}

		if (evt.device->type() != OIS::OISKeyboard)
		{
			return false;
		}

		const OIS::Keyboard* kbd = reinterpret_cast<const OIS::Keyboard*>(evt.device);

		outKeyData.KeyCode = kc;

		switch (state)
		{
		case CoherentUIInputConvertor::KeyDown:
			outKeyData.Type = Coherent::UI::KeyEventData::KeyDown;
			break;
		case CoherentUIInputConvertor::KeyUp:
			outKeyData.Type = Coherent::UI::KeyEventData::KeyUp;
			break;
		}

		if (kbd->isModifierDown(OIS::Keyboard::Shift))
		{
			outKeyData.Modifiers.IsShiftDown = true;
		}
		if (kbd->isModifierDown(OIS::Keyboard::Ctrl))
		{
			outKeyData.Modifiers.IsCtrlDown = true;
		}
		if (kbd->isModifierDown(OIS::Keyboard::Alt))
		{
			if (!(HAS_ALTGR && kbd->isKeyDown(OIS::KC_RMENU)))
			{
				outKeyData.Modifiers.IsAltDown = true;
			}
		}

		//result.IsAutoRepeat = false; // TODO: Auto-repeat logic
		if (IsKeyPad(evt))
			outKeyData.IsNumPad = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (::GetKeyState(VK_NUMLOCK) & 1)
		{
			outKeyData.Modifiers.IsNumLockOn = true;
		}
		if (::GetKeyState(VK_CAPITAL) & 1)
		{
			outKeyData.Modifiers.IsCapsOn = true;
		}
#endif

		return true;
	}

	bool CoherentUIInputConvertor::ConvertToCoherentMouseEvent(const OIS::MouseEvent& evt, OIS::MouseButtonID id, CoherentUIInputConvertor::MouseState state, Coherent::UI::MouseEventData& outMouseData)
	{
		if (evt.device->type() != OIS::OISMouse)
		{
			return false;
		}

		switch (id)
		{
		case OIS::MB_Left:
			outMouseData.Button = Coherent::UI::MouseEventData::ButtonLeft;
			break;
		case OIS::MB_Right:
			outMouseData.Button = Coherent::UI::MouseEventData::ButtonRight;
			break;
		case OIS::MB_Middle:
			outMouseData.Button = Coherent::UI::MouseEventData::ButtonMiddle;
			break;
		default:
			outMouseData.Button = Coherent::UI::MouseEventData::ButtonNone;
			break;
		}

		if (state == CoherentUIInputConvertor::MouseDown)
		{
			outMouseData.Type = Coherent::UI::MouseEventData::MouseDown;
		}
		else if (state == CoherentUIInputConvertor::MouseUp)
		{
			outMouseData.Type = Coherent::UI::MouseEventData::MouseUp;
		}
		else if (state == CoherentUIInputConvertor::MoveOrWheel)
		{
			if (evt.state.Z.rel != 0)
			{
				outMouseData.Type = Coherent::UI::MouseEventData::MouseWheel;
			}
			else
			{
				outMouseData.Type = Coherent::UI::MouseEventData::MouseMove;

				if (evt.state.buttonDown(OIS::MB_Left))
				{
					outMouseData.Button = Coherent::UI::MouseEventData::ButtonLeft;
				}
				else if (evt.state.buttonDown(OIS::MB_Right))
				{
					outMouseData.Button = Coherent::UI::MouseEventData::ButtonRight;
				}
				else if (evt.state.buttonDown(OIS::MB_Middle))
				{
					outMouseData.Button = Coherent::UI::MouseEventData::ButtonMiddle;
				}
			}
		}

		outMouseData.X = evt.state.X.abs;
		outMouseData.Y = evt.state.Y.abs;

		const OIS::Mouse* mouse = reinterpret_cast<const OIS::Mouse*>(evt.device);

		outMouseData.MouseModifiers.IsLeftButtonDown = evt.state.buttonDown(OIS::MB_Left);
		outMouseData.MouseModifiers.IsRightButtonDown = evt.state.buttonDown(OIS::MB_Right);
		outMouseData.MouseModifiers.IsMiddleButtonDown = evt.state.buttonDown(OIS::MB_Middle);

		if (evt.state.Z.rel != 0)
		{
			outMouseData.WheelY = evt.state.Z.rel / 120.0f;
		}

		return true;
	}
}