/***********************************************************************
    filename:   CEGUIDirect3D8Texture.cpp
    created:    Thu Jul 29 2010
    author:     Mark Rohrbacher
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include <d3dx8.h>
#include "CEGUIDirect3D8Texture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D8Texture::Direct3D8Texture(Direct3D8Renderer& owner) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
}

//----------------------------------------------------------------------------//
Direct3D8Texture::Direct3D8Texture(Direct3D8Renderer& owner,
                                   const String& filename,
                                   const String& resourceGroup) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
Direct3D8Texture::Direct3D8Texture(Direct3D8Renderer& owner, const Size& sz) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(sz),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    Size tex_sz(d_owner.getAdjustedSize(sz));

    HRESULT hr = D3DXCreateTexture(d_owner.getDevice(),
                                   static_cast<UINT>(tex_sz.d_width),
                                   static_cast<UINT>(tex_sz.d_height),
                                   1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
                                   &d_texture);

    if (FAILED(hr))
        CEGUI_THROW(RendererException(
            "Direct3D8Texture - Failed to create texture of specified size: "
            "D3D Texture creation failed."));

    updateTextureSize();
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
Direct3D8Texture::Direct3D8Texture(Direct3D8Renderer& owner,
                                   LPDIRECT3DTEXTURE8 tex) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    setDirect3D8Texture(tex);
}

//----------------------------------------------------------------------------//
Direct3D8Texture::~Direct3D8Texture()
{
    cleanupDirect3D8Texture();
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::setDirect3D8Texture(LPDIRECT3DTEXTURE8 tex)
{
    if (d_texture != tex)
    {
        cleanupDirect3D8Texture();
        d_dataSize.d_width = d_dataSize.d_height = 0;

        d_texture = tex;
        if (d_texture)
            d_texture->AddRef();
    }

    updateTextureSize();
    d_dataSize = d_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
LPDIRECT3DTEXTURE8 Direct3D8Texture::getDirect3D8Texture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
const Size& Direct3D8Texture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Size& Direct3D8Texture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2& Direct3D8Texture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::loadFromFile(const String& filename,
                                    const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException("Direct3D8Texture::loadFromFile - "
            "CEGUI::System object has not been created!"));

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
            resourceGroup);

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        CEGUI_THROW(RendererException("Direct3D8Texture::loadFromFile - " +
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::loadFromMemory(const void* buffer,
                                      const Size& buffer_size,
                                      PixelFormat pixel_format)
{
    cleanupDirect3D8Texture();

    // create a texture
    // TODO: Check resulting pixel format and react appropriately.
    D3DFORMAT pixfmt;
    switch (pixel_format)
    {
    case PF_RGB:
        pixfmt = D3DFMT_R8G8B8;
        break;
    case PF_RGBA:
        pixfmt = D3DFMT_A8R8G8B8;
        break;
    default:
        CEGUI_THROW(RendererException("Direct3D8Texture::loadFromMemory failed: "
            "Invalid PixelFormat value specified."));
    }

    Size tex_sz(d_owner.getAdjustedSize(buffer_size));

    HRESULT hr = D3DXCreateTexture(d_owner.getDevice(),
                                   static_cast<UINT>(tex_sz.d_width),
                                   static_cast<UINT>(tex_sz.d_height),
                                   1, 0, pixfmt, D3DPOOL_MANAGED, &d_texture);
    if (FAILED(hr))
        hr = D3DXCreateTexture(d_owner.getDevice(),
                                   static_cast<UINT>(tex_sz.d_width),
                                   static_cast<UINT>(tex_sz.d_height),
                                   1, D3DUSAGE_DYNAMIC, pixfmt, D3DPOOL_DEFAULT, &d_texture);

    if (FAILED(hr))
        CEGUI_THROW(RendererException("Direct3D8Texture::loadFromMemory failed: "
            "Direct3D8 texture creation failed."));

    d_dataSize = buffer_size;
    updateTextureSize();
    updateCachedScaleValues();

    // lock the D3D texture
    D3DLOCKED_RECT rect;
    hr = d_texture->LockRect(0, &rect, 0, 0);

    if (FAILED(hr))
    {
        d_texture->Release();
        d_texture = 0;

        CEGUI_THROW(RendererException("Direct3D8Texture::loadFromMemory failed: "
            "IDirect3DTexture8::LockRect failed."));
    }

    // copy data from buffer into texture
    ulong* dst = static_cast<ulong*>(rect.pBits);
    const ulong* src = static_cast<const ulong*>(buffer);

    // RGBA
    if (pixel_format == PF_RGBA)
    {
        for (uint i = 0; i < buffer_size.d_height; ++i)
        {
            for (uint j = 0; j < buffer_size.d_width; ++j)
            {
                // we dont need endian safety on microsoft
                uchar r = static_cast<uchar>(src[j] & 0xFF);
                uchar g = static_cast<uchar>((src[j] >> 8) & 0xFF);
                uchar b = static_cast<uchar>((src[j] >> 16)  & 0xFF);
                uchar a = static_cast<uchar>((src[j] >> 24) & 0xFF);

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += rect.Pitch / sizeof(ulong);
            src += static_cast<ulong>(buffer_size.d_width);
        }
    }
    // RGB
    else
    {
        for (uint i = 0; i < buffer_size.d_height; ++i)
        {
            for (uint j = 0; j < buffer_size.d_width; ++j)
            {
                uchar r = reinterpret_cast<const uchar*>(src)[j * 3];
                uchar g = reinterpret_cast<const uchar*>(src)[j * 3 + 1];
                uchar b = reinterpret_cast<const uchar*>(src)[j * 3 + 2];
                uchar a = 0xFF;

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += rect.Pitch / sizeof(ulong);
            src = reinterpret_cast<const ulong*>
                  (reinterpret_cast<const uchar*>(src) +
                   static_cast<int>(buffer_size.d_width) * 3);
        }
    }

    d_texture->UnlockRect(0);
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::saveToMemory(void* buffer)
{
    // TODO:
    CEGUI_THROW(RendererException(
        "Direct3D8Texture::saveToMemory - Unimplemented!"));
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::cleanupDirect3D8Texture()
{
    if (d_texture)
    {
        d_texture->Release();
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::updateCachedScaleValues()
{
    //
    // calculate what to use for x scale
    //
    const float orgW = d_dataSize.d_width;
    const float texW = d_size.d_width;

    // if texture and original data width are the same, scale is based
    // on the original size.
    // if texture is wider (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_x = 1.0f / ((orgW == texW) ? orgW : texW);

    //
    // calculate what to use for y scale
    //
    const float orgH = d_dataSize.d_height;
    const float texH = d_size.d_height;

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_y = 1.0f / ((orgH == texH) ? orgH : texH);
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::updateTextureSize()
{
    D3DSURFACE_DESC surfDesc;

    // obtain details of the size of the texture
    if (d_texture && SUCCEEDED(d_texture->GetLevelDesc(0, &surfDesc)))
    {
        d_size.d_width  = static_cast<float>(surfDesc.Width);
        d_size.d_height = static_cast<float>(surfDesc.Height);
    }
    // use the original size if query failed.
    // NB: This should probably be an exception.
    else
        d_size = d_dataSize;
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::setOriginalDataSize(const Size& sz)
{
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::preD3DReset()
{
    // if already saved surface info, or we have no texture, do nothing
    if (d_savedSurfaceDescValid || !d_texture)
        return;

    // get info about our texture
    d_texture->GetLevelDesc(0, &d_savedSurfaceDesc);

    // if texture is managed, we have nothing more to do
    if (d_savedSurfaceDesc.Pool == D3DPOOL_MANAGED)
        return;

    // otherwise release texture.
    d_texture->Release();
    d_texture = 0;
    d_savedSurfaceDescValid = true;
}

//----------------------------------------------------------------------------//
void Direct3D8Texture::postD3DReset()
{
    // if texture has no saved surface info, we do nothing.
    if (!d_savedSurfaceDescValid)
        return;

    // otherwise, create a new texture using saved details.
    d_owner.getDevice()->
        CreateTexture(d_savedSurfaceDesc.Width,
                      d_savedSurfaceDesc.Height,
                      1, d_savedSurfaceDesc.Usage, d_savedSurfaceDesc.Format,
                      d_savedSurfaceDesc.Pool, &d_texture);

    d_savedSurfaceDescValid = false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
