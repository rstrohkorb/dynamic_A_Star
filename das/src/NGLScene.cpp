#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/SimpleVAO.h>
#include <ngl/VAOFactory.h>
#include <iostream>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Dynamic A-Star Demo");
  // Graph setup
  std::vector<ngl::Vec3> points;
  points.reserve(16);
  for(size_t i = 0; i < 10; ++i)
  {
      for(size_t j = 0; j < 10; ++j)
      {
          points.push_back(ngl::Vec3(1.0f * i, 1.0f * j, 0.0f));
      }
  }
  m_graph = Graph(points);
  m_path = m_graph.aStar(0, 99);
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
  float s = 0.01f;
  m_project = ngl::ortho(_w * -s, _w * s, _h * -s, _h * s, 0.5f, 100.0f);
  //m_project = ngl::perspective(90.0f, static_cast<float>(_w/_h), 0.5f, 200.0f);
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

  //make a simple vao for the particle/sphere object
  m_sphereVAO = ngl::VAOFactory::createVAO(ngl::simpleVAO, GL_TRIANGLE_STRIP);
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  // create line list from graph
  auto lines = m_graph.render();
  // render out the lines
  m_lineVAO->bind();
  m_lineVAO->setData(ngl::SimpleVAO::VertexData(lines.size()*sizeof(ngl::Vec3),
                                          lines[0].m_x));
  m_lineVAO->setVertexAttributePointer(0, 3, GL_FLOAT, sizeof(ngl::Vec3), 0);
  m_lineVAO->setNumIndices(lines.size());
  loadMatrixToShader(ngl::Mat4(1.0f), ngl::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
  m_lineVAO->draw();
  m_lineVAO->unbind();

}

void NGLScene::loadMatrixToShader(const ngl::Mat4 &_tx, const ngl::Vec4 &_color)
{
    ngl::ShaderLib *shader = ngl::ShaderLib::instance();
    shader->use("lineShader");
    shader->setUniform("MVP", m_project * m_view * _tx);
    shader->setUniform("vertColor", _color);
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
