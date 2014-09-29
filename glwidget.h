#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <qt4/Qt3D/qglview.h>
#include <memory>

#include <iostream>

class QGLAbstractScene;
class QGLSceneNode;
class QGLPainter;

class GLWidget : public  QGLView
{
    Q_OBJECT

public:
    GLWidget(QVector3D chaserStartPos, QVector2D chaserStartOrientation,
             QVector3D targetStartPos, QVector2D targetStartOrientation,
             QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void tick()
    {
        QVector3D targetPos = this->_targetPhyPosition + QVector3D(10, 0, 0);
        this->setModelState(_chaserPhyPosition, _chaserPhyOrientation, targetPos, _targetPhyOrientation);
        repaint();
    }

    QVector3D chaserPosition() const
    {
        return _chaserPhyPosition;
    }

    QVector2D chaserOrientation() const
    {
        return _chaserPhyOrientation;
    }

    QVector3D targetPosition() const
    {
        return _targetPhyPosition;
    }

    QVector2D targetOrientation() const
    {
        std::cout << _targetPhyOrientation.x() << std::endl;
        return _targetPhyOrientation;
    }

    void addDistance()
    {
        _cameraDistance *= 1.2;
        repaint();
    }

    void subDistance()
    {
        _cameraDistance *= 0.84;
        repaint();
    }

    void setModelState(QVector3D chaserPosition, QVector2D chaserOrientation, QVector3D targetPosition, QVector2D targetOrientation)
    {

        _prevChaserPosition = _chaserPhyPosition;
        _prevChaserOrientation = _chaserPhyOrientation;

        _prevTargetPosition = _targetPhyPosition;
        _prevTargetOrientation = _targetPhyOrientation;


        _chaserPhyPosition = chaserPosition;
        _chaserPhyOrientation = chaserOrientation;

        _targetPhyPosition = targetPosition;
        _targetPhyOrientation = targetOrientation;

        repaint();
    }

protected:
    void initializeGL();
    void paintGL(QGLPainter *painter);
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

private:
    void applyShift(QGLSceneNode& scene, const QVector3D& phyPosition, const QQuaternion& phyOrientation);
    void applyShift(QGLSceneNode& scene, const QVector3D& phyPosition, const QVector2D& phyOrientation);
    void getMesh(std::auto_ptr<QGLAbstractScene>& scene, const char* path);
    QVector3D getCamera(QVector3D dTargetPos, QVector2D dTargetOrientation);
    QVector3D getCenter(QVector3D dTargetPos);
private:
    float _cameraDistance;
    QVector2D _cameraOrientation;
    bool _isPressed;
    QVector2D _lastMousePosition;

    QVector3D _targetPositionFix, _chaserPositionFix, _environmentPositionFix;
    QQuaternion _targetOrientationFix, _chaserOrientationFix, _environmentOrientationFix;

    QVector3D _targetPhyPosition, _chaserPhyPosition;
    QVector2D _targetPhyOrientation, _chaserPhyOrientation;

    QVector3D _prevTargetPosition, _prevChaserPosition;
    QVector2D _prevTargetOrientation, _prevChaserOrientation;

    QVector3D chaserDiffPosition()
    {
        return _chaserPhyPosition - _prevChaserPosition;
    }

    QVector3D targetDiffPosition()
    {
        return _targetPhyPosition - _prevTargetPosition;
    }

    QVector2D chaserDiffOrientation()
    {
        return _chaserPhyOrientation - _prevChaserOrientation;
    }

    QVector2D targetDiffOrientation()
    {
        return _targetPhyOrientation - _prevTargetOrientation;
    }

    static const char* KTargetModelPath;
    static const char* KChaserModelPath;
    static const char* KEnvironmentModelPath;

    std::auto_ptr<QGLAbstractScene> _targetScene;
    std::auto_ptr<QGLAbstractScene> _chaserScene;
    std::auto_ptr<QGLAbstractScene> _environmentScene;
    std::auto_ptr<QGLSceneNode> _additionalScene;

};


#endif // GLWIDGET_H
