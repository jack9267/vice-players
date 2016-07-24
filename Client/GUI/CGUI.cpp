//----------------------------------------------------------
//
// VC:MP Multiplayer Modification For GTA:VC
// Copyright 2004-2005 SA:MP team
//
// File Author(s): jenksta
// License: See LICENSE in root directory
//
//----------------------------------------------------------

#include "StdInc.h"

#define LOG_PATH "CEGUI.log"
#define STYLE_SCHEME "VanillaSkin.scheme"
//#define STYLE_SCHEME "WindowsLook.scheme"
#define STYLE_PREFIX "Vanilla"
//#define STYLE_PREFIX "WindowsLook"
#define STYLE_IMAGES "Vanilla-Images"
//#define STYLE_IMAGES "WindowsLook"

extern HWND hWND;

CGUI::CGUI(IDirect3DDevice8 * pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
	m_pRenderer = NULL;
	m_pSystem = NULL;
	m_pCursor = NULL;
	m_pWindowManager = NULL;
	m_pDefaultWindow = NULL;
	m_pDefaultFont = NULL;
	memset(&m_messageBox, 0, sizeof(m_messageBox));
	m_pInput = NULL;
	m_pInputMouse = NULL;
	m_bInitialized = false;
	memset(dwLastClickTime, 0, sizeof(DWORD) * 8);
	memset(byteButtonWasClicked, 0, sizeof(BYTE) * 8);
	memset(byteButtonClicked, 0, sizeof(BYTE) * 8);
	memset(&m_rCursorPosition, 0, sizeof(RECT));
	m_iCurrentId = 0;
	m_dwDoubleClickTime = GetDoubleClickTime();
	memset(&m_clickPosition, 0, sizeof(POINT));
	m_iCurrentKyeFag = 0;
}

CGUI::~CGUI()
{
	// Destroy the message box
	HideMessageBox();

	// Mark ourselves as uninitialized so functions don't work
	m_bInitialized = false;

	// Shutdown the input device
	if(m_pInputMouse)
	{
		m_pInputMouse->Unacquire();
		m_pInputMouse->Release();
	}

	// Shutdown the input
	if(m_pInput)
		m_pInput->Release();

	// Destroy the text drawing geometry buffer
	m_pRenderer->destroyGeometryBuffer(*m_pTextDrawingGeometryBuffer);

	// Destroy the gui fonts
	CEGUI::FontManager::getSingleton().destroy(*m_pTahomaBoldFont10);
	CEGUI::FontManager::getSingleton().destroy(*m_pTahomaBoldFont);
	CEGUI::FontManager::getSingleton().destroy(*m_pDefaultFont);

	// Destroy the default gui window
	m_pWindowManager->destroyWindow(m_pDefaultWindow);

	// Destroy the gui scheme
	CEGUI::SchemeManager::getSingleton().destroy(*m_pScheme);

	// Destroy the gui system
	m_pSystem->destroy();

	// Destroy the gui renderer
	m_pRenderer->destroy(*m_pRenderer);

}

char * CGUI::GetUniqueName()
{
	static char szKyeFag[20];
	sprintf(szKyeFag, "kyefag%d", m_iCurrentKyeFag);
	m_iCurrentKyeFag++;
	return szKyeFag;
}

