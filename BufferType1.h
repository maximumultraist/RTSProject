#ifndef PROJECT01_BUFFERTYPE1_H
#define PROJECT01_BUFFERTYPE1_H

#include <utility>
#include <unistd.h>
#include <iostream>

class BufferType1
{
public:
    BufferType1();
    std::pair<int,int> read(char value);
    void write(char value, int row, int col);
    void print(); //for testing purposes
private:
    std::pair<int, int> planex;
    std::pair<int, int> planey;
    std::pair<int, int> planez;
};


#endif //PROJECT01_BUFFERTYPE1_H
