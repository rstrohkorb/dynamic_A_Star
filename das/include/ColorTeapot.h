#ifndef COLORTEAPOT_H_
#define COLORTEAPOT_H_

#include <vector>
#include <ngl/Vec3.h>

class ColorTeapot
{
public:
    ColorTeapot();
    size_t numTris() const { return m_vertices.size(); }
    std::vector<ngl::Vec3> render(std::vector<ngl::Vec3> _colors);
private:
    std::vector<ngl::Vec3> m_vertices;
    std::vector<ngl::Vec3> m_normals;
};

#endif
