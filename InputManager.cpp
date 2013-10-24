#include "InputManager.h"
#include "UtfConverter.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <dinput.h>
#include <dinputd.h>
#include <windows.h>
#include <wbemidl.h>
#include <Xinput.h>

using namespace cocos2d;
using namespace cocos2d::extension;

#pragma region InputManager
static void InputUpdate();
static void GamepadUpdate();
static void KeyboardUpdate();
static void MouseUpdate();

static HRESULT GamepadInitialize();
static void KeyboardInitialize();
static void MouseInitialize();

static bool m_mouseEnabled = true;
static bool m_keyboardEnabled = true;
static bool m_gamepadEnabled = true;

static LPDIRECTINPUT8 din ;
static LPDIRECTINPUTDEVICE8 dinmouse;
static LPDIRECTINPUTDEVICE8 dinkeyboard;
static LPDIRECTINPUTDEVICE8 dingamepad[MAX_GAMEPAD];
static int numGamepad = 0;		

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
struct DI_ENUM_CONTEXT
{
    DIJOYCONFIG* pPreferredJoyCfg;
    bool bPreferredJoyCfgValid;
};
struct XINPUT_DEVICE_NODE
{
    DWORD dwVidPid;
    XINPUT_DEVICE_NODE* pNext;
};
class inputUpdateClass:public CCObject
{
public:
	void iupdate(float dt){InputUpdate();}
};
static inputUpdateClass* cls;
void InputManager::Initialize()
{
	cls = new inputUpdateClass();
	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(inputUpdateClass::iupdate),cls,0,false);
	DirectInput8Create(HINST_THISCOMPONENT,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&din,NULL);
	KeyboardInitialize();
	MouseInitialize();
	if(FAILED(GamepadInitialize()))
	{
		SetGamepadEnable(false);
	}
}

void InputManager::SetMouseEnable(bool value)
{
	m_mouseEnabled = value;
}

void InputManager::SetKeyboardEnable(bool value)
{
	m_keyboardEnabled = value;
}

void InputManager::SetGamepadEnable(bool value)
{
	m_gamepadEnabled = value;
}

bool InputManager::IsMouseEnabled()
{
	return m_mouseEnabled;
}

bool InputManager::IsKeyboardEnabled()
{
	return m_keyboardEnabled;
}

bool InputManager::IsGamepadEnabled()
{
	return m_gamepadEnabled;
}
void InputManager::Release()
{
	dinmouse->Unacquire();
	dinkeyboard->Unacquire();

	// Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
	for (int i = 0; i < numGamepad; i++)
	{
		if( dingamepad[i] )
			dingamepad[i]->Unacquire();
		    SAFE_RELEASE( dingamepad[i] );
	}
    // Release any DirectInput objects.

}


void InputUpdate()
{
	if(m_gamepadEnabled)
		GamepadUpdate();
	if(m_keyboardEnabled)
		KeyboardUpdate();
	if(m_mouseEnabled)
		MouseUpdate();
}

#pragma endregion

#pragma region Mouse
static DIMOUSESTATE mouseCurrentState;
static DIMOUSESTATE mouseOldState;
static bool MouseIsKeyDown(int key)
{
	return mouseCurrentState.rgbButtons[key] & 0x80;
}
static bool MouseIsKeyUp(int key)
{
	return !(mouseCurrentState.rgbButtons[key] & 0x80);
}
static bool MouseIsOldKeyDown(int key)
{
	return mouseOldState.rgbButtons[key] & 0x80;
}
static bool MouseIsOldKeyUp(int key)
{
	return !(mouseOldState.rgbButtons[key] & 0x80);
} 
POINT m_p;
ButtonState Mouse::LeftButtonState()
{
	if( MouseIsKeyUp(0) && MouseIsOldKeyDown(0)) //RELEASE
		return Button_Released;
	else if(MouseIsKeyDown(0) && MouseIsOldKeyUp(0))  //PRESSED
		return Button_Pressed;
	else if(MouseIsKeyDown(0) && MouseIsOldKeyDown(0)) //HOLDED
		return Button_Holded;
	return Button_None;                //NONE
}
ButtonState Mouse::RightButtonState()
{	
	if( MouseIsKeyUp(1) && MouseIsOldKeyDown(1)) //RELEASE
		return Button_Released;
	else if(MouseIsKeyDown(1) && MouseIsOldKeyUp(1))  //PRESSED
		return Button_Pressed;
	else if(MouseIsKeyDown(1) && MouseIsOldKeyDown(1)) //HOLDED
		return Button_Holded;
	return Button_None;                //NONE
}
ButtonState Mouse::MiddleButtonState()
{	
	if( MouseIsKeyUp(2) && MouseIsOldKeyDown(2)) //RELEASE
		return Button_Released;
	else if(MouseIsKeyDown(2) && MouseIsOldKeyUp(2))  //PRESSED
		return Button_Pressed;
	else if(MouseIsKeyDown(2) && MouseIsOldKeyDown(2)) //HOLDED
		return Button_Holded;
	return Button_None;                //NONE
}
float Mouse::GetMouseWheelDelta()
{
	return mouseCurrentState.lZ;
}
cocos2d::CCPoint Mouse::GetMousePosition()
{
	return ccp(m_p.x,m_p.y);
}
cocos2d::CCPoint Mouse::GetMousePositionDelta()
{
	return ccp(mouseCurrentState.lX, mouseCurrentState.lY);
}

