#include"vectorcanvas.h"
#include<QDebug>
#include<QStylePainter>
#include"spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include<iostream>
VectorCanvas::VectorCanvas()
{
    connect(this,SIGNAL(PenChange(QColor,int)),this,SLOT(onPenChange(QColor,int)));
    lastPic=QPixmap(QSize(800,600));
}

VectorCanvas::~VectorCanvas()
{

}

void VectorCanvas::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton){
        leftPress=true;
        lastPos=event->pos();
        //auto logger=spdlog::basic_logger_st("basic_logger","./log/vectorcanvas.txt");
        if(logger==nullptr){
            try
                {
                    logger = spdlog::basic_logger_mt("basic_logger", "E:/Qt project/build-SpreadSheets-Desktop_Qt_5_7_1_MSVC2015_32bit-Debug/debug/basic-log.txt");
                }
                catch (const spdlog::spdlog_ex &ex)
                {
                    std::cout << "Log init failed: " << ex.what() << std::endl;
                }
        }
        logger->info("Press in position ({0},{1})",lastPos.x(),lastPos.y());
        spdlog::info("Press in position ({0},{1})",lastPos.x(),lastPos.y());
    }
}

void VectorCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if(leftPress){
        curPos=event->pos();
        if(curPos!=lastPos){
            update();
        }
    }
}

void VectorCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    leftPress=false;
}

void VectorCanvas::wheelEvent(QWheelEvent *event)
{
    qDebug()<<"heel";
}

void VectorCanvas::paintEvent(QPaintEvent *event)
{
    QPainter p1(&lastPic);
    p1.setPen(m_pen);
    p1.setRenderHint(QPainter::Antialiasing,true);
    p1.drawLine(lastPos,curPos);
    lastPos=curPos;
    QStylePainter p(this);
    p.setPen(m_pen);
//    p.setRenderHint(QPainter::Antialiasing,true);
    p.drawPixmap(0,0,lastPic);


}

void VectorCanvas::onClickColor()
{
    int r=rand()%256;
    int g=rand()%256;
    int b=rand()%256;
    QColor c(r,g,b);
    m_pen.setColor(c);
}

void VectorCanvas::onClickWidth()
{
    int w=rand()%19;
    m_pen.setWidth(w);

}
