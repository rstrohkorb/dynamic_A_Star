#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);

  m_gl=new  NGLScene(this);

  // gl widget
  m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);
  // connect up slots
  // SIM OPTIONS
  connect(m_ui->m_startSim, SIGNAL(clicked()), m_gl, SLOT(startSim()));
  connect(m_ui->m_stopSim, SIGNAL(clicked()), m_gl, SLOT(stopSim()));
  // GRAPH OPTIONS
  connect(m_ui->m_graphSelection, SIGNAL(currentIndexChanged(int)), m_gl, SLOT(setGraphType(int)));
  connect(m_ui->m_resetGraph, SIGNAL(clicked()), m_gl, SLOT(resetGraph()));

//  connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
//  // set the rotation signals
//  connect(m_ui->m_rotationX,SIGNAL(valueChanged(double)),m_gl,SLOT(setXRotation(double)));
//  connect(m_ui->m_rotationY,SIGNAL(valueChanged(double)),m_gl,SLOT(setYRotation(double)));
//  connect(m_ui->m_rotationZ,SIGNAL(valueChanged(double)),m_gl,SLOT(setZRotation(double)));
//  /// set the scale signals
//  connect(m_ui->m_scaleX,SIGNAL(valueChanged(double)),m_gl,SLOT(setXScale(double)));
//  connect(m_ui->m_scaleY,SIGNAL(valueChanged(double)),m_gl,SLOT(setYScale(double)));
//  connect(m_ui->m_scaleZ,SIGNAL(valueChanged(double)),m_gl,SLOT(setZScale(double)));
//  /// set the position signals
//  connect(m_ui->m_positionX,SIGNAL(valueChanged(double)),m_gl,SLOT(setXPosition(double)));
//  connect(m_ui->m_positionY,SIGNAL(valueChanged(double)),m_gl,SLOT(setYPosition(double)));
//  connect(m_ui->m_positionZ,SIGNAL(valueChanged(double)),m_gl,SLOT(setZPosition(double)));
//  /// set the combo box index change signal
//  connect(m_ui->m_objectSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObjectMode(int)));
//  connect(m_ui->m_colour,SIGNAL(clicked()),m_gl,SLOT(setColour()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}
