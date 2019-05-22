#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <ngl/Mat4.h>
#include <ngl/AbstractVAO.h>
#include "WindowParams.h"
#include "Graph.h"
#include "ColorTeapot.h"
#include <QEvent>
#include <QResizeEvent>
#include <QOpenGLWidget>
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

class NGLScene : public QOpenGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for GLWindow
    /// @param [in] _parent the parent window to create the GL context in
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(QWidget *_parent );
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

public slots :
    /// Functions for handling slots
    /// Sim options
    void startSim();
    void stopSim();
    /// Particle options
    void setNumParticles(int _i);
    void setRandomGoal(bool _isRandom);
    void changeGoal();
    /// other options
    void setGraphType(int _i);
    void setTeapotVisible(bool _isVisible);
    void setTeapotEffectToggle(bool _isOn);

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;

    /// @brief windows parameters for mouse control etc.
    WinParams m_win;
    /// position for our model
    ngl::Vec3 m_modelPos;
    /// view matrix
    ngl::Mat4 m_view;
    /// projection matrix
    ngl::Mat4 m_project;
    /// timerID, for starting/stopping timer
    int m_timerId;

    // add a QT timer event
    void timerEvent(QTimerEvent *_event) override;

    /// VAOs
    std::unique_ptr<ngl::AbstractVAO> m_lineVAO;
    std::unique_ptr<ngl::AbstractVAO> m_teapotVAO;
    /// Graph
    Graph m_graph;
    /// teapot
    ColorTeapot m_teapot;
    bool m_teapotVisible = false;
    bool m_teapotEffectOn = false;

    /// load matrix to shaders
    void loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_color);
    void loadMatrixToTeapotShader(const ngl::Mat4 &_tx);

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
    bool m_visParticles = false;
    size_t m_numParticles = 10;
    size_t m_goal = 0;
    bool m_isGoalRandom = false;
    /// particle handlers
    void spawn();
    void createParticle(size_t _goal);
    void animateParticles();
    void prune();
    void resetParticles();
    void resetGoal(size_t _goal);

    /// graph construction methods
    void makeGraph_2Dgrid(ngl::Vec2 _bl, ngl::Vec2 _tr, size_t _h, size_t _w);
    void makeGraph_3Dgrid(ngl::Vec3 _bl, ngl::Vec3 _tr, size_t _h, size_t _w, size_t _d);
    void makeGraph_2Drand(ngl::Vec2 _bl, ngl::Vec2 _tr, size_t _n, size_t _degree);
    void makeGraph_3Drand(ngl::Vec3 _bl, ngl::Vec3 _tr, size_t _n, size_t _degree);

};



#endif
