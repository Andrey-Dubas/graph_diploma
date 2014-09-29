#include "window.h"
#include "ui_window.h"

#include "glwidget.h"

#include <QtGui>

Window::Window()
{
    glWidget = new GLWidget(QVector3D(0, 0, 0), QVector2D(0, 90), QVector3D(10, 10, 10), QVector2D(0, 0), this);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    setLayout(mainLayout);

    glWidget->resize(200, 200);

    setWindowTitle(tr("diploma GL"));
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_1)
    {
        this->glWidget->addDistance();
    }
    if(event->key() == Qt::Key_2)
    {
        this->glWidget->subDistance();
    }

    QVector2D targetOri = glWidget->targetOrientation();
    switch(event->key())
    {
    case Qt::Key_W:
        glWidget->tick();
        break;
    case Qt::Key_A:
        targetOri.setX(0.02);
        glWidget->setModelState(
                    glWidget->chaserPosition(),
                    glWidget->chaserOrientation(),

                    glWidget->targetPosition(),
                    targetOri
                    );
        break;

    case Qt::Key_D:
        targetOri.setX(0.02);
        glWidget->setModelState(
                    glWidget->chaserPosition(),
                    glWidget->chaserOrientation(),

                    glWidget->targetPosition(),
                    targetOri
                    );
        break;
    }

    QWidget::keyPressEvent(event);
}

