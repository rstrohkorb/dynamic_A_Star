#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/Random.h>
#include <ngl/ShaderLib.h>
#include <ngl/SimpleVAO.h>
#include <ngl/Transformation.h>
#include <ngl/VAOFactory.h>
#include <iostream>

NGLScene::NGLScene(QWidget *_parent )
{
    // set this widget to have the initial keyboard focus
    setFocus();
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());
    // initialize member variables
    setGraphType(0);
    m_goal = m_graph.size() - 1;
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  //field of view, aspect ratio, near clipping plane, far clipping plane
  //float s = 0.01f;
  //m_project = ngl::ortho(_w * -s, _w * s, _h * -s, _h * s, 0.5f, 100.0f);
  m_project = ngl::perspective(45.0f, static_cast<float>(_w/_h), 0.5f, 200.0f);
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.6f, 0.6f, 0.6f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  // load shaders
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->loadShader("lineShader", "shaders/lineVertex.glsl",
                   "shaders/lineFragment.glsl");
  //set the camera
  m_view = ngl::lookAt({0.0f, 20.0f, 20.0f}, ngl::Vec3::zero(), ngl::Vec3::up());

  //make a simple vao for the lines
  m_lineVAO = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_LINES);

  //make a primitive sphere
  ngl::VAOPrimitives::instance()->createSphere("sphere", 0.3f, 40);
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  // handle mouse rotation
  ngl::Mat4 rotx;
  ngl::Mat4 roty;
  ngl::Mat4 mouseRotation;
  rotx.rotateX(m_win.spinXFace);
  roty.rotateY(m_win.spinYFace);
  mouseRotation = roty * rotx;
  // create line list from graph
  auto lines = m_graph.render();
  // render out the lines
  m_lineVAO->bind();
  m_lineVAO->setData(ngl::SimpleVAO::VertexData(lines.size()*sizeof(ngl::Vec3),
                                          lines[0].m_x));
  m_lineVAO->setVertexAttributePointer(0, 3, GL_FLOAT, sizeof(ngl::Vec3), 0);
  m_lineVAO->setNumIndices(lines.size());
  loadMatrixToShader(mouseRotation, ngl::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  m_lineVAO->draw();
  m_lineVAO->unbind();
  // render out the particles
  if(m_visParticles)
  {
      ngl::Transformation tx;
      auto *prim = ngl::VAOPrimitives::instance();
      for(auto p : m_particles)
      {
          tx.setPosition(p.pos);
          loadMatrixToShader(mouseRotation * tx.getMatrix(), ngl::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
          prim->draw("sphere");
      }
  }
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
    m_timerId = _event->timerId();
    animateParticles();
    update();
}

void NGLScene::spawn()
{
    // check list completeness
    if(m_particles.size() < m_numParticles)
    {
        // add particles
        auto toAdd = m_numParticles - m_particles.size();
        for(size_t i = 0; i < toAdd; ++i)
        {
            createParticle(m_goal);
        }
    }
}

void NGLScene::createParticle(size_t _goal)
{
    // handle random start/goal
    ngl::Random *rng = ngl::Random::instance();
    size_t start = static_cast<size_t>(rng->randomPositiveNumber(m_graph.size()-1));
    // create a particle and load it up
    Particle p(m_graph.pos(start), m_graph.pos(_goal), 0.05f);
    p.path = m_graph.aStar(start, _goal);
    auto direction = p.path[0] - p.pos;
    direction.normalize();
    p.dir = direction;
    m_particles.push_back(p);
}

void NGLScene::animateParticles()
{
    for(auto& p : m_particles)
    {
        // set direction
        auto direction = p.path[0] - p.pos;
        direction.normalize();
        // check for path stage completion
        if(direction != p.dir)
        {
            p.pos = p.path[0];
            p.path.erase(p.path.begin());
            if(p.path.size() == 0)
            {
                continue;
            }
            direction = p.path[0] - p.pos;
            direction.normalize();
            p.dir = direction;
        }
        // set new pos
        p.pos += direction * p.speed;
    }
    prune(); // cut off particles that have reached their goals
    spawn(); // spawn in new particles up to m_numParticles
}

void NGLScene::prune()
{
    for(auto it = m_particles.begin(); it != m_particles.end();)
    {
        if( it->path.size() == 0 )
        {
            it = m_particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void NGLScene::resetParticles()
{
    m_particles.clear();
    spawn();
}

void NGLScene::loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_color)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("lineShader");
    shader->setUniform("MVP", m_project * m_view * _tx);
    shader->setUniform("vertColor", _color);
}

void NGLScene::makeGraph_2Dgrid(ngl::Vec2 _bl, ngl::Vec2 _tr, size_t _h, size_t _w)
{
    std::vector<ngl::Vec3> points;
    points.reserve(_h * _w);
    // amount between points in x/y directions
    auto diffx = abs(_tr.m_x - _bl.m_x)/_w;
    auto diffy = abs(_tr.m_y - _bl.m_y)/_h;
    // allocate points
    for(float y = _bl.m_y; y < _tr.m_y; y += diffy)
    {
        for(float x = _bl.m_x; x < _tr.m_x; x += diffx)
        {
            points.push_back(ngl::Vec3(x, y, 0.0f));
        }
    }
    m_graph = Graph(points, 2);
    resetParticles();
}

void NGLScene::makeGraph_3Dgrid(ngl::Vec3 _bl, ngl::Vec3 _tr, size_t _h, size_t _w, size_t _d)
{
    std::vector<ngl::Vec3> points;
    points.reserve(_h * _w * _d);
    // amount between points in x/y directions
    auto diffx = abs(_tr.m_x - _bl.m_x)/_w;
    auto diffy = abs(_tr.m_y - _bl.m_y)/_h;
    auto diffz = abs(_tr.m_z - _bl.m_z)/_d;
    // allocate points
    for(float y = _bl.m_y; y < _tr.m_y; y += diffy)
    {
        for(float x = _bl.m_x; x < _tr.m_x; x += diffx)
        {
            for(float z = _bl.m_z; z < _tr.m_z; z += diffz)
            {
                points.push_back(ngl::Vec3(x, y, z));
            }
        }
    }
    m_graph = Graph(points, 3);
    resetParticles();
}

void NGLScene::makeGraph_2Drand(ngl::Vec2 _bl, ngl::Vec2 _tr, size_t _n, size_t _degree)
{
    ngl::Random *rng = ngl::Random::instance();
    std::vector<ngl::Vec3> points;
    points.reserve(_n);
    // determine point range
    auto xrange = abs(_tr.m_x - _bl.m_x)/2.0f;
    auto yrange = abs(_tr.m_y - _bl.m_y)/2.0f;
    // allocate points randomly
    for(size_t i = 0; i < _n; ++i)
    {
        auto p = rng->getRandomPoint(xrange, yrange, 0.0f) + ngl::Vec3(_bl.m_x, _bl.m_y, 0.0f);
        points.push_back(p);
    }
    m_graph = Graph(points, _degree);
    resetParticles();
}

void NGLScene::makeGraph_3Drand(ngl::Vec3 _bl, ngl::Vec3 _tr, size_t _n, size_t _degree)
{
    ngl::Random *rng = ngl::Random::instance();
    std::vector<ngl::Vec3> points;
    points.reserve(_n);
    // determine point range
    auto xrange = abs(_tr.m_x - _bl.m_x)/2.0f;
    auto yrange = abs(_tr.m_y - _bl.m_y)/2.0f;
    auto zrange = abs(_tr.m_z - _bl.m_z)/2.0f;
    // allocate points randomly
    for(size_t i = 0; i < _n; ++i)
    {
        auto p = rng->getRandomPoint(xrange, yrange, zrange) + _bl;
        points.push_back(p);
    }
    m_graph = Graph(points, _degree);
    resetParticles();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::startSim()
{
    m_visParticles = true;
    startTimer(10);
}

void NGLScene::stopSim()
{
    killTimer(m_timerId);
}

void NGLScene::setGraphType(int _i)
{
    // timer shouldn't be going while we switch graph types, turn off particles
    killTimer(m_timerId);
    m_visParticles = false;
    // switch graph types
    switch(_i)
    {
    case 0:
        makeGraph_2Dgrid(ngl::Vec2(-5.0f, -5.0f), ngl::Vec2(5.0f, 5.0f), 10, 10); break;
    case 1:
        makeGraph_2Drand(ngl::Vec2(-5.0f, -5.0f), ngl::Vec2(5.0f, 5.0f), 100, 3); break;
    case 2:
        makeGraph_3Dgrid(ngl::Vec3(-5.0f), ngl::Vec3(5.0f), 10, 10, 10); break;
    case 3:
        makeGraph_3Drand(ngl::Vec3(-5.0f), ngl::Vec3(5.0f), 300, 3); break;
    default: break;
    }
    // update gl window and redraw
    update();
}

void NGLScene::resetGraph()
{
    // probably going to delete this
}
