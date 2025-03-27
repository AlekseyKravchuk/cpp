# toolchain.cmake

# Указываем путь к компиляторам
set(CMAKE_C_COMPILER clang-20)
set(CMAKE_CXX_COMPILER clang++-20)

# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

# Определяем стандарт C++
set(CMAKE_CXX_STANDARD 20)

# Прочие настройки компилятора (если необходимо)
# set(CMAKE_CXX_FLAGS "-O2 -Wall")
