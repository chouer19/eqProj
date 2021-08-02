#include "eqDriving/planning/form/window.hpp"

#include <QApplication>
#include <QSurfaceFormat>

void test_abc();

int main(int argc, char *argv[])
{
    //test_abc();
    QApplication app(argc, argv);

    //QSurfaceFormat fmt;
    //fmt.setSamples(4);
    //QSurfaceFormat::setDefaultFormat(fmt);

    Window window;
    window.show();
    return app.exec();
}

#include "eqDriving/vehicle/Control.hpp"
#include "eqDriving/vehicle/State.hpp"
#include "eqDriving/math/Pose3.hpp"
#include "eqDriving/math/Vector3.hpp"
using namespace eqDriving;
using namespace vehicle;
void test_abc(){

    State state;
    state.Velocity(3,3,3);
    vector<Pose3d> poses;
    Vector3d cmder = purepursuit(state, poses, [](double v){return v * v + 3;});
    cout << cmder << endl;
}
