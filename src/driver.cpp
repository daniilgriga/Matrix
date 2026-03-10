#include "processor.hpp"

#include <cstring>
#include <iostream>
#include <exception>

int main (int argc, char* argv[])
{
    try
    {
        // usage: matrix [--chain]
        if (argc > 1 && std::strcmp (argv[1], "--chain") == 0)
            proc::process_chain();
        else
            proc::process_det();

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;

        return 1;
    }
}
