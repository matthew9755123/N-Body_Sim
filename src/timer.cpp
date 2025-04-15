#pragma once
#include <iostream>
#include <cstdlib>

#include "timer.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " N" << std::endl;
        return -1;
    }

    size_t loops = std::stoul(argv[1]); // convert from argv[1];
    timer t;
    double a = 3.14, b = 3.14159, c = 0.0;
    t.start();
    for (size_t i = 0; i < loops; ++i)
        c += a * b;
    t.stop();

    std::cout << loops << " loops took " << t.elapsed() << " milliseconds" << std::endl;
    return 0;
}