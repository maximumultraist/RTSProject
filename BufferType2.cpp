#include "BufferType2.h"
#include <iostream>

BufferType2::BufferType2() //filling in initial buffer grid
{
    log[0] = std::make_pair(0, 0);
    log[1] = std::make_pair(0, 2);
    log[2] = std::make_pair(3, 6);
}

std::pair<int,int> BufferType2::read(char value)
{
    if(value == 'X'){ //if wanting coordinates of X
        return log[0];
    }

    if(value == 'Y'){ //if wanting coordinates of Y
        return log[1];
    }

    if(value == 'Z'){ //if wanting coordinates of Z
        return log[2];
    }
    std::cout << "Error: invalid value supplied. Cannot read value \'" << value << "\'" << std::endl;
    exit(EXIT_FAILURE);
    return std::make_pair(-1,-1);
}

void BufferType2::write(char value, std::pair<int, int> location)
{
    if(value == 'X'){ //if wanting to write X coordinates
       log[0] = location;
    }
    if(value == 'Y'){ //if wanting to write Y coordinates
        log[1] = location;
    }
    if(value == 'Z'){ //if wanting to write Z coordinates
        log[2] = location;
    }
}

void BufferType2::print() {
    std::cout << "Last location of Plane X (row, column): (" << log[0].first << ", " << log[0].second << ")" << std::endl;
    std::cout << "Last location of Plane Y (row, column): (" << log[1].first << ", " << log[1].second << ")" << std::endl;
    std::cout << "Last location of Plane Z (row, column): (" << log[2].first << ", " << log[2].second << ")" << std::endl;
}