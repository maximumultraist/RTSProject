#include "BufferType1.h"
#include <mutex>
#include <iostream>

BufferType1::BufferType1()
{
    for(int i = 0; i < 8; i++){ //fills initial X grid with null values
        for(int j = 0; j < 7; j++){
            plane1[i][j] = NULL;
        }
    }
    plane1[0][0] = 'X'; //places initial X

    for(int i = 0; i < 8; i++){ //fills initial Y grid with null values
        for(int j = 0; j < 7; j++){
            plane2[i][j] = NULL;
        }
    }
    plane2[0][2] = 'Y'; //places initial Y

    for(int i = 0; i < 8; i++){ //fills initial Z grid with null values
        for(int j = 0; j < 7; j++){
            plane3[i][j] = NULL;
        }
    }
    plane3[3][6] = 'Z'; //places initial Z
}

std::pair<int,int> BufferType1::read(char value) //returns coordinates of value passed (X, Y, or Z)
{
    std::lock_guard<std::mutex> lock(rdmtx); //locks function until completion

    if(value == 'X') { //if wanting coordinates of X
        for(int i = 0; i < 8; i ++){
            for(int j = 0; j < 7; j++){
                if(plane1[i][j] == 'X'){
                    return std::make_pair(i,j);
                }
            }
        }
    }
    else if(value == 'Y') { //if wanting coordinates of Y
        for(int i = 0; i < 8; i ++){
            for(int j = 0; j < 7; j++){
                if(plane2[i][j] == 'Y'){
                    return std::make_pair(i,j);
                }
            }
        }
    }
    else if(value == 'Z') { //if wanting coordinates of Z
        for(int i = 0; i < 8; i ++){
            for(int j = 0; j < 7; j++){
                if(plane3[i][j] == 'Z'){
                    return std::make_pair(i,j);
                }
            }
        }
    }
}

void BufferType1::write(char value, std::pair<int, int> location)
{
    std::lock_guard<std::mutex> lock(wrmtx); //locks function until completion

    if(value == 'X'){ //if wanting to move X
        for(int i = 0; i < 8; i++){ //removing old placement of X
            for(int j = 0; j < 7; j++){
                if(plane1[i][j] == 'X')
                    plane1[i][j] = NULL;
            }
        }
        for(int i = 0; i < 8; i++){ //placing X in new placement
            for(int j = 0; j < 7; j++){
                if(i == location.first && j == location.second)
                    plane1[i][j] = 'X';
            }
        }
    }

    if(value == 'Y'){ //if wanting to move Y
        for(int i = 0; i < 8; i++){ //removing old placement of Y
            for(int j = 0; j < 7; j++){
                if(plane1[i][j] == 'Y')
                    plane1[i][j] = NULL;
            }
        }
        for(int i = 0; i < 8; i++){ //placing Y in new placement
            for(int j = 0; j < 7; j++){
                if(i == location.first && j == location.second)
                    plane1[i][j] = 'Y';
            }
        }
    }

    if(value == 'Z'){ //if wanting to move Z
        for(int i = 0; i < 8; i++){ //removing old placement of Z
            for(int j = 0; j < 7; j++){
                if(plane1[i][j] == 'Z')
                    plane1[i][j] = NULL;
            }
        }
        for(int i = 0; i < 8; i++){ //placing Z in new placement
            for(int j = 0; j < 7; j++){
                if(i == location.first && j == location.second)
                    plane1[i][j] = 'Z';
            }
        }
    }
}

void BufferType1::print() //for testing purposes
{
    for(int i = 0; i < 8; i++){ //printing X coordinates
        for(int j = 0; j < 7; j++){
            if(plane1[i][j] != NULL)
                std::cout << "X coordinates: " << i << "," << j << "\n";
        }
    }

    for(int i = 0; i < 8; i++){ //printing Y coordinates
        for(int j = 0; j < 7; j++){
            if(plane2[i][j] != NULL)
                std::cout << "Y coordinates: " << i << "," << j << "\n";
        }
    }

    for(int i = 0; i < 8; i++){ //printing Z coordinates
        for(int j = 0; j < 7; j++){
            if(plane3[i][j] != NULL)
                std::cout << "Z coordinates: " << i << "," << j << "\n";
        }
    }
}

