#ifndef RTSPROJECT_BUFFERTYPE2_H
#define RTSPROJECT_BUFFERTYPE2_H

#include <utility>
#include <vector>
#include <unistd.h>

class BufferType2 {
public:
    BufferType2();
    void write(char value, std::pair<int, int> location);
    std::pair<int, int> read(char value);
    void print(); //for testing purposes
private:
    std::vector<std::pair<int, int> > log {3};
};
#endif //RTSPROJECT_BUFFERTYPE2_H
