#ifndef VECTORCANVAS_H
#define VECTORCANVAS_H
#include<QWidget>
#include<QMouseEvent>
#include<QPainter>
#include<QPoint>
#include<QSize>
#include<QWheelEvent>
#include<QScrollArea>
#include<memory>
#include"spdlog/logger.h"
class VectorCanvas : public QWidget
{
    Q_OBJECT

public:
    VectorCanvas();
    ~VectorCanvas();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);
signals:
    void PenChange(QColor color,int width);
protected slots:
    void onClickColor();
    void onClickWidth();
private:
    QPen m_pen=Qt::red;
    bool leftPress=false;
    QPoint lastPos;
    QPoint curPos;
    QPixmap lastPic;
    std::shared_ptr<spdlog::logger> logger;

};

#endif // VECTORCANVAS_H
