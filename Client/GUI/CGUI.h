//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#pragma once
#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800
#include <list>
#include "../d3d8/include/d3d8.h"
#include "../d3d8/include/d3dx8.h"
#include <CEGUI.h>
#include <dinput.h>
#include <RendererModules/Direct3D8/CEGUIDirect3D8Renderer.h>
//#include "CDirectInput8Proxy.h"

// TODO: Rename these to DIB (DIMB?) (DIM?) (Same as DIK_)
#define DIMOUSE_LEFTBUTTON   0
#define DIMOUSE_RIGHTBUTTON  1
#define DIMOUSE_MIDDLEBUTTON 2
#define DIMOUSE_4BUTTON      3
#define DIMOUSE_5BUTTON      4
#define DIMOUSE_6BUTTON      5
#define DIMOUSE_7BUTTON      6
#define DIMOUSE_8BUTTON      7

// Using this causes the intellisense to not recognize the new struct which means no syntax highlighting :(
//#define InheritedStructTypedef(inheritFrom, structName) typedef struct structName : inheritFrom { }
//InheritedStructTypedef(CEGUI::Window, CGUIButton);
// TODO: These should all be the cegui types
// e.g. typedef CGUITitleBar CEGUI::TitleBar?
struct CGUITitleBar : CEGUI::Window {};
struct CGUIButton : CEGUI::Window {};
struct CGUIFrameWindow : CEGUI::Window {};
struct CGUIEditBox : CEGUI::Window {};
struct CGUIStaticImage : CEGUI::Window {};
struct CGUIStaticText : CEGUI::Window {};
struct CGUICheckBox : CEGUI::Window {};
struct CGUIMultiColumnList : CEGUI::Window {};

class CGUI
{
private:
	IDirect3DDevice8 * m_pD3DDevice;
	CEGUI::Direct3D8Renderer * m_pRenderer;
	CEGUI::System * m_pSystem;
	CEGUI::Scheme * m_pScheme;
	CEGUI::MouseCursor * m_pCursor;
	CEGUI::WindowManager * m_pWindowManager;
	CEGUI::DefaultWindow * m_pDefaultWindow;
	CEGUI::Font * m_pDefaultFont;
	CEGUI::Font * m_pTahomaBoldFont;
	CEGUI::Font * m_pTahomaBoldFont10;
	CEGUI::GeometryBuffer * m_pTextDrawingGeometryBuffer;

	struct
	{
		CGUIFrameWindow * pWindow;
		CGUIStaticText * pText;
		CGUIButton * pButtons[3];
		CEGUI::Event::Subscriber * pEventSubscriber;
	} m_messageBox;

	IDirectInput8 * m_pInput;
	IDirectInputDevice8 * m_pInputMouse;
	bool m_bInitialized;
	DWORD m_dwInputAxes;
	DWORD m_dwInputButtons;
	DIMOUSESTATE2 m_MouseState;
	DWORD dwLastClickTime[8];
	BYTE byteButtonWasClicked[8];
	BYTE byteButtonClicked[8];
	D3DDISPLAYMODE m_DisplayMode;
	RECT m_rCursorPosition;
	int m_iCurrentId;
	DWORD m_dwDoubleClickTime;
	POINT m_clickPosition;
	int m_iCurrentKyeFag;

public:
	CGUI(IDirect3DDevice8 * pD3DDevice);
	~CGUI();

	char * GetUniqueName();
	bool Initialize();
	void Render();
	void DrawText(CEGUI::String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect = CEGUI::colour(0xFFFFFFFF), CEGUI::Font * pFont = NULL, bool bProcessFormatting = true, bool bAllowColorFormatting = true, CEGUI::Rect * rClipRect = NULL, float fSpaceExtra = 0.0f, float fXScale = 1.0f, float fYScale = 1.0f);
	void DrawText(CEGUI::String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, CEGUI::String sFontName, bool bProcessFormatting = true, bool bAllowColorFormatting = true, CEGUI::Rect * rClipRect = NULL, float fSpaceExtra = 0.0f, float fXScale = 1.0f, float fYScale = 1.0f);
	bool OnMessageBoxOkClick(const CEGUI::EventArgs &eventArgs);
	void ShowMessageBox(const CEGUI::String &sText, const CEGUI::String &sTitle = "", CEGUI::Event::Subscriber * pEventSubscriber = NULL);
	void HideMessageBox();
	CEGUI::Window * CreateGUIWindow(const CEGUI::String &sType, const CEGUI::String &sName = "", CEGUI::Window * pParentWindow = NULL);
	void RemoveGUIWindow(CEGUI::String &sName);
	void RemoveGUIWindow(CEGUI::Window * pWindow);
	void RemoveGUIWindow(CGUITitleBar * pTitleBar);
	void RemoveGUIWindow(CGUIButton * pButton);
	void RemoveGUIWindow(CGUIFrameWindow * pFrameWindow);
	void RemoveGUIWindow(CGUIEditBox * pEditBox);
	void RemoveGUIWindow(CGUIStaticImage * pStaticText);
	void RemoveGUIWindow(CGUIStaticText * pStaticText);
	void RemoveGUIWindow(CGUICheckBox * pCheckBox);
	CGUITitleBar * CreateGUITitleBar(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUITitleBar * CreateGUITitleBar(CEGUI::Window * pParentWindow = NULL);
	CGUIButton * CreateGUIButton(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIButton * CreateGUIButton(CEGUI::Window * pParentWindow = NULL);
	CGUIFrameWindow * CreateGUIFrameWindow(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIFrameWindow * CreateGUIFrameWindow(CEGUI::Window * pParentWindow = NULL);
	CGUIEditBox * CreateGUIEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIEditBox * CreateGUIEditBox(CEGUI::Window * pParentWindow = NULL);
	CGUIStaticImage * CreateGUIStaticImage(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIStaticImage * CreateGUIStaticImage(CEGUI::Window * pParentWindow = NULL);
	CGUIStaticText * CreateGUIStaticText(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIStaticText * CreateGUIStaticText(CEGUI::Window * pParentWindow = NULL);
	CGUICheckBox * CreateGUICheckBox(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUICheckBox * CreateGUICheckBox(CEGUI::Window * pParentWindow = NULL);
	CGUIMultiColumnList * CreateGUIMultiColumnList(CEGUI::String &sName, CEGUI::Window * pParentWindow = NULL);
	CGUIMultiColumnList * CreateGUIMultiColumnList(CEGUI::Window * pParentWindow = NULL);
	void SetCursorVisible(bool bVisible);
	bool IsCursorVisible();
	void OnLostDevice();
	void OnResetDevice();
	void MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool OnGUIKeyDown(const CEGUI::EventArgs &eventArgs);
	bool IsInitialized();
	D3DDISPLAYMODE GetDisplayMode();
	CEGUI::Direct3D8Renderer * GetRenderer();
	CEGUI::System * GetSystem();
	CEGUI::WindowManager * GetWindowManager();
	CEGUI::DefaultWindow * GetDefaultWindow();
	CEGUI::Font * GetDefaultFont();
	CEGUI::Font * GetTahomaBoldFont();
	CEGUI::Font * GetTahomaBold10Font();
};