#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include <QMap>
#include <QSet>
#include <set>
#include "../geolabel.h"
#include "openglwidget.h"
#include "tridimensionalobject.h"
namespace Ui {
class OpenGLWindow;
}

class OpenGLWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenGLWindow(vector<GeometryEngine *> *obj, QWidget *parent = 0);
    ~OpenGLWindow();

public slots:
    void changeCheckboxState(bool newState);
    void modifyZoom(int value);
    void modifySpeed(int value);
private:
    Ui::OpenGLWindow *ui;
    QMap<QString, int> checkBoxMap;
    vector<GeometryEngine *> *objectsList;
};

#endif // OPENGLWINDOW_H