bool CGUI::Initialize()
{
	// Make sure we are not initialized
	if(!m_bInitialized)
	{
		// Create the gui renderer
		m_pRenderer = &CEGUI::Direct3D8Renderer::create(m_pD3DDevice);

		// Create the gui system
		m_pSystem = &CEGUI::System::create(*m_pRenderer, 0, 0, 0, 0, "", "CEGUI.log");

		// Get a pointer to the gui window manager
		m_pWindowManager = CEGUI::WindowManager::getSingletonPtr();

		// Initialize the required dirs for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider * rp = (CEGUI::DefaultResourceProvider *)m_pSystem->getResourceProvider();

		char resourcePath[MAX_PATH];

		// For each resource type, set a resource group directory
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/schemes/");
		rp->setResourceGroupDirectory("schemes", resourcePath);
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/imagesets/");
		rp->setResourceGroupDirectory("imagesets", resourcePath);
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/fonts/");
		rp->setResourceGroupDirectory("fonts", resourcePath);
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/layouts/");
		rp->setResourceGroupDirectory("layouts", resourcePath);
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/looknfeel/");
		rp->setResourceGroupDirectory("looknfeels", resourcePath);
		sprintf(resourcePath, "%s%s", "", "VC-P/CEGUI/xml_schemas/");
		rp->setResourceGroupDirectory("schemas", resourcePath);

		// Set the default resource groups to be used
		CEGUI::Imageset::setDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		m_pWindowManager->setDefaultResourceGroup("layouts");

		// Setup default group for validation schemas
		CEGUI::XMLParser * parser = m_pSystem->getXMLParser();

		if(parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");

		// Clearing this queue actually makes sure it's created
		m_pRenderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

		// Set the gui logging level
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

		// Load the gui scheme
		m_pScheme = &CEGUI::SchemeManager::getSingleton().create(STYLE_SCHEME);

		// Set the mouse cursor
		m_pSystem->setDefaultMouseCursor(STYLE_IMAGES, "MouseArrow");

		// Get a pointer to the gui cursor
		m_pCursor = CEGUI::MouseCursor::getSingletonPtr();

		// Hide the mouse cursor
		m_pCursor->setVisible(false);

		// Create the default gui window
		m_pDefaultWindow = (CEGUI::DefaultWindow *)m_pWindowManager->createWindow("DefaultWindow", GetUniqueName());

		// Set the default gui window size
		m_pDefaultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

		// Subscribe to our global key down event
		CEGUI::GlobalEventSet::getSingletonPtr()->subscribeEvent(CEGUI::Window::EventNamespace + "/" + CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&CGUI::OnGUIKeyDown, this));

		// Get the display mode size
		m_pD3DDevice->GetDisplayMode(&m_DisplayMode);

		// TODO: Size needs to be set again on d3d device reset
		// NOTE: Are both of these needed?
		/*m_pRenderer->setDisplaySize(CEGUI::Size((float)m_DisplayMode.Width, (float)m_DisplayMode.Height));*/
		/*m_pSystem->notifyDisplaySizeChanged(CEGUI::Size((float)m_DisplayMode.Width, (float)m_DisplayMode.Height));*/

		// Set the default gui window as the gui sheet
		m_pSystem->setGUISheet(m_pDefaultWindow);

		// Create the gui fonts
		//CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
		//CEGUI::FontManager::getSingleton().createFreeTypeFont("DejaVuSans-6", 6, false, "DejaVuSans.ttf", "", true/*, 600, 600*/);
		m_pDefaultFont = &CEGUI::FontManager::getSingleton().createFreeTypeFont("Tahoma", 8, true, "tahoma.ttf");
		m_pTahomaBoldFont = &CEGUI::FontManager::getSingleton().createFreeTypeFont("Tahoma-Bold", 8, true, "tahomabd.ttf");
		m_pTahomaBoldFont10 = &CEGUI::FontManager::getSingleton().createFreeTypeFont("Tahoma-Bold-10", 10, true, "tahomabd.ttf");

		// Set the default gui font (Is this needed?, CEGUI seems to use the first created font as default
		// (Which is what we want it to do))
		m_pSystem->setDefaultFont(m_pDefaultFont);

		// Setup text drawing geometry buffer
		m_pTextDrawingGeometryBuffer = &m_pRenderer->createGeometryBuffer();
		m_pTextDrawingGeometryBuffer->setClippingRegion(CEGUI::Rect(CEGUI::Vector2(0, 0), m_pRenderer->getDisplaySize()));

		// Create the input
		if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID *)&m_pInput, NULL)))
			return false;

		// Create the input device
		if(FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pInputMouse, NULL)))
			return false;

		// Set the input device as a mouse
		if(FAILED(m_pInputMouse->SetDataFormat(&c_dfDIMouse2)))
			return false;

		// Set the input device to non exclusive
		if(FAILED(m_pInputMouse->SetCooperativeLevel(0, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
			return false;

		// Acquire the input device
		if(FAILED(m_pInputMouse->Acquire()))
			return false;
		/*
		// Get the input device info
		DIDEVCAPS MouseCapabilities;
		MouseCapabilities.dwSize = sizeof(MouseCapabilities);

		if(FAILED(m_pInputMouse->GetCapabilities(&MouseCapabilities)))
			return false;

		m_dwInputAxes = MouseCapabilities.dwAxes;
		m_dwInputButtons = MouseCapabilities.dwButtons;

		// Initialize the mouse button info
		for(BYTE i = 0; i < 8; i++)
		{
			dwLastClickTime[i] = 0;
			byteButtonWasClicked[i] = 0;
			byteButtonClicked[i] = 0;
		}*/

		return m_bInitialized = true;
	}
	return false;
}

// TODO: Use the game's mouse dinput device not our own?
void CGUI::Render()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Render our gui
		if(m_pSystem)
		{
			m_pSystem->renderGUI();
		}

		// Process our mouse events
		if(m_pInput && m_pInputMouse)
		{
			if(m_pCursor->isVisible())
			{
				// TODO: Log when input device is lost
				// If we have lost the input device re-acquire it
				if(m_pInputMouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState) == DIERR_INPUTLOST)
				{
					if(FAILED(m_pInputMouse->Acquire()))
						return;
				}

				// If we have focus set the cursor position
				// to 0 so it don't interfere with any other
				// programs

				// Handle mouse position changes
				// Dunno if it can stay like this or if
				// i need to check for changes before i
				// set stuff.
				m_rCursorPosition.left += m_MouseState.lX;

				if(m_rCursorPosition.left > (long)m_DisplayMode.Width)
					m_rCursorPosition.left = m_DisplayMode.Width;
				else if(m_rCursorPosition.left < 0)
					m_rCursorPosition.left = 0;

				m_rCursorPosition.top += m_MouseState.lY;

				if(m_rCursorPosition.top > (long)m_DisplayMode.Height)
					m_rCursorPosition.top = m_DisplayMode.Height;
				else if(m_rCursorPosition.top < 0)
					m_rCursorPosition.top = 0;

				m_pSystem->injectMousePosition((float)m_rCursorPosition.left, (float)m_rCursorPosition.top);

				// Check the mouse wheel for changes
				// TODO: I don't think this is right, check it
				if(m_MouseState.lZ != 0)
					m_pSystem->injectMouseWheelChange((float)m_MouseState.lZ);

				// Check the mouse buttons for changes
				// TODO: Fix double click detection
				// TODO: First click after a double click should be a single click
				for(BYTE i = 0; i < 8; i++)
				{
					if(m_MouseState.rgbButtons[i] & 0x80)
					{
						if(!byteButtonClicked[i])
						{
							bool dc = false;
							if(dwLastClickTime[i])
							{
								if((GetTickCount() - dwLastClickTime[i]) < m_dwDoubleClickTime)
								{
									if(m_clickPosition.x == m_rCursorPosition.left && m_clickPosition.y == m_rCursorPosition.top)
									{
										if(i < 3)
										{
											m_pSystem->injectMouseButtonDoubleClick((CEGUI::MouseButton)i);
											dc = true;
										}
									}
								}
							}
							if(!dc)
							{
								if(i < 3)
									m_pSystem->injectMouseButtonDown((CEGUI::MouseButton)i);
							}
							dwLastClickTime[i] = GetTickCount();
							byteButtonClicked[i] = 1;
						}
					}
					else
					{
						if(byteButtonClicked[i])
						{
							if(i < 3)
								m_pSystem->injectMouseButtonUp((CEGUI::MouseButton)i);
						}
						byteButtonClicked[i] = 0;
					}
				}
			}
		}
	}
}

