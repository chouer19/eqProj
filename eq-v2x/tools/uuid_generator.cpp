#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace std;

string generateUUID(){
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); 
    stringstream sss;
    for(boost::uuids::uuid::const_iterator it=a_uuid.begin(); it!=a_uuid.end(); ++it){
        sss << std::setfill ('0') << std::setw (2) << hex << (int)*it;
    }
    string rawuuid;
    sss >> rawuuid;
    return rawuuid;
}
 
int main(){

    cout << generateUUID() << endl;
    return 1;

    boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); 
    assert(16 == a_uuid.size());
    string uuid_string = boost::uuids::to_string(a_uuid); cout << uuid_string << endl;
    stringstream sss;
    for(boost::uuids::uuid::const_iterator it=a_uuid.begin(); it!=a_uuid.end(); ++it){
        sss << std::setfill ('0') << std::setw (2) << hex << (int)*it;
    }
    string rawuuid;
    sss >> rawuuid;
    cout << rawuuid << endl;
    return 0;
}
