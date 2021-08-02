#include <vector>
#include <iostream>
#include "ser_outside_xc/eqDriving/serial/SimpleSerial.hpp"

using namespace std;

using namespace eqDriving;
using namespace io;
int main(){
    SimpleSerial sp;
    sp.open("/dev/ttyUSB0",115200);
    while(true){
        cout << "dafdsafdasfdsafsad\n";
        cout << sp.readLine() << endl;
    }
}
