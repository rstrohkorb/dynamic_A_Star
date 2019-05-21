#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include <ngl/AbstractVAO.h>
#include "WindowParams.h"
#include "Graph.h"
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------

    /// @brief windows parameters for mouse control etc.
    WinParams m_win;
    /// position for our model
    ngl::Vec3 m_modelPos;
    /// view matrix
    ngl::Mat4 m_view;
    /// projection matrix
    ngl::Mat4 m_project;

    // add a QT timer event
    void timerEvent(QTimerEvent *_event) override;

    /// VAO
    std::unique_ptr<ngl::AbstractVAO> m_lineVAO;
    /// Graph
    Graph m_graph;

    /// load matrix to shader
    void loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_color);

    /// private struct for the particle animation data
    struct Particle
    {
        ngl::Vec3 pos;
        ngl::Vec3 goal;
        std::vector<ngl::Vec3> path;
        ngl::Vec3 dir;
        float speed;

        // constructors
        Particle(ngl::Vec3 _pos, ngl::Vec3 _goal, std::vector<ngl::Vec3> _path, ngl::Vec3 _dir, float _speed) :
            pos(_pos), goal(_goal), path(_path), dir(_dir), speed(_speed) {;}
        Particle(ngl::Vec3 _pos, ngl::Vec3 _goal, float _speed) : pos(_pos), goal(_goal), speed(_speed) {;}
        Particle()=default;
    };
    /// store the particles
    std::vector<Particle> m_particles;
    /// animate the particles
    void animateParticles();
    /// prune finished particles
    void prune();

};



#endif
