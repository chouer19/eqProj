#include "header/system/pub_globals.h"
#include "header/system/sys_abstract.hpp"
#include"header/algorithm/alg_abstract.hpp"
#include "header/system/main_linux.h"
#include "controlcan.h"
#include <unistd.h>
#include <thread>

bool main_p2_start=false;
int main_p2(void)
{

//    algorithm->latctrl->install();
//    algorithm->latctrl->start();
//    algorithm->lonctrl->install();
//    algorithm->lonctrl->start();
//    algorithm->speed->install();
//    algorithm->speed->start();
//    algorithm->path->install();
//    algorithm->path->start();

    while (main_p2_start)
    {
        sys->taskList_p[2]->exec();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}