void CGUI::DrawText(CEGUI::String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, CEGUI::Font * pFont, bool bProcessFormatting, bool bAllowColorFormatting, CEGUI::Rect * rClipRect, float fSpaceExtra, float fXScale, float fYScale)
{
	if(m_bInitialized)
	{
		CEGUI::Font * pTextFont = pFont;

		if(!pTextFont)
			pTextFont = m_pDefaultFont;

		// TODO: Caching
		if(pTextFont)
		{
			m_pTextDrawingGeometryBuffer->reset();
			// Custom replacement for CEGUI::Font::drawText
			const float fBaseY = (vecPosition.d_y + pTextFont->getBaseline(fYScale));
			CEGUI::Vector2 vecGlyphPos(vecPosition);
			float fOtherY = 0;
			CEGUI::ColourRect rColourRect = rColorRect;

			// Loop through all characters
			for(size_t c = 0; c < sText.length(); ++c)
			{
				const CEGUI::FontGlyph * glyph;

				// Check for font formatting
				if(bProcessFormatting)
				{
					// Check for newline constant
					if(sText[c] == '\n')
					{
						vecGlyphPos.d_x = vecPosition.d_x;
						fOtherY += pTextFont->getFontHeight();
						continue;
					}

					// Check for color formatting
					else if(sText.length() >= 10 && (sText[c] == '[' && sText[c + 9] == ']'))
					{
						bool bValid = true;
						CEGUI::String sColour;

						// Loop through all color chars
						for(size_t i = 0; i < 8; i++)
						{
							unsigned char cChar = sText[(c + i) + 1];

							// Make sure its 0-99, A-F or a-f
							if((cChar < '0' || cChar > '9') && (cChar < 'A' || cChar > 'F') && 
								(cChar < 'a' || cChar > 'f'))
							{
								// char is invalid
								bValid = false;
								break;
							}

							// Add the char to the color string
							sColour += cChar;
						}

						// Set the color if its valid
						if(bValid)
						{
							// Is color formatting allowed?
							if(bAllowColorFormatting)
							{
								rColourRect.setColours((CEGUI::colour(strtoul(sColour.c_str(), NULL, 16)) >> 8) | 0xFF000000);
							}

							c += 9;
							continue;
						}
					}
				}

				// Get the glyph data
				if((glyph = pTextFont->getGlyphData(sText[c]))) // NB: assignment
				{
					// Get the glyph image
					const CEGUI::Image * const img = glyph->getImage();

					// Calculate the glyph y position
					vecGlyphPos.d_y = (fBaseY - (img->getOffsetY() - img->getOffsetY() * fYScale) + fOtherY);

					// Draw the glyph
					img->draw(*m_pTextDrawingGeometryBuffer, vecGlyphPos, glyph->getSize(fXScale, fYScale), rClipRect, rColourRect);

					// Increment the x position
					vecGlyphPos.d_x += glyph->getAdvance(fXScale);

					// Apply extra spacing to space chars
					if(sText[c] == ' ')
						vecGlyphPos.d_x += fSpaceExtra;
				}
			}
			m_pTextDrawingGeometryBuffer->draw();
		}
	}
}

