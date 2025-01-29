#include "option.h"

int main() {
    OptionsDescription desc;

    desc.add_options()
            ("help", "Show help message")
            ("version", "Show version information")
            ("some_option", "Some option to test");

    desc.print();

    return 0;
}
