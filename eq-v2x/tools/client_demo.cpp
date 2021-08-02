//======================================================================
/* ! \file client_demo.cpp     
                      
 *                    
 * \copydoc Copyright 
 * \author Xue Chong(xc)             
 * \date July 30, 2019
 *                    
 * demo of using eqSingleClient class to write or read data from cloud server
 * Usage:  ????????????
 * client for connectint cloud, send and receive message
 *///------------------------------------------------------------------- 
#include <iostream>
#include "ser_outside_xc/eqDriving/v2x/eqSingletonClient.hpp"
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <string.h>
#include <stdlib.h>     /* atoi */

int utcTime(){
    //return boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
    return to_tm(boost::posix_time::second_clock::universal_time()).tm_sec;
} 

int utc = 0;

void wait(int seconds)                                                                        
{     
  boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
} 

void wait_m(int miliseconds){
  boost::this_thread::sleep_for(boost::chrono::milliseconds{miliseconds});
}

using namespace std;
//Initialize pointer to zero so that it can be initialized in first call to getInstance
SingletonClient *SingletonClient::instance = 0;

void write_login_thread(){
   SingletonClient *s = s->getInstance();
   //char jianquan[20];
    while(true){
        wait_m(100);
        if(s->jianquan_valid()){continue;}
        s->test_login();
        wait_m(200);
        //s->jianquan();
        cout << "login\n";
    }
}

void rap_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->rap();
        //wait(5);
        wait(1);
    }
}
void request_assignment_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->request_assignment();
        wait_m(500);
    }
}
void report_message_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->report_message();
        //wait_m(500);
        wait_m(1000);
        //wait_m(10);
    }
}
void bufa_message_thread(){
    SingletonClient *s = s->getInstance();
    while(true){
        s->bufa_message();
        wait_m(500);
    }
}

void update_gis_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->update_gis();
        wait_m(500);
        wait_m(500);
        wait_m(500);
        wait_m(500);
    }
}
void get_map_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        //s->get_map();
        wait_m(500);
        wait(3);
        //wait(12);
    }
}

void report_rtcm_thread(){

}
void respond_thread(){
   SingletonClient *s = s->getInstance();
    while(true){
        s->respond_ontime();
        wait_m(100);
    }
}

//void test_gis(int timm){
void test_gis(){
    int base = utcTime();
   SingletonClient *s = s->getInstance();
    size_t size = 0;
    while(true){
        for(int i =0; i< 10; i++){
            s->update_gis();
            if(utcTime()-utc > 5 || (utcTime() - utc > 60 - utc && utc > 55)){
                s->rap();
                utc = utcTime();
            }
            //cout << "updating.........  step :  "  << size++ << endl;
        }
        wait_m(2);
    }
    cout << "dddddddddddddddddddddddddd\n";

}

void test_run(){
    while(true){
        wait(1);
        cout << "alive ..............\n";
    }
}

int main(int argc, char ** argv){
   utc = utcTime();
   SingletonClient *s = s->getInstance();
   //while(true){
   //s->start_connect();
   //}
   SingletonClient *r = r->getInstance();

   //s->start();
   //cout << "two client\n";
   boost::thread t1(&SingletonClient::read_thread, s);
   boost::thread t2(write_login_thread);
   //boost::thread t3(&SingletonClient::respond_thread, s);
   boost::thread t5(get_map_thread);
   boost::thread t6(rap_thread);
   boost::thread t7(report_message_thread);
   //int count = atoi(argv[1]);
   //boost::thread t8(test_gis, count);
   //boost::thread t8(test_gis) ;
   boost::thread t10(&SingletonClient::write_thread, s);
   boost::thread t11(&SingletonClient::run_thread, s);

   //while(true){
   //    s->Run();
   //}
   t11.join();
   t1.join();
   t2.join();
   //t3.join();
   t5.join();
   t6.join();
   t7.join();
   //t8.join();
   t10.join();

   return 0;
}