void CGUI::DrawText(CEGUI::String sText, CEGUI::Vector2 vecPosition, CEGUI::ColourRect rColorRect, CEGUI::String sFontName, bool bProcessFormatting, bool bAllowColorFormatting, CEGUI::Rect * rClipRect, float fSpaceExtra, float fXScale, float fYScale)
{
	DrawText(sText, vecPosition, rColorRect, &CEGUI::FontManager::getSingleton().get(sFontName), bProcessFormatting, bAllowColorFormatting, rClipRect, fSpaceExtra, fXScale, fYScale);
}

bool CGUI::OnMessageBoxOkClick(const CEGUI::EventArgs &eventArgs)
{
	HideMessageBox();

	if(m_messageBox.pEventSubscriber)
		return (*m_messageBox.pEventSubscriber)(eventArgs);
	
	return true;
}

bool m_bMessageBoxHideCursor = false;
bool m_bCursorChangedForMessageBox = false;

void CGUI::ShowMessageBox(const CEGUI::String &sText, const CEGUI::String &sTitle, CEGUI::Event::Subscriber * pEventSubscriber)
{
	if(m_bInitialized)
	{
		HideMessageBox();

		m_messageBox.pWindow = CreateGUIFrameWindow();
		m_messageBox.pWindow->setText(sTitle);
		m_messageBox.pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.2f, 0)));
		m_messageBox.pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.4f, 0)));
		m_messageBox.pWindow->setAlwaysOnTop(true);
		m_messageBox.pWindow->setVisible(true);
		m_messageBox.pText = CreateGUIStaticText(m_messageBox.pWindow);
		m_messageBox.pText->setText(sText);
		m_messageBox.pText->setSize(CEGUI::UVector2(CEGUI::UDim(0.8f, 0), CEGUI::UDim(0.6f, 0)));
		m_messageBox.pText->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.01f, 0)));
		m_messageBox.pText->setProperty("FrameEnabled", "false");
		m_messageBox.pText->setProperty("BackgroundEnabled", "false");
		m_messageBox.pText->setVisible(true);
		m_messageBox.pButtons[0] = CreateGUIButton(m_messageBox.pWindow);
		m_messageBox.pButtons[0]->setText("Ok");
		m_messageBox.pButtons[0]->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.15f, 0)));
		m_messageBox.pButtons[0]->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3f, 0), CEGUI::UDim(0.75f, 0)));
		m_messageBox.pButtons[0]->setVisible(true);
		m_messageBox.pButtons[0]->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUI::OnMessageBoxOkClick, this));
		m_messageBox.pEventSubscriber = pEventSubscriber;

		if(!m_pCursor->isVisible())
		{
			m_bMessageBoxHideCursor = true;
			m_bCursorChangedForMessageBox = false;
			m_pCursor->setVisible(true);
		}
	}
}

