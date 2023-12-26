#pragma once

#include <fstream>
#include <iostream>

class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input);
    ~RedirectStandardInput();

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};