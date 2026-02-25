#include "processor.hpp"

#include <iostream>
#include <exception>

int main ()
{
    try
    {
        proc::process_det();

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;

        return 1;
    }
}
