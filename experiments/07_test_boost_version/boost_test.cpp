#include <iostream>
#include <boost/version.hpp>

/*
============================== Boost installation ==============================
sudo apt update && sudo apt upgrade
sudo apt install build-essential
wget https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.gz
tar -xzvf boost_1_86_0.tar.gz
cd boost_1_86_0/
./bootstrap.sh --prefix=/usr/local
sudo ./b2 install
================================================================================
*/

int main() {
  std::cout << "Boost library version: " << BOOST_LIB_VERSION << '\n';
}


