/***********************************************************************
    filename:   CEGUIDirect3D8Texture.h
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
#ifndef _CEGUIDirect3D8Texture_h_
#define _CEGUIDirect3D8Texture_h_

#include "../../CEGUIBase.h"
#include "../../CEGUIRenderer.h"
#include "../../CEGUITexture.h"
#include "CEGUIDirect3D8Renderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//! Texture implementation for the Direct3D8Renderer.
class DIRECT3D8_GUIRENDERER_API Direct3D8Texture : public Texture
{
public:
    /*!
    \brief
        set the D3D8 texture that this Texture is based on to the specified
        texture.
    */
    void setDirect3D8Texture(LPDIRECT3DTEXTURE8 tex);

    /*!
    \brief
        Return the internal D3D8 texture used by this Texture object.

    \return
        Pointer to the D3D8 texture interface that this object is using.
    */
    LPDIRECT3DTEXTURE8 getDirect3D8Texture() const;

    /*!
    \brief
        Sets what the texture should consider as the original data size.

    \note
        This also causes the texel scaling values to be updated.
    */
    void setOriginalDataSize(const Size& sz);

    //! auto called via the Renderer prior to Reset on the Direct3DDevice8.
    void preD3DReset();

    //! auto called via the Renderer after Reset on the Direct3DDevice8.
    void postD3DReset();

    // implement abstract members from base class.
    const Size& getSize() const;
    const Size& getOriginalDataSize() const;
    const Vector2& getTexelScaling() const;
    void loadFromFile(const String& filename, const String& resourceGroup);
    void loadFromMemory(const void* buffer, const Size& buffer_size,
                        PixelFormat pixel_format);
    void saveToMemory(void* buffer);

protected:
    // Friends (to allow construction and destruction)
    friend Texture& Direct3D8Renderer::createTexture(void);
    friend Texture& Direct3D8Renderer::createTexture(const String&, const String&);
    friend Texture& Direct3D8Renderer::createTexture(const Size&);
    friend Texture& Direct3D8Renderer::createTexture(LPDIRECT3DTEXTURE8 tex);
    friend void Direct3D8Renderer::destroyTexture(Texture&);

    //! Basic constructor.
    Direct3D8Texture(Direct3D8Renderer& owner);
    //! Construct texture from an image file.
    Direct3D8Texture(Direct3D8Renderer& owner, const String& filename,
                     const String& resourceGroup);
    //! Construct texture with a given size.
    Direct3D8Texture(Direct3D8Renderer& owner, const Size& sz);
    //! Construct texture that wraps an existing D3D8 texture.
    Direct3D8Texture(Direct3D8Renderer& owner, LPDIRECT3DTEXTURE8 tex);
    //! Destructor.
    virtual ~Direct3D8Texture();

    //! clean up the internal texture.
    void cleanupDirect3D8Texture();
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();
    //! set d_size to actual texture size (d_dataSize is used if query fails)
    void updateTextureSize();

    //! Direct3D8Renderer object that created and owns this texture.
    Direct3D8Renderer& d_owner;
    //! The D3D8 texture we're wrapping.
    LPDIRECT3DTEXTURE8 d_texture;
    //! Size of the texture.
    Size d_size;
    //! original pixel of size data loaded into texture
    Size d_dataSize;
    //! cached pixel to texel mapping scale values.
    Vector2 d_texelScaling;
    //! holds info about the texture surface before we released it for reset.
    D3DSURFACE_DESC d_savedSurfaceDesc;
    //! true when d_savedSurfaceDesc is valid and texture can be restored.
    bool d_savedSurfaceDescValid;
};

} // End of  CEGUI namespace section


#endif // end of guard _CEGUIDirect3D8Texture_h_
