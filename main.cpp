#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <numeric>
#include <vector>
#include <mutex>
#include <thread>
#include "BufferType1.h"
#include "BufferType2.h"

int main()
{
    //testing buffertype 1 and making sure both functions work
    BufferType1 bufferA; // creating buffer A

    //testing read function
    std::pair<int,int> pair1, pair2, pair3; //pairs to store value locations
    pair1 = bufferA.read('X'); //pulling location of X
    pair2 = bufferA.read('Y'); //pulling location of Y
    pair3 = bufferA.read('Z'); //pulling location of Z
    std::cout << "buffer A location of X: " << pair1.first <<"," << pair1.second << "\n"; //printing location of X
    std::cout << "buffer A location of Y: " << pair2.first <<"," << pair2.second << "\n"; //printing location of Y
    std::cout << "buffer A location of Z: " << pair3.first <<"," << pair3.second << "\n"; //printing location of Z

    //testing write function
    std::pair<int,int> pair4 (5,3); //pair to store new desired location of value at 5,3
    bufferA.write('X',pair4); //moving X from 0,0 to 5,3
    std::pair<int,int> pair5(bufferA.read('X')); //pair to pull X value from buffer A to check if it moved
    std::cout<< "buffer A new location of X: " << pair5.first << "," << pair5.second << "\n"; //printing new location of X
    bufferA.print(); //testing BufferType1 print functionality


    //testing BufferType2 and making sure both functions work
    BufferType2 bufferC; //creating buffer C
    bufferC.print(); //testing print on newly created blank buffer

    bufferC.write('X',std::make_pair(4,5)); //inserting new X coordinate
    bufferC.print(); //testing print to show write was successful

    std::pair<int,int> pair7 (bufferC.read('X')); //testing bufferType2 read function on X
    std::cout<< "X located at :" << pair7.first << "," << pair7.second << "\n"; //printing just read points
    return 0;
}