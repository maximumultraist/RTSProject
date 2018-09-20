#include "BufferType1.h"

BufferType1::BufferType1()
{
    planex = std::make_pair(0, 0);
    planey = std::make_pair(0, 2);
    planez = std::make_pair(3, 6);
}

std::pair<int,int> BufferType1::read(char value) //returns coordinates of value passed (X, Y, or Z)
{
    if(value == 'X') { //if wanting coordinates of X
        return planex;
    }
    else if(value == 'Y') { //if wanting coordinates of Y
        return planey;
    }
    else if(value == 'Z') { //if wanting coordinates of Z
        return planez;
    }

    std::cout << "Error: invalid value supplied -- cannot read from \'" << value << "\'" << std::endl;
    return std::make_pair(-1,-1);
}

void BufferType1::write(char value, int row, int col) // writes values to selected plane type
{
    if ((row <= 7 && col <=6) && (row >= 0 && col >= 0)) { // make sure we are in bounds
        if (value == 'X') {
            planex.first = row;
            planex.second = col;
        } if (value == 'Y') {
            planey.first = row;
            planey.second = col;
        } if (value == 'Z') {
            planez.first = row;
            planez.second = col;
        }
    }
    else {
        std::cout
                << "Error: invalid value supplied. This is actually pretty hard to do. Unfortunately, the program will die now."
                << std::endl;
        exit(EXIT_FAILURE);
    }
}

void BufferType1::print() //for testing purposes
{
    std::cout << "Plane X coordinates (row, column): (" << planex.first << ", " << planex.second << ")" << std::endl;
    std::cout << "Plane Y coordinates (row, column): (" << planey.first << ", " << planey.second << ")" << std::endl;
    std::cout << "Plane Z coordinates (row, column): (" << planez.first << ", " << planez.second << ")" << std::endl;
};

