// PUBLIC DOMAIN
/*
	file:
		IDirect3DDevice8Hook.cpp
	desc:
		This is the code for wrapping the IDirect3DDevice8 interface.
*/

// Includes
#include "StdInc.h"

// Externals
extern BOOL bWindowedMode;
extern IDirect3DDevice8 *pD3DDevice;
//extern D3DDISPLAYMODE D3DDisplayMode;
extern CGame		 *pGame;
extern CChatWindow   *pChatWindow;
extern CCmdWindow	 *pCmdWindow;
extern CBBFont		 *bbfont;
extern D3DMATRIX	 matView;

extern CGUI			 *pGUI;

extern DWORD BarOldStateBlock;
extern DWORD BarNewStateBlock;

void InitD3DStuff();
void TheSceneEnd();

// Class Functions
IDirect3DDevice8Hook::IDirect3DDevice8Hook(IDirect3D8 * pD3D, IDirect3DDevice8 * pDevice)
{
	m_pD3D = pD3D;
	m_pDevice = pDevice;
	pD3DDevice = pDevice;

	InitD3DStuff();
}

IDirect3DDevice8Hook::~IDirect3DDevice8Hook()
{

}

// Functions
HRESULT __stdcall IDirect3DDevice8Hook::QueryInterface(REFIID iid, void **ppvObject)
{
	return m_pDevice->QueryInterface(iid, ppvObject);
}

ULONG __stdcall IDirect3DDevice8Hook::AddRef()
{
	return m_pDevice->AddRef();
}

ULONG __stdcall IDirect3DDevice8Hook::Release()
{
	return m_pDevice->Release();
}

HRESULT __stdcall IDirect3DDevice8Hook::TestCooperativeLevel()
{
	return m_pDevice->TestCooperativeLevel();
}

UINT __stdcall IDirect3DDevice8Hook::GetAvailableTextureMem()
{
	return m_pDevice->GetAvailableTextureMem();
}

HRESULT __stdcall IDirect3DDevice8Hook::ResourceManagerDiscardBytes(DWORD Bytes)
{
	return m_pDevice->ResourceManagerDiscardBytes(Bytes);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetDirect3D(IDirect3D8** ppD3D)
{
	return m_pDevice->GetDirect3D(ppD3D);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetDeviceCaps(D3DCAPS8* pCaps)
{
	return m_pDevice->GetDeviceCaps(pCaps);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetDisplayMode(pMode);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	return m_pDevice->GetCreationParameters(pParameters);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap)
{
	return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void __stdcall IDirect3DDevice8Hook::SetCursorPosition(UINT X, UINT Y, DWORD Flags)
{
	m_pDevice->SetCursorPosition(X, Y, Flags);
}

BOOL __stdcall IDirect3DDevice8Hook::ShowCursor(BOOL bShow)
{
	return m_pDevice->ShowCursor(bShow);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** ppSwapChain)
{
	return m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT __stdcall IDirect3DDevice8Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if(bWindowedMode)
	{
		pPresentationParameters->Windowed = 1;
		pPresentationParameters->Flags = 0;
		pPresentationParameters->FullScreen_RefreshRateInHz = 0;
		pPresentationParameters->FullScreen_PresentationInterval = 0;

		//pPresentationParameters->BackBufferFormat = D3DDisplayMode.Format;

		SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, SWP_SHOWWINDOW);
	}

	if(pGUI)
		pGUI->OnLostDevice();

	if(bbfont)
	{
		delete bbfont;
		bbfont = NULL;
	}

	if(BarOldStateBlock)
	{
		m_pDevice->DeleteStateBlock(BarOldStateBlock);
		BarOldStateBlock = 0;
	}

	if(BarNewStateBlock)
	{
		m_pDevice->DeleteStateBlock(BarNewStateBlock);
		BarNewStateBlock = 0;
	}

	if(pChatWindow)
	{
		pChatWindow->DeleteDeviceObjects();
	}

	if(pCmdWindow)
	{
		pCmdWindow->DeleteDeviceObjects();
	}

	HRESULT hr = m_pDevice->Reset(pPresentationParameters);

	if(pGUI)
		pGUI->OnResetDevice();

	return hr;
}

HRESULT __stdcall IDirect3DDevice8Hook::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer)
{
	return m_pDevice->GetBackBuffer(BackBuffer, Type, ppBackBuffer);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDevice->GetRasterStatus(pRasterStatus);
}

void __stdcall IDirect3DDevice8Hook::SetGammaRamp(DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	m_pDevice->SetGammaRamp(Flags, pRamp);
}

void __stdcall IDirect3DDevice8Hook::GetGammaRamp(D3DGAMMARAMP* pRamp)
{
	m_pDevice->GetGammaRamp(pRamp);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture)
{
	return m_pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture)
{
	return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture)
{
	return m_pDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer)
{
	return m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer)
{
	return m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, Lockable, ppSurface);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateImageSurface(Width, Height, Format, ppSurface);
}

