#ifndef RTSPROJECT_BUFFERTYPE2_H
#define RTSPROJECT_BUFFERTYPE2_H

#include <utility>
#include <mutex>

class BufferType2
{
public:
    BufferType2();
    void write(char value, std::pair<int,int> location);
    std::pair<int,int> read(char value);
    void print(); //for testing purposes
private:
    int array[3][2]; //char array to store locations of values
};


#endif //RTSPROJECT_BUFFERTYPE2_H
