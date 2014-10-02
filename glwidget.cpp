#include <QtGui>
#include <QtOpenGL>
#include </home/andy/qt-quick3d/qt-quick3d/src/threed/scene/qglabstractscene.h>
#include <qgraphicsrotation3d.h>
#include <iostream>



#include <QtGui>
#include <QtOpenGL>
#include <qglbuilder.h>
#include <qgeometrydata.h>

#include <cmath>

#include "glwidget.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

const char* GLWidget::KTargetModelPath = ":/model/souz.blend";
const char* GLWidget::KChaserModelPath = ":/model/souz.blend";
const char* GLWidget::KEnvironmentModelPath = ":/model/MountainLand.blend";//":/model/Montagne.blend";

//_targetOrientationFix(60, QVector3D(0.0, 1, 0)) - 30 - x, 0 - y

//, _targetOrientationFix(90, QVector3D(0.0, 1, 0)) - along x axis
// _targetOrientationFix(90, QVector3D(0, 1, 0)) - target going along x axis
// _targetOrientationFix(90, QVector3D(1, 0, 0)) - target going along z axis

GLWidget::GLWidget(QVector3D chaserStartPos, QVector2D chaserStartOrientation, QVector3D targetStartPos, QVector2D targetStartOrientation, QWidget *parent)
    : QGLView(QGLFormat(QGL::SampleBuffers), parent)
    , _targetPositionFix(0, 0, -3.7)
    , _targetOrientationFix(90, QVector3D(0.0, 1, 0))
    , _chaserPositionFix(0, 0, 0)
    , _chaserOrientationFix(90, QVector3D(0, 0, 1))
    , _environmentPositionFix(0, -100, -100)
    , _environmentOrientationFix(90, QVector3D(0, 0, 1))

    , _cameraDistance(20)
    , _cameraOrientation(3.14, 0.3)

    , _isPressed(false)

    , _targetPhyPosition(targetStartPos)
    , _targetPhyOrientation(targetStartOrientation)
    , _chaserPhyPosition(chaserStartPos)
    , _chaserPhyOrientation(chaserStartOrientation)

    , _prevTargetPosition(0, 0, 0)
    , _prevTargetOrientation(0, 0)
    , _prevChaserPosition(0, 0, 0)
    , _prevChaserOrientation(0, 0)
{

}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::initializeGL()
{
    std::cout << __FUNCTION__ << std::endl;
    qglClearColor(QColor(0, 0, 0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);

    getMesh(_targetScene, KTargetModelPath);

    getMesh(_environmentScene, KEnvironmentModelPath);
/*
    applyShift(*_targetScene->mainNode(), _targetPositionFix, _targetOrientationFix);

    applyShift(*_environmentScene->mainNode(), _environmentPositionFix, _environmentOrientationFix);


    QGLBuilder builder;
    QGeometryData centerId;

    centerId.appendVertex(QVector3D(0, 0, 0));
    centerId.appendVertex(QVector3D(0, 1, 100));
    centerId.appendVertex(QVector3D(0, 100, 1));
    centerId.appendVertex(QVector3D(100, 1, 1));

    builder << QGL::Faceted << centerId;
    _additionalScene.reset(builder.finalizedSceneNode());
    */
}


/*

orientation in opengl:
        y
        |
        |
        |
        |
        |
        |_______________z
       /
      /
     /
    /
   x
*/
void GLWidget::paintGL(QGLPainter *painter)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);

    //applyShift(*_targetScene->mainNode(), _targetPhyPosition + _targetPositionFix, targetDiffOrientation());

    camera()->setEye(getCamera(_targetPhyPosition, targetDiffOrientation()));
    //camera()->setCenter(QVector3D(getCenter(_targetPhyPosition));
    QGLSceneNode* mainNode = new QGLSceneNode;
    mainNode->addNode(_targetScene->mainNode());
    //mainNode->addNode(_environmentScene->mainNode());

    mainNode->draw(painter);

}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    _isPressed = true;
    _lastMousePosition.setX(event->pos().x());
    _lastMousePosition.setY(event->pos().y());

    //QGLView::mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    _isPressed = false;
    QGLView::mouseReleaseEvent(event);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(_isPressed == true)
    {
        float dx = _lastMousePosition.x() - event->pos().x();
        float dy = _lastMousePosition.y() - event->pos().y();

        const float turnKoef = 0.03f;
        float dAngleX = dx * turnKoef;
        float dAngleY = dy * turnKoef;

        _cameraOrientation.setX(_cameraOrientation.x() + dAngleX);
        _cameraOrientation.setY(_cameraOrientation.y() + dAngleY);

        _lastMousePosition.setX(event->pos().x());
        _lastMousePosition.setY(event->pos().y());
        repaint();
    }

    //QGLView::mouseMoveEvent(event);
}

void GLWidget::applyShift(QGLSceneNode &node, const QVector3D &phyPosition, const QQuaternion &phyOrientation)
{
    QGraphicsRotation3D* rotate1 = new QGraphicsRotation3D;
    rotate1->setAngle(phyOrientation.scalar());
    rotate1->setAxis(phyOrientation.vector());
    node.addTransform(rotate1);

    node.setPosition(phyPosition);
}

void GLWidget::applyShift(QGLSceneNode& node, const QVector3D& phyPosition, const QVector2D& phyOrientation)
{
    float dAngleX = phyOrientation.x();
    QGraphicsRotation3D* rotateAroundZ = new QGraphicsRotation3D;
    rotateAroundZ->setAngle(dAngleX);
    rotateAroundZ->setAxis(QVector3D(0, 0, 1));

    float dAngleY = phyOrientation.y();
    QGraphicsRotation3D* rotateAroundY = new QGraphicsRotation3D;
    rotateAroundZ->setAngle(dAngleY);
    rotateAroundZ->setAxis(QVector3D(0, 1, 0));

    node.addTransform(rotateAroundZ);
    node.addTransform(rotateAroundY);

    node.setPosition(phyPosition);
}

void GLWidget::getMesh(std::auto_ptr<QGLAbstractScene>& scene, const char* path)
{

    QLatin1String qlatin(path);
    QGLAbstractScene* model = QGLAbstractScene::loadScene(qlatin);
    if (model == NULL)
    {
        std::cout << "loadScene for " << path << " return null\n";
    }
    else
    {
        scene.reset(model);
    }

}

QVector3D GLWidget::getCamera(QVector3D dTargetPos, QVector2D dTargetOrientation)
{

    float z = _cameraDistance * std::sin(_cameraOrientation.y());
    float xy = _cameraDistance * std::cos(_cameraOrientation.y());

    float y = xy * std::sin(_cameraOrientation.x());
    float x = xy * std::cos(_cameraOrientation.x());

    QVector3D cameraPosition(x, z, y);

    std::cout << "Angles: x = " << _cameraOrientation.x() << ", y = " << _cameraOrientation.y() << std::endl;
    std::cout << "pos: x = " << cameraPosition.x() << ", y = " << cameraPosition.y() << ", z = " << cameraPosition.z()
              << ", distance: " << _cameraDistance << std::endl;

    return cameraPosition;//res + dTargetPos;
}

QVector3D GLWidget::getCenter(QVector3D dTargetPos)
{
    return dTargetPos;
}