HRESULT __stdcall IDirect3DDevice8Hook::CopyRects(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray)
{
	return m_pDevice->CopyRects(pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray);
}

HRESULT __stdcall IDirect3DDevice8Hook::UpdateTexture(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture)
{
	return m_pDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetFrontBuffer(IDirect3DSurface8* pDestSurface)
{
	return m_pDevice->GetFrontBuffer(pDestSurface);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil)
{
	return m_pDevice->SetRenderTarget(pRenderTarget, pNewZStencil);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetRenderTarget(IDirect3DSurface8** ppRenderTarget)
{
	return m_pDevice->GetRenderTarget(ppRenderTarget);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface)
{
	return m_pDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT __stdcall IDirect3DDevice8Hook::BeginScene()
{
	return m_pDevice->BeginScene();
}

HRESULT __stdcall IDirect3DDevice8Hook::EndScene()
{	
	DWORD token = 0;
    CreateStateBlock ( D3DSBT_ALL, &token );

	if(pGUI) pGUI->Render();

    // Restore the render states
    if ( token != 0 )
    {
		ApplyStateBlock(token);
    }

	DeleteStateBlock(token);
	TheSceneEnd();
	return m_pDevice->EndScene();
}

HRESULT __stdcall IDirect3DDevice8Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	if (State == D3DTS_VIEW) matView = *pMatrix;

	return m_pDevice->SetTransform(State, pMatrix);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return m_pDevice->GetTransform(State, pMatrix);
}

HRESULT __stdcall IDirect3DDevice8Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->MultiplyTransform(State, pMatrix);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetViewport(CONST D3DVIEWPORT8* pViewport)
{
	return m_pDevice->SetViewport(pViewport);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetViewport(D3DVIEWPORT8* pViewport )
{
	return m_pDevice->GetViewport(pViewport);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetMaterial(CONST D3DMATERIAL8* pMaterial)
{
	return m_pDevice->SetMaterial(pMaterial);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetMaterial(D3DMATERIAL8* pMaterial)
{
	return m_pDevice->GetMaterial(pMaterial);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetLight(DWORD Index, CONST D3DLIGHT8* pLight)
{
	return m_pDevice->SetLight(Index, pLight);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetLight(DWORD Index, D3DLIGHT8* pLight)
{
	return m_pDevice->GetLight(Index, pLight);
}

HRESULT __stdcall IDirect3DDevice8Hook::LightEnable(DWORD LightIndex, BOOL bEnable)
{
	return m_pDevice->LightEnable(LightIndex, bEnable);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return m_pDevice->GetLightEnable(Index, pEnable);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetClipPlane(DWORD Index, CONST float* pPlane )
{
	return m_pDevice->SetClipPlane(Index, pPlane);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetClipPlane(DWORD Index, float* pPlane)
{
	return m_pDevice->GetClipPlane(Index, pPlane);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return m_pDevice->SetRenderState(State, Value);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pDevice->GetRenderState(State, pValue);
}

HRESULT __stdcall IDirect3DDevice8Hook::BeginStateBlock()
{
	return m_pDevice->BeginStateBlock();
}

HRESULT __stdcall IDirect3DDevice8Hook::EndStateBlock(DWORD* pToken)
{
	return m_pDevice->EndStateBlock(pToken);
}

HRESULT __stdcall IDirect3DDevice8Hook::ApplyStateBlock(DWORD Token)
{
	return m_pDevice->ApplyStateBlock(Token);
}

HRESULT __stdcall IDirect3DDevice8Hook::CaptureStateBlock(DWORD Token)
{
	return m_pDevice->CaptureStateBlock(Token);
}

HRESULT __stdcall IDirect3DDevice8Hook::DeleteStateBlock(DWORD Token)
{
	return m_pDevice->DeleteStateBlock(Token);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD* pToken)
{
	return m_pDevice->CreateStateBlock(Type, pToken);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus)
{
	return m_pDevice->SetClipStatus(pClipStatus);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetClipStatus(D3DCLIPSTATUS8* pClipStatus)
{
	return m_pDevice->GetClipStatus(pClipStatus);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture)
{
	return m_pDevice->GetTexture(Stage, ppTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture)
{
	return m_pDevice->SetTexture(Stage, pTexture);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return m_pDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	return m_pDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT __stdcall IDirect3DDevice8Hook::ValidateDevice(DWORD* pNumPasses)
{
	return m_pDevice->ValidateDevice(pNumPasses);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetInfo(DWORD DevInfoID, VOID* pDevInfoStruct, DWORD DevInfoStructSize)
{
	return m_pDevice->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetCurrentTexturePalette(UINT* pPaletteNumber)
{
	return m_pDevice->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{
	return m_pDevice->DrawIndexedPrimitive(Type, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT __stdcall IDirect3DDevice8Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags)
{
	return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreateVertexShader(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage)
{
	return m_pDevice->CreateVertexShader(pDeclaration, pFunction, pHandle, Usage);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetVertexShader(DWORD Handle)
{
	return m_pDevice->SetVertexShader(Handle);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetVertexShader(DWORD* pHandle)
{
	return m_pDevice->GetVertexShader(pHandle);
}

HRESULT __stdcall IDirect3DDevice8Hook::DeleteVertexShader(DWORD Handle)
{
	return m_pDevice->DeleteVertexShader(Handle);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetVertexShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->GetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetVertexShaderDeclaration(Handle, pData, pSizeOfData);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetVertexShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride)
{
	return m_pDevice->SetStreamSource(StreamNumber, pStreamData, Stride);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride)
{
	return m_pDevice->GetStreamSource(StreamNumber, ppStreamData, pStride);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex)
{
	return m_pDevice->SetIndices(pIndexData, BaseVertexIndex);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetIndices(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex)
{
	return m_pDevice->GetIndices(ppIndexData, pBaseVertexIndex);
}

HRESULT __stdcall IDirect3DDevice8Hook::CreatePixelShader(CONST DWORD* pFunction, DWORD* pHandle)
{
	return m_pDevice->CreatePixelShader(pFunction, pHandle);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetPixelShader(DWORD Handle)
{
	return m_pDevice->SetPixelShader(Handle);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetPixelShader(DWORD* pHandle)
{
	return m_pDevice->GetPixelShader(pHandle);
}

HRESULT __stdcall IDirect3DDevice8Hook::DeletePixelShader(DWORD Handle)
{
	return m_pDevice->DeletePixelShader(Handle);
}

HRESULT __stdcall IDirect3DDevice8Hook::SetPixelShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->SetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->GetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT __stdcall IDirect3DDevice8Hook::GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetPixelShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT __stdcall IDirect3DDevice8Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT __stdcall IDirect3DDevice8Hook::DeletePatch(UINT Handle)
{
	return m_pDevice->DeletePatch(Handle);
}