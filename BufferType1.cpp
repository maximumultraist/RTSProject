#include "BufferType1.h"
#include <mutex>

BufferType1::BufferType1()
{
    plane1[0][0] = 'X';
    plane2[0][2] = 'Y';
    plane3[3][6] = 'Z';
}

BufferType1::BufferType1(std::pair<int, int> Xlocation,
                         std::pair<int, int> YlocationY,
                         std::pair<int, int> ZlocationZ)
{
    plane1[Xlocation.first][Xlocation.second] = 'X';
    plane2[Xlocation.first][Xlocation.second] = 'Y';
    plane3[Xlocation.first][Xlocation.second] = 'Z';
}

char BufferType1::read(int planeID)
{

}

void BufferType1::write(int planeID, std::pair<int, int> location)
{

    
}

