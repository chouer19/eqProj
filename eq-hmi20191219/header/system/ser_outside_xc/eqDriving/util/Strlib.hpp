#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace eqDriving{
    namespace util{
        // Class splitstring which adds method split()
        class SplitString : public string {
            vector<string> flds;
        public:
            SplitString(char *s) : string(s) { };
            SplitString(string s) : string(s) { };
            vector<string>& split(char delim, int rep=0);
        };
    }
}
