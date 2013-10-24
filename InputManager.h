#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__
#include "cocos2d.h"
#include "HelperFunctions.h"
// Created by Sercan Degirmenci on 03.02.2013

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <dinput.h>
namespace cocos2d { namespace extension {
	#define MAX_GAMEPAD 4
	enum ButtonState
	{
		Button_None = 0,
		Button_Pressed ,
		Button_Holded,
		Button_Released
	};
	class InputManager
	{
	public:
		static void Initialize();
		static void SetMouseEnable(bool value);
		static void SetKeyboardEnable(bool value);
		static void SetGamepadEnable(bool value);
		static bool IsMouseEnabled();
		static bool IsKeyboardEnabled();
		static bool IsGamepadEnabled();
		static void Release();
	};
	class Gamepad
	{
	public:
		static bool IsKeyReleased(int gpIndex, int key);
		static bool IsKeyPressed(int gpIndex,int key);
		static bool IsKeyHolded(int gpIndex,int key);
		static bool IsKeyPressedOrHolded(int gpIndex,int key);
		static bool IsActive(int gpIndex);
		static cocos2d::CCPoint GetLeftStick(int gpIndex);
		static cocos2d::CCPoint GetRightStick(int gpIndex);
		static std::string GetName(int gpIndex);
		static ButtonState GetButtonState(int gpIndex,int key);
	};
	class Mouse
	{
	public:
		static ButtonState LeftButtonState();
		static ButtonState RightButtonState();
		static ButtonState MiddleButtonState();
		static float GetMouseWheelDelta();
		static cocos2d::CCPoint GetMousePosition();
		static cocos2d::CCPoint GetMousePositionDelta();
	};
	class Keyboard
	{
	public:
		static bool IsKeyReleased(int key);
		static bool IsKeyPressed(int key);
		static bool IsKeyHolded(int key);
		static bool IsKeyPressedOrHolded(int key);
		static ButtonState GetButtonState(int key);
	};
	struct GamepadKeys
	{
		enum Keys
		{
			Button01 = 0,
			Button02,
			Button03,
			Button04,
			Button05,
			Button06,
			Button07,
			Button08,
			Button09,
			Button10,
			Button11,
			Button12,
			LTUp,
			LTRight,
			LTDown,
			LTLeft,
			LRUp,
			LRRight,
			LRDown,
			LRLeft,
			DPadUp,
			DPadRight,
			DPadDown,
			DPadLeft
		};
	};
	struct KeyboardKeys
	{
		enum Keys
		{
			Space = DIK_SPACE,
			Up = DIK_UP,
			Down = DIK_DOWN,
			Left = DIK_LEFT,
			Right = DIK_RIGHT,
			Home = DIK_HOME,
			Insert = DIK_INSERT,
			Delete = DIK_DELETE,
			PageDown = DIK_NEXT,
			PageUp = DIK_PRIOR,
			End = DIK_END,
			NumpadEnter = DIK_NUMPADENTER,
			Add = DIK_ADD,
			Number0 = DIK_0,
			Number1 = DIK_1,
			Number2 = DIK_2,
			Number3 = DIK_3,
			Number4 = DIK_4,
			Number5 = DIK_5,
			Number6 = DIK_6,
			Number7 = DIK_7,
			Number8 = DIK_8,
			Number9 = DIK_9,
			F1 = DIK_F1,
			F2 = DIK_F2,
			F3 = DIK_F3,
			F4 = DIK_F4,
			F5 = DIK_F5,
			F6 = DIK_F6,
			F7 = DIK_F7,
			F8 = DIK_F8,
			F9 = DIK_F9,
			F10 = DIK_F10,
			F11 = DIK_F11,
			F12 = DIK_F12,
			NumberPad0 = DIK_NUMPAD0,
			NumberPad1 = DIK_NUMPAD1,
			NumberPad2 = DIK_NUMPAD2,
			NumberPad3 = DIK_NUMPAD3,
			NumberPad4 = DIK_NUMPAD4,
			NumberPad5 = DIK_NUMPAD5,
			NumberPad6 = DIK_NUMPAD6,
			NumberPad7 = DIK_NUMPAD7,
			NumberPad8 = DIK_NUMPAD8,
			NumberPad9 = DIK_NUMPAD9,
			Tab = DIK_TAB,
			Back = DIK_BACK,
			Equals = DIK_EQUALS,
			Enter = DIK_RETURN,
			Subtract = DIK_SUBTRACT,
			LeftControl = DIK_LCONTROL,
			RightControl = DIK_RCONTROL,
			LeftShift = DIK_LSHIFT,
			RightShift = DIK_RSHIFT,
			LeftAlt = DIK_LMENU,
			RightAlt = DIK_RMENU,
			Escape = DIK_ESCAPE,
			A = DIK_A,
			B = DIK_B,
			C = DIK_C,
			D = DIK_D,
			E = DIK_E,
			F = DIK_F,
			G = DIK_G,
			H = DIK_H,
			I = DIK_I,
			J = DIK_J,
			K = DIK_K,
			L = DIK_L,
			M = DIK_M,
			N = DIK_N,
			O = DIK_O,
			P = DIK_P,
			R = DIK_R,
			S = DIK_S,
			T = DIK_T,
			U = DIK_U,
			V = DIK_V,
			Y = DIK_Y,
			Z = DIK_Z,
			X = DIK_X,
			Q = DIK_Q,
			W = DIK_W
		};
	};
}}
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#endif //__INPUTMANAGER_H__