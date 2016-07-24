/***********************************************************************
    filename:   CEGUIDirect3D8TextureTarget.h
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
#ifndef _CEGUIDirect3D8TextureTarget_h_
#define _CEGUIDirect3D8TextureTarget_h_

#include "CEGUIDirect3D8RenderTarget.h"
#include "../../CEGUITextureTarget.h"
#include "../../CEGUIRect.h"
#include <d3d8.h>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4250)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class Direct3D8Texture;

//! Direct3D8TextureTarget - allows rendering to an Direct3D8 texture via .
class DIRECT3D8_GUIRENDERER_API Direct3D8TextureTarget : public Direct3D8RenderTarget,
                                                         public TextureTarget
{
public:
    Direct3D8TextureTarget(Direct3D8Renderer& owner);
    virtual ~Direct3D8TextureTarget();

    //! auto called via the Renderer prior to Reset on the Direct3DDevice8.
    void preD3DReset();

    //! auto called via the Renderer after Reset on the Direct3DDevice8.
    void postD3DReset();

    // overrides from Direct3D8RenderTarget
    void activate();
    void deactivate();
    // implementation of RenderTarget interface
    bool isImageryCache() const;
    // implementation of TextureTarget interface
    void clear();
    Texture& getTexture() const;
    void declareRenderSize(const Size& sz);
    bool isRenderingInverted() const;

protected:
    //! default size of created texture objects
    static const float DEFAULT_SIZE;

    //! allocate and set up the texture used for rendering.
    void initialiseRenderTexture();
    //! clean up the texture used for rendering.
    void cleanupRenderTexture();
    //! resize the texture
    void resizeRenderTexture();

    //! switch to the texture surface & depth buffer
    void enableRenderTexture();
    //! switch back to previous surface
    void disableRenderTexture();

    //! Direct3D8 texture that's rendered to.
    LPDIRECT3DTEXTURE8 d_texture;
    //! Direct3D8 surface for the texture
    LPDIRECT3DSURFACE8 d_surface;
    //! we use this to wrap d_texture so it can be used by the core CEGUI lib.
    Direct3D8Texture* d_CEGUITexture;
    //! colour surface that was in use before this target was activated.
    LPDIRECT3DSURFACE8 d_prevColourSurface;
    LPDIRECT3DSURFACE8 d_prevDepthStencilSurface;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D8TextureTarget_h_