void CGUI::HideMessageBox()
{
	if(m_bInitialized)
	{
		if(m_messageBox.pButtons[0])
			RemoveGUIWindow(m_messageBox.pButtons[0]);

		if(m_messageBox.pText)
			RemoveGUIWindow(m_messageBox.pText);

		if(m_messageBox.pWindow)
			RemoveGUIWindow(m_messageBox.pWindow);

		if(!m_bCursorChangedForMessageBox)
		{
			if(m_bMessageBoxHideCursor)
				m_pCursor->setVisible(false);
		}
	}
}

CEGUI::Window * CGUI::CreateGUIWindow(const CEGUI::String &sType, const CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	if(m_bInitialized)
	{
		CEGUI::Window * pWindow = m_pWindowManager->createWindow(sType, sName);
		if(pWindow)
		{
			CEGUI::Window * pParent = (pParentWindow == NULL) ? m_pDefaultWindow : pParentWindow;
			pParent->addChildWindow(pWindow);
		}
		return pWindow;
	}
	return NULL;
}

void CGUI::RemoveGUIWindow(CEGUI::String &sName)
{
	if(m_bInitialized)
	{
		m_pDefaultWindow->removeChildWindow(sName);
		m_pWindowManager->destroyWindow(sName);
	}
}

void CGUI::RemoveGUIWindow(CEGUI::Window * pWindow)
{
	if(m_bInitialized)
	{
		// Make sure the window exists
		if(m_pWindowManager->isWindowPresent(pWindow->getName()))
		{
			// Remove the window from the default window
			m_pDefaultWindow->removeChildWindow(pWindow);

			// Destroy the window
			m_pWindowManager->destroyWindow(pWindow);

			// NULL the window pointer
			pWindow = NULL;
		}
	}
}

void CGUI::RemoveGUIWindow(CGUITitleBar * pTitleBar)
{
	RemoveGUIWindow((CEGUI::Window *)pTitleBar);
}

void CGUI::RemoveGUIWindow(CGUIButton * pButton)
{
	RemoveGUIWindow((CEGUI::Window *)pButton);
}

void CGUI::RemoveGUIWindow(CGUIFrameWindow * pFrameWindow)
{
	RemoveGUIWindow((CEGUI::Window *)pFrameWindow);
}

void CGUI::RemoveGUIWindow(CGUIEditBox * pEditBox)
{
	RemoveGUIWindow((CEGUI::Window *)pEditBox);
}

void CGUI::RemoveGUIWindow(CGUIStaticImage * pStaticText)
{
	RemoveGUIWindow((CEGUI::Window *)pStaticText);
}

void CGUI::RemoveGUIWindow(CGUIStaticText * pStaticText)
{
	RemoveGUIWindow((CEGUI::Window *)pStaticText);
}

void CGUI::RemoveGUIWindow(CGUICheckBox * pCheckBox)
{
	RemoveGUIWindow((CEGUI::Window *)pCheckBox);
}

CGUITitleBar * CGUI::CreateGUITitleBar(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUITitleBar *)CreateGUIWindow(STYLE_PREFIX "/Titlebar", sName, pParentWindow);
}

