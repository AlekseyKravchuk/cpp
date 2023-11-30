#include <iostream>
#include <unistd.h>
#include <getopt.h>


void ProcessArgs(int argc, char* argv[]) {
    const char* const short_opts = "dr";
    const option long_opts[] = {
                                {"direct", no_argument, nullptr, 'd'},
                                {"reverse", no_argument, nullptr, 'b'}
    };

    while(true) {
        const auto opt = getopt_long(argc, argv, )
    }
}


int main(int argc, char* argv[]) {
    // extern char *optarg; // A pointer to the current option argument, if there is one.
    // extern int optopt;   // The last known option
    // extern int optind;   // An index of the next argv pointer to process when getopt() is called again.

    int opt{0};
    const char optstring[] = "abf:?";

    // std::cout << "The program " << argv[0] << " was invoked with " << argc-1 << " options" << std::endl;
    // for(int i = 1; i < argc; i++) {
    //     std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    // }

    // const char optstring[] = "abf:?";
    while( (opt = getopt(argc, argv, optstring)) != -1 ) {
        switch(opt) {
            case 'a':
                std::cout << "Option 'a' is found." << std::endl;
                break;
            case 'b':
                std::cout << "Option 'b' is found." << std::endl;
                break;
            case 'f':
                if(optarg) {
                    std::cout << "Option 'f' with argument " << optarg << " is found." << std::endl;
                }

        }
    }
}
