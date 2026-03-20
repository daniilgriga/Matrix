#include <iostream>
#include <exception>

#include "processor.hpp"

int main()
{
    try
    {
        proc::process_chain();

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;

        return 1;
    }
}
