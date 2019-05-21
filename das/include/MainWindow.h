#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// code for all MainWindow class from QtNGL, by Jon Macey

#include <QMainWindow>
#include "NGLScene.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *m_ui;
        /// @brief our openGL widget
        NGLScene *m_gl;
};

#endif // MAINWINDOW_H