CGUITitleBar * CGUI::CreateGUITitleBar(CEGUI::Window * pParentWindow)
{
	return (CGUITitleBar *)CreateGUIWindow(STYLE_PREFIX "/Titlebar", GetUniqueName(), pParentWindow);
}

CGUIButton * CGUI::CreateGUIButton(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIButton *)CreateGUIWindow(STYLE_PREFIX "/Button", sName, pParentWindow);
}

CGUIButton * CGUI::CreateGUIButton(CEGUI::Window * pParentWindow)
{
	return (CGUIButton *)CreateGUIWindow(STYLE_PREFIX "/Button", GetUniqueName(), pParentWindow);
}

CGUIFrameWindow * CGUI::CreateGUIFrameWindow(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIFrameWindow *)CreateGUIWindow(STYLE_PREFIX "/FrameWindow", sName, pParentWindow);
}

CGUIFrameWindow * CGUI::CreateGUIFrameWindow(CEGUI::Window * pParentWindow)
{
	return (CGUIFrameWindow *)CreateGUIWindow(STYLE_PREFIX "/FrameWindow", GetUniqueName(), pParentWindow);
}

CGUIEditBox * CGUI::CreateGUIEditBox(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIEditBox *)CreateGUIWindow(STYLE_PREFIX "/Editbox", sName, pParentWindow);
}

CGUIEditBox * CGUI::CreateGUIEditBox(CEGUI::Window * pParentWindow)
{
	return (CGUIEditBox *)CreateGUIWindow(STYLE_PREFIX "/Editbox", GetUniqueName(), pParentWindow);
}

CGUIStaticImage * CGUI::CreateGUIStaticImage(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIStaticImage *)CreateGUIWindow(STYLE_PREFIX "/StaticImage", sName, pParentWindow);
}

CGUIStaticImage * CGUI::CreateGUIStaticImage(CEGUI::Window * pParentWindow)
{
	return (CGUIStaticImage *)CreateGUIWindow(STYLE_PREFIX "/StaticImage", GetUniqueName(), pParentWindow);
}

CGUIStaticText * CGUI::CreateGUIStaticText(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIStaticText *)CreateGUIWindow(STYLE_PREFIX "/StaticText", sName, pParentWindow);
}

CGUIStaticText * CGUI::CreateGUIStaticText(CEGUI::Window * pParentWindow)
{
	return (CGUIStaticText *)CreateGUIWindow(STYLE_PREFIX "/StaticText", GetUniqueName(), pParentWindow);
}

CGUICheckBox * CGUI::CreateGUICheckBox(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUICheckBox *)CreateGUIWindow(STYLE_PREFIX "/Checkbox", sName, pParentWindow);
}

CGUICheckBox * CGUI::CreateGUICheckBox(CEGUI::Window * pParentWindow)
{
	return (CGUICheckBox *)CreateGUIWindow(STYLE_PREFIX "/Checkbox", GetUniqueName(), pParentWindow);
}

CGUIMultiColumnList * CGUI::CreateGUIMultiColumnList(CEGUI::String &sName, CEGUI::Window * pParentWindow)
{
	return (CGUIMultiColumnList *)CreateGUIWindow(STYLE_PREFIX "/MultiColumnList", sName, pParentWindow);
}

CGUIMultiColumnList * CGUI::CreateGUIMultiColumnList(CEGUI::Window * pParentWindow)
{
	return (CGUIMultiColumnList *)CreateGUIWindow(STYLE_PREFIX "/MultiColumnList", GetUniqueName(), pParentWindow);
}

void CGUI::SetCursorVisible(bool bVisible)
{
	if(m_bInitialized)
	{
		m_pCursor->setVisible(bVisible);
		m_bCursorChangedForMessageBox = true;
	}
}

bool CGUI::IsCursorVisible()
{
	if(m_bInitialized)
		return m_pCursor->isVisible();

	return false;
}

void CGUI::OnLostDevice()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Notify our gui renderer that the device has been lost
		if(m_pRenderer)
			m_pRenderer->preD3DReset();
	}
}

