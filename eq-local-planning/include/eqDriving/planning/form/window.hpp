#ifndef WINDOW_H
#define WINDOW_H

#include "eqDriving/planning/form/helper.hpp"

#include <QWidget>

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private:
    Helper helper;
};
//! [0]

#endif
