#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>

namespace Ui {
class OpenGLWindow;
}

class OpenGLWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenGLWindow(QWidget *parent = 0);
    ~OpenGLWindow();

private:
    Ui::OpenGLWindow *ui;
};

#endif // OPENGLWINDOW_H
