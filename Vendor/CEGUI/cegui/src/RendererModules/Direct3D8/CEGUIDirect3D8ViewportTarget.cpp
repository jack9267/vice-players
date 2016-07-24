/***********************************************************************
    filename:   CEGUIDirect3D8ViewportTarget.cpp
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
#include "CEGUIDirect3D8ViewportTarget.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIExceptions.h"
#include <d3d8.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D8ViewportTarget::Direct3D8ViewportTarget(Direct3D8Renderer& owner) :
    Direct3D8RenderTarget(owner)
{
    // initialise renderer size
    D3DVIEWPORT8 vp;
    if (FAILED(d_owner.getDevice()->GetViewport(&vp)))
        CEGUI_THROW(RendererException("Direct3D8RenderTarget: Unable to access "
            "required view port information from Direct3DDevice8."));

    Rect area(
        Point(static_cast<float>(vp.X), static_cast<float>(vp.Y)),
        Size(static_cast<float>(vp.Width), static_cast<float>(vp.Height))
    );

    setArea(area);
}

//----------------------------------------------------------------------------//
Direct3D8ViewportTarget::Direct3D8ViewportTarget(Direct3D8Renderer& owner,
    const Rect& area) :
        Direct3D8RenderTarget(owner)
{
    setArea(area);
}

//----------------------------------------------------------------------------//
bool Direct3D8ViewportTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
