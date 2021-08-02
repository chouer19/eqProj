#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include<mutex>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);

    bool outThis();

signals:

private slots:
    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void enterEvent(QEvent *event);

    void leaveEvent(QEvent *event);

private:
    std::mutex m_mutex;

    bool outthis=true;
    bool setOutThis(bool _outthis);

};

#endif // MYWIDGET_H
