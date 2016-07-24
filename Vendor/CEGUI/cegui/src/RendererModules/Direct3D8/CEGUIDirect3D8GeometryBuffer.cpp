/***********************************************************************
    filename:   CEGUIDirect3D8GeometryBuffer.cpp
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
#include "CEGUIDirect3D8GeometryBuffer.h"
#include "CEGUIDirect3D8Texture.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIVertex.h"
#include <windows.h>
#include <d3d8.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D8GeometryBuffer::Direct3D8GeometryBuffer(Direct3D8Renderer& owner,
                                                 LPDIRECT3DDEVICE8 device) :
    d_owner(owner),
    d_activeTexture(0),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_device(device),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::draw() const
{
    //TODO: Set up clipping for this buffer

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    d_device->SetTransform(D3DTS_WORLD, &d_matrix);

    d_owner.setupRenderingBlendMode(d_blendMode);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        // draw the batches
        size_t pos = 0;
        BatchList::const_iterator i = d_batches.begin();
        for ( ; i != d_batches.end(); ++i)
        {
            d_device->SetTexture(0, (*i).first);
            d_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, (*i).second / 3,
                                    &d_vertices[pos], sizeof(D3DVertex));
            pos += (*i).second;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setTranslation(const Vector3& t)
{
    d_translation = t;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setRotation(const Vector3& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setPivot(const Vector3& p)
{
    d_pivot = p;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setClippingRegion(const Rect& region)
{
    d_clipRect.d_top    = PixelAligned(region.d_top);
    d_clipRect.d_bottom = PixelAligned(region.d_bottom);
    d_clipRect.d_left   = PixelAligned(region.d_left);
    d_clipRect.d_right  = PixelAligned(region.d_right);
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                             uint vertex_count)
{
    performBatchManagement();

    // update size of current batch
    d_batches.back().second += vertex_count;

    // buffer these vertices
    D3DVertex vd;
    const Vertex* vs = vbuff;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by D3D as needed.
        vd.x       = vs->position.d_x - 0.5f;
        vd.y       = vs->position.d_y - 0.5f;
        vd.z       = vs->position.d_z;
        vd.diffuse = vs->colour_val.getARGB();
        vd.tu      = vs->tex_coords.d_x;
        vd.tv      = vs->tex_coords.d_y;
        d_vertices.push_back(vd);
    }
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<Direct3D8Texture*>(texture);
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::reset()
{
    d_batches.clear();
    d_vertices.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* Direct3D8GeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint Direct3D8GeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint Direct3D8GeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* Direct3D8GeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::performBatchManagement()
{
    const LPDIRECT3DTEXTURE8 t = d_activeTexture ?
                                 d_activeTexture->getDirect3D8Texture() : 0;

    // create a new batch if there are no batches yet, or if the active texture
    // differs from that used by the current batch.
    if (d_batches.empty() || (t != d_batches.back().first))
        d_batches.push_back(BatchInfo(t, 0));
}

//----------------------------------------------------------------------------//
void Direct3D8GeometryBuffer::updateMatrix() const
{
    const D3DXVECTOR3 p(d_pivot.d_x, d_pivot.d_y, d_pivot.d_z);
    const D3DXVECTOR3 t(d_translation.d_x, d_translation.d_y, d_translation.d_z);

    D3DXQUATERNION r;
    D3DXQuaternionRotationYawPitchRoll(&r,
        D3DXToRadian(d_rotation.d_y),
        D3DXToRadian(d_rotation.d_x),
        D3DXToRadian(d_rotation.d_z));

    D3DXMatrixTransformation(&d_matrix, 0, 0, 0, &p, &r, &t);
    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
const D3DXMATRIX* Direct3D8GeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return &d_matrix;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
