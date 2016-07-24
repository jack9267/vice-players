/***********************************************************************
    filename:   CEGUIDirect3D8Renderer.cpp
    created:    Thu Jul 29 2010
    author:     Mark Rohrbacher
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#define NOMINMAX
#include "CEGUIDirect3D8Renderer.h"
#include "CEGUIDirect3D8Texture.h"
#include "CEGUIDirect3D8GeometryBuffer.h"
#include "CEGUIDirect3D8RenderTarget.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIExceptions.h"
#include "CEGUIDirect3D8ViewportTarget.h"
#include "CEGUIDirect3D8TextureTarget.h"
#include "CEGUISystem.h"
#include "CEGUIDefaultResourceProvider.h"
#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String Direct3D8Renderer::d_rendererID(
"CEGUI::Direct3D8Renderer - Official Direct3D 8 based 2nd generation renderer "
"module.");

//----------------------------------------------------------------------------//
static const D3DMATRIX s_identityMatrix =
{
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

//----------------------------------------------------------------------------//
Direct3D8Renderer& Direct3D8Renderer::bootstrapSystem(LPDIRECT3DDEVICE8 device)
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "Direct3D8Renderer::bootstrapSystem: CEGUI::System object is "
            "already initialised."));

    Direct3D8Renderer& renderer(create(device));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException("Direct3D8Renderer::destroySystem: "
            "CEGUI::System object is not created or was already destroyed."));

    Direct3D8Renderer* renderer =
        static_cast<Direct3D8Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
Direct3D8Renderer& Direct3D8Renderer::create(LPDIRECT3DDEVICE8 device)
{
    return *new Direct3D8Renderer(device);
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroy(Direct3D8Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
RenderingRoot& Direct3D8Renderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& Direct3D8Renderer::createGeometryBuffer()
{
    Direct3D8GeometryBuffer* b = new Direct3D8GeometryBuffer(*this, d_device);
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                     d_geometryBuffers.end(),
                                     &buffer);

    if (d_geometryBuffers.end() != i)
    {
        d_geometryBuffers.erase(i);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* Direct3D8Renderer::createTextureTarget()
{
    TextureTarget* t = new Direct3D8TextureTarget(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                    d_textureTargets.end(),
                                    target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        delete target;
    }
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& Direct3D8Renderer::createTexture()
{
    Direct3D8Texture* tex = new Direct3D8Texture(*this);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D8Renderer::createTexture(const String& filename,
        const String& resourceGroup)
{
    Direct3D8Texture* tex = new Direct3D8Texture(*this, filename,
            resourceGroup);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D8Renderer::createTexture(const Size& size)
{
    Direct3D8Texture* tex = new Direct3D8Texture(*this, size);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<Direct3D8Texture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::beginRendering()
{
    d_device->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    // no pixel shaders initially
    d_device->SetPixelShader(0);

    // set device states
    d_device->SetRenderState(D3DRS_LIGHTING, FALSE);
    d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    d_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    // setup texture addressing settings
    d_device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
    d_device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

    // setup colour calculations
    d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

    // setup alpha calculations
    d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    // setup filtering
    d_device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    d_device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

    // disable texture stages we do not need.
    d_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

    // setup scene alpha blending
    d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    // put alpha blend operations into a known state
    setupRenderingBlendMode(BM_NORMAL, true);

    // set view matrix back to identity.
    d_device->SetTransform(D3DTS_VIEW, &s_identityMatrix);
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::setDisplaySize(const Size& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rect area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }
}

//----------------------------------------------------------------------------//
const Size& Direct3D8Renderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2& Direct3D8Renderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint Direct3D8Renderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& Direct3D8Renderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Direct3D8Renderer::Direct3D8Renderer(LPDIRECT3DDEVICE8 device) :
    d_device(device),
    d_displaySize(getViewportSize()),
    d_displayDPI(96, 96),
    d_defaultRoot(0),
    d_defaultTarget(0)
{
    D3DCAPS8 caps;
    device->GetDeviceCaps(&caps);

    d_maxTextureSize = ceguimin(caps.MaxTextureHeight, caps.MaxTextureWidth);

    d_supportNonSquareTex = !(caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY);

    d_supportNPOTTex = !(caps.TextureCaps & D3DPTEXTURECAPS_POW2) ||
                       (caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL);

    d_defaultTarget = new Direct3D8ViewportTarget(*this);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);
}

//----------------------------------------------------------------------------//
Direct3D8Renderer::~Direct3D8Renderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultRoot;
    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//
Size Direct3D8Renderer::getViewportSize()
{
    D3DVIEWPORT8 vp;

    if (FAILED(d_device->GetViewport(&vp)))
        CEGUI_THROW(RendererException(
            "Direct3D8Renderer::getViewportSize - Unable to access required "
            "view port information from Direct3DDevice8."));
    else
        return Size(static_cast<float>(vp.Width),
                    static_cast<float>(vp.Height));
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::preD3DReset()
{
    // perform pre-reset on texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<Direct3D8TextureTarget*>(*target_iterator)->preD3DReset();

    // perform pre-reset on textures
    TextureList::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        (*texture_iterator)->preD3DReset();
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::postD3DReset()
{
    // perform post-reset on textures
    TextureList::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        (*texture_iterator)->postD3DReset();

    // perform post-reset on texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<Direct3D8TextureTarget*>(*target_iterator)->postD3DReset();

    // notify system about the (possibly) new viewport size.
    System::getSingleton().notifyDisplaySizeChanged(getViewportSize());
}

//----------------------------------------------------------------------------//
LPDIRECT3DDEVICE8 Direct3D8Renderer::getDevice() const
{
    return d_device;
}

//----------------------------------------------------------------------------//
Texture& Direct3D8Renderer::createTexture(LPDIRECT3DTEXTURE8 texture)
{
    Direct3D8Texture* tex = new Direct3D8Texture(*this, texture);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
bool Direct3D8Renderer::supportsNonSquareTexture()
{
    return d_supportNonSquareTex;
}

//----------------------------------------------------------------------------//
bool Direct3D8Renderer::supportsNPOTTextures()
{
    return d_supportNPOTTex;
}

//----------------------------------------------------------------------------//
Size Direct3D8Renderer::getAdjustedSize(const Size& sz)
{
    Size s(sz);

    if (!d_supportNPOTTex)
    {
        s.d_width  = getSizeNextPOT(sz.d_width);
        s.d_height = getSizeNextPOT(sz.d_height);
    }
    if (!d_supportNonSquareTex)
        s.d_width = s.d_height =
                        ceguimax(s.d_width, s.d_height);

    return s;
}

//----------------------------------------------------------------------------//
float Direct3D8Renderer::getSizeNextPOT(float sz) const
{
    uint size = static_cast<uint>(sz);

    // if not power of 2
    if ((size & (size - 1)) || !size)
    {
        int log = 0;

        // get integer log of 'size' to base 2
        while (size >>= 1)
            ++log;

        // use log to calculate value to use as size.
        size = (2 << log);
    }

    return static_cast<float>(size);
}

//----------------------------------------------------------------------------//
void Direct3D8Renderer::setupRenderingBlendMode(const BlendMode mode,
                                                const bool force)
{
    // exit if no change (and setup not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;


    if (d_activeBlendMode == BM_RTT_PREMULTIPLIED)
    {
        d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
    else
    {
        d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
