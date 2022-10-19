#include <iostream>

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)

int main() {
    int UNIQUE_VAR_NAME_PROFILE;

    // std::cout << UNIQUE_VAR_NAME_PROFILE << std::endl;
    return 0;
}
