#include"header/system/main_linux.h"
#include "header/system/pub_globals.h"

#include <thread>
#include <unistd.h>
#include <QFile>

bool main_p0_start=false;

int main_p0(void)
{


    sys->telecom->install();
    sys->telecom->start();

    while (main_p0_start)
    {
        sys->taskList_p[0]->exec();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}



