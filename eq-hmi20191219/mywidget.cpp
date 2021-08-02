#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{

}

bool MyWidget::outThis()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return outthis;
}

void MyWidget::mousePressEvent(QMouseEvent *event)
{

}

void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void MyWidget::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void MyWidget::enterEvent(QEvent *event)
{
    setOutThis(false);
}

void MyWidget::leaveEvent(QEvent *event)
{
    setOutThis(true);
}

bool MyWidget::setOutThis(bool _outthis)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    outthis=_outthis;
}