void CGUI::OnResetDevice()
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		// Notify our gui renderer that the device has been reset
		if(m_pRenderer)
			m_pRenderer->postD3DReset();
	}
}

DWORD ScanCodeToDIK(WORD wScanCode)
{
	switch(wScanCode)
	{
	case 0x41: return DIK_A; // Control + A (Select All)
	case 0x43: return DIK_C; // Control + C (Copy)
	case 0x58: return DIK_X; // Control + X (Cut)
	case 0x56: return DIK_V; // Control + V (Paste)
	case VK_TAB: return DIK_TAB; // Tab
	case VK_HOME: return DIK_HOME; // Home
	case VK_END: return DIK_END; // End
	case VK_INSERT: return DIK_INSERT; // Insert
	case VK_DELETE: return DIK_DELETE; // Delete
	case VK_UP: return DIK_UP; // Arrow Up
	case VK_DOWN: return DIK_DOWN; // Arrow Down
	case VK_LEFT: return DIK_LEFT; // Arrow Left
	case VK_RIGHT: return DIK_RIGHT; // Arrow Right
	case VK_SHIFT: return DIK_LSHIFT; // Shift
	case VK_LSHIFT: return DIK_LSHIFT; // Left Shift
	case VK_RSHIFT: return DIK_RSHIFT; // Right Shift
	case VK_BACK: return DIK_BACK; // Backspace
	case VK_RETURN: return DIK_RETURN; // Enter
	case VK_CONTROL: return DIK_LCONTROL; // Control
	case VK_LCONTROL: return DIK_LCONTROL; // Left Control
	case VK_RCONTROL: return DIK_RCONTROL; // Right Control
	}

	return 0;
}

// TODO: We should not continue on execution of the WndProc code if this returns true
void CGUI::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Make sure we are initialized
	if(m_bInitialized)
	{
		switch(uMsg)
		{
			// Mouse messages
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
			{
				// We handle the mouse manually with direct input
				return;
			}
		}

		// If its a char pass it to the gui
		if(uMsg == WM_CHAR)
			m_pSystem->injectChar(wParam);
		// Check if its a key
		else if(uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
		{
			// Convert it to a cegui scan code
			DWORD dwKey = ScanCodeToDIK(wParam);
			if(dwKey)
			{
				// Pass it to the gui
				if(uMsg == WM_KEYDOWN)
					m_pSystem->injectKeyDown(dwKey);
				else
					m_pSystem->injectKeyUp(dwKey);
			}
		}
	}
}

void SetClipboardDataFromString(const char * szString, size_t sStringSize)
{
	// Open the clipboard
	if(OpenClipboard(NULL))
	{
		// Empty the clipboard
		EmptyClipboard();

		// Allocate the memory globally to store our cut text
		HGLOBAL hMemory = GlobalAlloc(GMEM_DDESHARE, sStringSize);

		// Lock the globally allocated memory then get a pointer to it
		char * szMemory = (char *)GlobalLock(hMemory);

		// Copy the cut text to the globally allocated memory
		strcpy(szMemory, szString);

		// Unlock the globally allocated memory
		GlobalUnlock(hMemory);

		// Set the clipboard data to the allocated memory
		SetClipboardData(CF_TEXT, hMemory);

		// Close the clipboard
		CloseClipboard();
	}
}