void MouseUpdate()
{
	mouseOldState=mouseCurrentState;
	dinmouse->Acquire();
	dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseCurrentState);

	GetCursorPos(&m_p);
	//XMVECTOR pos=XMVectorSet(p.x,p.y,0,0);
	//pos=XMVector3Unproject(pos,GameWindowRect.left,GameWindowRect.top,
	//	GameWindowRect.right-GameWindowRect.left,
	//	GameWindowRect.top-GameWindowRect.bottom,
	//	0,1,Projection,StillaEngine::IdentityMatrix,StillaEngine::IdentityMatrix);
	//X=XMVectorGetX(pos);
	//Y=-XMVectorGetY(pos);
	//
	//DeltaX=mouseCurrentState.lX;
	//DeltaY=mouseCurrentState.lY;
	//MouseWheel=(float)mouseCurrentState.lZ;
}
void MouseInitialize()
{
	din->CreateDevice(GUID_SysMouse,&dinmouse,NULL);	
	dinmouse->SetDataFormat(&c_dfDIMouse);
	dinmouse->SetCooperativeLevel(CCEGLView::sharedOpenGLView()->getHWnd(),DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
}
#pragma endregion

#pragma region Keyboard

static BYTE keyCurrentstate[256];
static BYTE keyOldstate[256];
static bool KeyboardIsOldKeyDown(int key)
{
	return keyOldstate[key] & 0x80;
}
static bool KeyboardIsOldKeyUp(int key)
{
	return !(keyOldstate[key] & 0x80);
}
static bool KeyboardIsKeyDown(int key)
{
	return keyCurrentstate[key] & 0x80;
}
static bool KeyboardIsKeyUp(int key)
{
	return !(keyCurrentstate[key] & 0x80);
}
bool Keyboard::IsKeyReleased(int key)
{
	return (KeyboardIsKeyUp(key) && KeyboardIsOldKeyDown(key));
}
bool Keyboard::IsKeyPressed(int key)
{
	return (KeyboardIsKeyDown(key) && KeyboardIsOldKeyUp(key));
}
bool Keyboard::IsKeyHolded(int key)
{
	return (KeyboardIsKeyDown(key) && KeyboardIsOldKeyDown(key));
}
bool Keyboard::IsKeyPressedOrHolded(int key)
{
	return (IsKeyPressed(key) || IsKeyHolded(key));
}
ButtonState Keyboard::GetButtonState(int key)
{
	if(IsKeyReleased(key))
		return Button_Released;
	else if(IsKeyPressed(key))
		return Button_Pressed;
	else if(IsKeyHolded(key))
		return Button_Holded;
	return Button_None;
}
void KeyboardUpdate()
{
	for (int i = 0; i < 256; i++)
	{
		keyOldstate[i] = keyCurrentstate[i];
	}
	dinkeyboard->Acquire();
	dinkeyboard->GetDeviceState(256,(LPVOID)keyCurrentstate);
}
void KeyboardInitialize()
{
	din->CreateDevice(GUID_SysKeyboard,&dinkeyboard,NULL);

	dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
	dinkeyboard->SetCooperativeLevel(CCEGLView::sharedOpenGLView()->getHWnd(),DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	for (int i = 0; i < 256; i++)
	{
		keyCurrentstate[i] = 0;
		keyOldstate[i] = 0;
	}

}

#pragma endregion

#pragma region Gamepad
static DIJOYSTATE2 gamepadOldState[MAX_GAMEPAD];
static DIJOYSTATE2 gamepadCurrentState[MAX_GAMEPAD];
static std::map<int,int> buttons[MAX_GAMEPAD];
static std::string gamepadNames[MAX_GAMEPAD];
bool                    g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE*     g_pXInputDeviceList = NULL;
HRESULT SetupForIsXInputDevice();
bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );
void CleanupForIsXInputDevice();

static int getstate(int first,int second)
{
	if((first==0||first==3) && second==0)
		return 0;//None
	else if (second==0)
		return 3;//Released;
	if(first==0||first==3)
		return 1;//Pressed
	else if(first==1||first==2)
		return 2;//Holded

	return 0;//None
}
static bool GamepadIsOldKeyDown(int gpIndex,int key)
{
	if(gpIndex>=numGamepad)return false;
	return gamepadOldState[gpIndex].rgbButtons[key] & 0x80;
}
static bool GamepadIsOldKeyUp(int gpIndex,int key)
{
	if(gpIndex>=numGamepad)return false;
	return !(gamepadOldState[gpIndex].rgbButtons[key] & 0x80);
}
static bool GamepadIsKeyDown(int gpIndex,int key)
{
	if(gpIndex>=numGamepad)	return false;
	return (gamepadCurrentState[gpIndex].rgbButtons[key] & 0x80);
}
static bool GamepadIsKeyUp(int gpIndex,int key)
{
	if(gpIndex >= numGamepad)return false;
	return !(gamepadCurrentState[gpIndex].rgbButtons[key] & 0x80);
}
static void DPadUpdate()
{
	for (int i = 0; i < MAX_GAMEPAD; i++)
	{
		if(!dingamepad[i])
			continue;
		std::map<int,int>kom;
		for (int j = 12; j < 24; j++)
		{
			kom[j]=0;
		}
		switch(gamepadCurrentState[i].rgdwPOV[0])
		{
		case 0:
			kom[20]=1;
			break;
		case 4500:
			kom[20]=1;
			kom[21]=1;
			break;
		case 9000:
			kom[21]=1;
			break;
		case 13500:
			kom[21]=1;
			kom[22]=1;
			break;
		case 18000:
			kom[22]=1;
			break;
		case 22500:
			kom[22]=1;
			kom[23]=1;
			break;
		case 27000:			
			kom[23]=1;
			break;
		case 31500:			
			kom[23]=1;
			kom[20]=1;
			break;
		}
		CCPoint left = Gamepad::GetLeftStick(i);
		CCPoint right = Gamepad::GetRightStick(i);
		if(left.x < -0.2f)
			kom[15]=1;
		else if(left.x > 0.2f)
			kom[13]=1;
		if(left.y < -0.2f)
			kom[12]=1;
		else if(left.y > 0.2f)
			kom[14]=1;

		if(right.x < -0.2f)
			kom[19]=1;
		else if(right.x > 0.2f)
			kom[17]=1;
		if(right.y < -0.2f)
			kom[16]=1;
		else if(right.y > 0.2f)
			kom[18]=1;


		for(int j=12;j<25;j++)
		{
			buttons[i][j]=getstate(buttons[i][j],kom[j]);
		}
	}

}
bool Gamepad::IsKeyReleased(int gpIndex, int key)
{
	if(key>11 && key<<24)
		return buttons[gpIndex][key]==3;

	return (GamepadIsKeyUp(gpIndex,key) && GamepadIsOldKeyDown(gpIndex,key));}
bool Gamepad::IsKeyPressed(int gpIndex,int key)
{
	if(key>11 && key<<24)
		return buttons[gpIndex][key]==1;
	return (GamepadIsKeyDown(gpIndex,key) && GamepadIsOldKeyUp(gpIndex,key));
}
bool Gamepad::IsKeyHolded(int gpIndex,int key)
{
	if(key>11 && key<<24)
		return buttons[gpIndex][key]==2;
	return (GamepadIsKeyDown(gpIndex,key) && GamepadIsOldKeyDown(gpIndex,key));
}
bool Gamepad::IsKeyPressedOrHolded(int gpIndex,int key)
{
	if(key>11 && key<<24)
		return buttons[gpIndex][key]==1||buttons[gpIndex][key]==2;
	return (IsKeyPressed(gpIndex,key) || IsKeyHolded(gpIndex,key));
}
bool Gamepad::IsActive(int gpIndex)
{
	return gpIndex < numGamepad;
}
cocos2d::CCPoint Gamepad::GetLeftStick(int gpIndex)
{
	if(gpIndex >= numGamepad)
		return ccp(0,0);

	float x=(gamepadCurrentState[gpIndex].lX)/10;
	float y=(gamepadCurrentState[gpIndex].lY)/10;
	return ccp(x/100,y/100);
}
cocos2d::CCPoint Gamepad::GetRightStick(int gpIndex)
{
	if(gpIndex >= numGamepad)
		return ccp(0,0);
	float z=(gamepadCurrentState[gpIndex].lZ)/10;
	float rz=(gamepadCurrentState[gpIndex].lRz)/10;
	return ccp(z/100,rz/100);
}
std::string Gamepad::GetName(int gpIndex)
{
	return gamepadNames[gpIndex];
}
ButtonState Gamepad::GetButtonState(int gpIndex,int key)
{
	if(IsKeyReleased(gpIndex,key))
		return Button_Released;
	else if(IsKeyPressed(gpIndex,key))
		return Button_Pressed;
	else if(IsKeyHolded(gpIndex,key))
		return Button_Holded;
	return Button_None;
}
void GamepadUpdate()
{
	HRESULT hr;
	for (int i = 0; i < numGamepad; i++)
	{
		if( NULL == dingamepad[i] )
			continue;
		gamepadOldState[i]=gamepadCurrentState[i];
    // Poll the device to read the current state
    hr = dingamepad[i]->Poll();
    if( FAILED( hr ) )
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        hr = dingamepad[i]->Acquire();
        while( hr == DIERR_INPUTLOST )
            hr = dingamepad[i]->Acquire();

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
       
    }

    // Get the input's device state
    if( FAILED( hr = dingamepad[i]->GetDeviceState( sizeof( DIJOYSTATE2 ), &gamepadCurrentState[i] ) ) )
        return ; // The device should have been acquired during the Poll()
	}
	DPadUpdate();
}
//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
	numGamepad++;
    DI_ENUM_CONTEXT* pEnumContext = ( DI_ENUM_CONTEXT* )pContext;
    HRESULT hr;

    if( g_bFilterOutXinputDevices && IsXInputDevice( &pdidInstance->guidProduct ) )
        return DIENUM_CONTINUE;
	
    // Skip anything other than the perferred joystick device as defined by the control panel.  
    // Instead you could store all the enumerated joysticks and let the user pick.
    //if( pEnumContext->bPreferredJoyCfgValid &&
    //    !IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
    //    return DIENUM_CONTINUE;

    // Obtain an interface to the enumerated joystick.
	hr = din->CreateDevice( pdidInstance->guidInstance,&dingamepad[numGamepad-1], NULL );
	gamepadNames[numGamepad-1]=ToUtf8( pdidInstance->tszProductName);
    return DIENUM_CONTINUE;

}
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
    HWND hDlg = ( HWND )pContext;

    static int nSliderCount = 0;  // Number of returned slider controls
    static int nPOVCount = 0;     // Number of returned POV controls

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof( DIPROPRANGE );
        diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
		if( FAILED( (dingamepad[numGamepad-1])->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
            return DIENUM_STOP;
		
    }

    return DIENUM_CONTINUE;
}
HRESULT GamepadInitialize()
{
	for(int i=0 ; i < MAX_GAMEPAD ;i++)
	{
		gamepadNames[i] = "";
		for (int j = 12; j < 24; j++)
		{
			buttons[i][j]=0;
		}
	}

	HRESULT hr;
	if( g_bFilterOutXinputDevices )
        SetupForIsXInputDevice();
	HWND hWnd =CCEGLView::sharedOpenGLView()->getHWnd();
	DIJOYCONFIG PreferredJoyCfg = {0};
    DI_ENUM_CONTEXT enumContext;
    enumContext.pPreferredJoyCfg = &PreferredJoyCfg;
    enumContext.bPreferredJoyCfgValid = false;
    IDirectInputJoyConfig8* pJoyConfig = NULL;
    hr = din->QueryInterface( IID_IDirectInputJoyConfig8, ( void** )&pJoyConfig );
	PreferredJoyCfg.dwSize = sizeof( PreferredJoyCfg );

	if( SUCCEEDED( pJoyConfig->GetConfig( 0, &PreferredJoyCfg, DIJC_GUIDINSTANCE ) ) ) // This function is expected to fail if no joystick is attached
        enumContext.bPreferredJoyCfgValid = true;
    SAFE_RELEASE( pJoyConfig );

    // Look for a simple joystick we can use for this sample program.
	if( FAILED( hr = din->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                         EnumJoysticksCallback,
										 &enumContext, DIEDFL_ALLDEVICES ) ) )
        return hr;

    if( g_bFilterOutXinputDevices )
        CleanupForIsXInputDevice();

    
	    // Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	for (int i = 0; i < numGamepad; i++)
	{
		if(dingamepad[i]==0)
			continue;

    if( FAILED( hr = dingamepad[i]->SetDataFormat( &c_dfDIJoystick2 ) ) )
        return hr;

    // Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
	if( FAILED( hr = dingamepad[i]->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE |
                                                       DISCL_FOREGROUND ) ) )
        return hr;

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    if( FAILED( hr = dingamepad[i]->EnumObjects( EnumObjectsCallback,
                                               ( VOID* )hWnd, DIDFT_ALL ) ) )
        return hr;
	}
    return S_OK;
}

