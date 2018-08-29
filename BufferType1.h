#ifndef PROJECT01_BUFFERTYPE1_H
#define PROJECT01_BUFFERTYPE1_H

#include <utility>
#include <mutex>

class BufferType1
{
public:
    BufferType1();
    BufferType1(std::pair<int,int> Xlocation,
                std::pair<int,int> Ylocation,
                std::pair<int,int> Zlocation);
    std::pair<int,int> read(char value);
    void write(char value, std::pair<int,int> location);
private:
    std::mutex rdmtx, wrmtx; //a mutex for read and write
    char plane1[8][7];
    char plane2[8][7];
    char plane3[8][7];
};


#endif //PROJECT01_BUFFERTYPE1_H