bool CGUI::OnGUIKeyDown(const CEGUI::EventArgs &eventArgs)
{
	// Cast the argument to a key event args structure
	const CEGUI::KeyEventArgs &keyEventArgs = (const CEGUI::KeyEventArgs &)eventArgs;

	// Check if the left or right control key is held down
	if(keyEventArgs.sysKeys & CEGUI::Key::LeftControl || keyEventArgs.sysKeys & CEGUI::Key::RightControl)
	{
		// Check if its the a key that was pressed
		if(keyEventArgs.scancode == CEGUI::Key::A)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
			{
				// Get the edit box pointer
				CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;

				// Set the selection start to 0 and end to the edit box text length
				pEditBox->setSelection(0, pEditBox->getText().size());
			}
		}
		// Check if its the c or x key that was pressed
		else if(keyEventArgs.scancode == CEGUI::Key::C || keyEventArgs.scancode == CEGUI::Key::X)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
			{
				// Get the edit box pointer
				CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;

				// Get the edit box selection length
				size_t sSelectionLength = pEditBox->getSelectionLength();

				// Make sure we have a selection
				if(sSelectionLength > 0)
				{
					// Get the edit box selection start index
					size_t sSelectionStartIndex = pEditBox->getSelectionStartIndex();

					// Get the edit box selection end index
					size_t sSelectionEndIndex = (sSelectionStartIndex + sSelectionLength);

					// Get the edit box text
					CEGUI::String sEditBoxText = pEditBox->getText();

					// Get the text we wish to copy from the edit box text
					CEGUI::String sSelectionText = sEditBoxText.substr(sSelectionStartIndex, sSelectionEndIndex);

					// Copy the selection text to the clip board
					SetClipboardDataFromString(sSelectionText.c_str(), (sSelectionText.length() + 1));

					// If its the control + x key cut the selection from the edit box text
					if(keyEventArgs.scancode == CEGUI::Key::X)
					{
						// Cut the text from the edit box text
						sEditBoxText.replace(sSelectionStartIndex, sSelectionEndIndex, "");

						// Set the edit box text to the new cut text
						pEditBox->setText(sEditBoxText);
					}
				}
			}
		}
		// Check if its the v key that was pressed
		if(keyEventArgs.scancode == CEGUI::Key::V)
		{
			// Make sure its an edit box that is selected
			if(keyEventArgs.window->getType() == STYLE_PREFIX "/Editbox")
			{
				// Get the edit box pointer
				CEGUI::Editbox * pEditBox = (CEGUI::Editbox *)keyEventArgs.window;

				// Get the edit box selection length
				size_t sSelectionLength = pEditBox->getSelectionLength();

				// Get the edit box selection start index
				size_t sSelectionStartIndex = pEditBox->getSelectionStartIndex();

				// Get the edit box text
				CEGUI::String sEditBoxText = pEditBox->getText();

				// Open the clipboard
				if(OpenClipboard(NULL))
				{
					// Get a pointer to the clipboard text
					char * szClipboardText = (char *)GetClipboardData(CF_TEXT);

					// Make sure we have clipboard text
					if(szClipboardText)
					{
						// Get the caret index
						size_t sCaretIndex = pEditBox->getCaratIndex();

						// Add the clipboard text length to the caret index
						sCaretIndex += strlen(szClipboardText);

						// If we don't have a selection just insert the text
						if(sSelectionLength == 0)
							sEditBoxText.insert(sSelectionStartIndex, szClipboardText);
						// If we do have a selection overwrite the selected text
						else
							sEditBoxText.replace(sSelectionStartIndex, sSelectionLength, szClipboardText);

						// Set the edit box text to the string with the pasted text
						pEditBox->setText(sEditBoxText);

						// Set the edit box caret index to the new index after the pasted text
						pEditBox->setCaratIndex(sCaretIndex);
					}
				}
			}
		}
	}

	return false;
}

bool CGUI::IsInitialized()
{
	return m_bInitialized;
}

D3DDISPLAYMODE CGUI::GetDisplayMode()
{
	return m_DisplayMode;
}

CEGUI::Direct3D8Renderer * CGUI::GetRenderer()
{
	if(m_bInitialized)
		return m_pRenderer;

	return NULL;
}

CEGUI::System * CGUI::GetSystem()
{
	if(m_bInitialized)
		return m_pSystem;

	return NULL;
}

CEGUI::WindowManager * CGUI::GetWindowManager()
{
	if(m_bInitialized)
		return m_pWindowManager;

	return NULL;
}

CEGUI::DefaultWindow * CGUI::GetDefaultWindow()
{
	if(m_bInitialized)
		return m_pDefaultWindow;

	return NULL;
}

CEGUI::Font * CGUI::GetDefaultFont()
{
	if(m_bInitialized)
		return m_pDefaultFont;

	return NULL;
}

CEGUI::Font * CGUI::GetTahomaBoldFont()
{
	if(m_bInitialized)
		return m_pTahomaBoldFont;

	return NULL;
}

CEGUI::Font * CGUI::GetTahomaBold10Font()
{
	if(m_bInitialized)
		return m_pTahomaBoldFont10;

	return NULL;
}