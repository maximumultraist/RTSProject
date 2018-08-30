#include "BufferType2.h"
#include <mutex>
#include <iostream>

BufferType2::BufferType2() //filling in initial buffer grid
{
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            array[i][j] = NULL;
        }
    }
}

std::pair<int,int> BufferType2::read(char value)
{
    if(value == 'X'){ //if wanting coordinates of X
        return (std::make_pair((int)array[0][0],(int)array[0][1]));
    }

    else if(value == 'Y'){ //if wanting coordinates of Y
        return (std::make_pair((int)array[1][0],(int)array[1][1]));
    }

    else if(value == 'Z'){ //if wanting coordinates of Z
        return (std::make_pair((int)array[2][0],(int)array[2][1]));
    }
}

void BufferType2::write(char value, std::pair<int, int> location)
{
    if(value == 'X'){ //if wanting to write X coordinates
        array[0][0] = location.first;
        array[0][1] = location.second;
    }
    else if(value == 'Y'){ //if wanting to write Y coordinates
        array[1][0] = location.first;
        array[1][1] = location.second;
    }
    else if(value == 'Z'){ //if wanting to write Z coordinates
        array[2][0] = location.first;
        array[2][1] = location.second;
    }
}

void BufferType2::print()
{
    for(int i = 0; i < 3; i++){
        if(i == 0)
            std::cout<<"X ";
        else if(i == 1)
            std::cout<<"Y ";
        else if(i == 2)
            std::cout<<"Z ";
        for(int j = 0; j < 2; j++){
            if(array[i][j] == NULL){
                std::cout << " - ";
            } else{
                std::cout << " " << array[i][j] << " ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}