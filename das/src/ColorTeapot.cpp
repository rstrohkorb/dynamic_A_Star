#include <ngl/NGLInit.h>
#include <ngl/Util.h>

#include "ColorTeapot.h"
#include "teapot.h"

ColorTeapot::ColorTeapot()
{
    // the ngl teapot is an array of size 128304
    // data is arranged in: tx,ty,nx,ny,nz,vx,vy,vz
    // there are 5346 triangles
    // i'm stealing this so that I can color the teapot by vertex
    size_t NUM_TRIS = 5346;
    size_t NUM_VERTS = NUM_TRIS * 3;

    m_vertices.reserve(NUM_VERTS);
    m_normals.reserve(NUM_VERTS);

    for(size_t i = 0; i < NUM_VERTS; ++i)
    {
        auto ind = i * 8;
        // ind+0 is tx
        // ind+1 is ty
        m_normals.push_back(ngl::Vec3(static_cast<float>(teapot[ind+2]),
                                      static_cast<float>(teapot[ind+3]),
                                      static_cast<float>(teapot[ind+4])));
        m_vertices.push_back(ngl::Vec3(static_cast<float>(teapot[ind+5]),
                                       static_cast<float>(teapot[ind+6]),
                                       static_cast<float>(teapot[ind+7])));
    }
}

std::vector<ngl::Vec3> ColorTeapot::render(std::vector<ngl::Vec3> _colors)
{
    // _colors can be of any size, just going to use % operator and repeat the colors
    // return vector org: [vertex] [color]
    std::vector<ngl::Vec3> renderlist;
    renderlist.reserve(m_vertices.size() * 2);
    for(size_t i = 0; i < m_vertices.size(); ++i)
    {
        renderlist.push_back(m_vertices[i]);
        renderlist.push_back(_colors[i % _colors.size()]);
    }

    return renderlist;
}
