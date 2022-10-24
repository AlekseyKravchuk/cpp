#include <fstream>
#include <string>

#include "log_duration.h"

using namespace std;

int main() {
    ofstream out_file("ballad.txt"s);
    // {
    //     LOG_DURATION("Writing to the file without flushing buffer");
    //     for (int i = 0; i < 1000000; ++i) {
    //         out_file << "С любимыми не расставайтесь\n"s;
    //     }
    // }
    
    {
        LOG_DURATION("Writing to the file and DO flushing buffer");
        for (int i = 0; i < 1000000; ++i) {
            out_file << "С любимыми не расставайтесь"s << endl;
        }
    }

    throw;
}