HRESULT SetupForIsXInputDevice()
{
    IWbemServices* pIWbemServices = NULL;
    IEnumWbemClassObject* pEnumDevices = NULL;
    IWbemLocator* pIWbemLocator = NULL;
    IWbemClassObject* pDevices[20] = {0};
    BSTR bstrDeviceID = NULL;
    BSTR bstrClassName = NULL;
    BSTR bstrNamespace = NULL;
    DWORD uReturned = 0;
    bool bCleanupCOM = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize( NULL );
    bCleanupCOM = SUCCEEDED( hr );

    // Create WMI
    hr = CoCreateInstance( __uuidof( WbemLocator ),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof( IWbemLocator ),
                           ( LPVOID* )&pIWbemLocator );
    if( FAILED( hr ) || pIWbemLocator == NULL )
        goto LCleanup;

    // Create BSTRs for WMI
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
    bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L,
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED( hr ) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

    // Get list of Win32_PNPEntity devices
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
    if( FAILED( hr ) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for(; ; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED( hr ) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice = 0; iDevice < uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it’s an XInput device
                // Unfortunately this information can not be found by just using DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );

                    // Add the VID/PID to a linked list
                    XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
                    if( pNewNode )
                    {
                        pNewNode->dwVidPid = dwVidPid;
                        pNewNode->pNext = g_pXInputDeviceList;
                        g_pXInputDeviceList = pNewNode;
                    }
                }
            }
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if( bstrNamespace )
        SysFreeString( bstrNamespace );
    if( bstrDeviceID )
        SysFreeString( bstrDeviceID );
    if( bstrClassName )
        SysFreeString( bstrClassName );
    for( iDevice = 0; iDevice < 20; iDevice++ )
    SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    return hr;
}

bool IsXInputDevice( const GUID* pGuidProductFromDirectInput )
{
    // Check each xinput device to see if this device's vid/pid matches
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode )
    {
        if( pNode->dwVidPid == pGuidProductFromDirectInput->Data1 )
            return true;
        pNode = pNode->pNext;
    }
	
    return false;
}

void CleanupForIsXInputDevice()
{
    // Cleanup linked list
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode )
    {
        XINPUT_DEVICE_NODE* pDelete = pNode;
        pNode = pNode->pNext;
        SAFE_DELETE( pDelete );
    }
}

#pragma endregion


#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32