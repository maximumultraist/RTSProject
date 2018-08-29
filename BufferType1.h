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
    char read(int planeID);
    void write(int planeID, std::pair<int,int> location);
private:
    std::mutex mtx;
    char plane1[8][7];
    char plane2[8][7];
    char plane3[8][7];
};


#endif //PROJECT01_BUFFERTYPE1_H
