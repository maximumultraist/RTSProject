#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <numeric>
#include <vector>
#include <thread>
#include <shared_mutex>
#include <condition_variable>
#include "BufferType1.h"
#include "BufferType2.h"

BufferType1 bufferA;
BufferType1 bufferB;
BufferType2 bufferC;
BufferType2 bufferD;
std::shared_mutex mtx1,mtx2;
std::condition_variable_any proceed1, proceed2;
bool collision = false, readyAtoB = true, checkAtoB = false, checkBtoA = true, readyBtoA = false;
bool lockX, lockY, lockZ;

void reset_locks()
{
    lockX = false;
    lockY = false;
    lockZ = false;
}


void generate_positions () {
    for (auto i = 1; i < 21; i++) {
        if (i%2 == 1) {
                auto temp1 = bufferA.read('X'); // logic to generate new positions
                auto temp2 = bufferA.read('Y');
                auto temp3 = bufferA.read('Z');

                auto x1 = (temp1.first+1)%8;
                auto x2 = (temp1.second+1)%7;
                auto y1 =(temp2.first+1)%8;
                auto y2 = 2;
                auto z1 = 3;
                auto z2 = (temp3.second+1)%7;

                temp1.first = x1;
                temp1.second = x2;
                temp2.first = y1;
                temp2.second = y2;
                temp3.first = z1;
                temp3.second = z2;

            {
                std::unique_lock<std::shared_mutex> lock1(mtx1); // lock buffers
                proceed1.wait(lock1, [] { return readyAtoB; });

                bufferC.write('X', temp1);
                bufferC.write('Y', temp2);
                bufferC.write('Z', temp3);

                //printf("Look, Ma, I generate wrote to buffer B & C in round %d!\n", i);

                readyAtoB = false;
                checkAtoB = true;
                lock1.unlock();
            }

                if (lockX==false)
                    bufferB.write('X', x1, x2);
                if (lockY==false)
                    bufferB.write('Y', y1, y2);
                if (lockZ==false)
                    bufferB.write('Z', z1, z2);

                //bufferB.print();
                //printf("\n\n");

            reset_locks();
            proceed1.notify_all();
        }
        else {
            auto temp1 = bufferB.read('X'); // logic to generate new positions
            auto temp2 = bufferB.read('Y');
            auto temp3 = bufferB.read('Z');

            auto x1 = (temp1.first+1)%8;
            auto x2 = (temp1.second+1)%7;
            auto y1 =(temp2.first+1)%8;
            auto y2 = 2;
            auto z1 = 3;
            auto z2 = (temp3.second+1)%7;

            temp1.first = x1;
            temp1.second = x2;
            temp2.first = y1;
            temp2.second = y2;
            temp3.first = z1;
            temp3.second = z2;

            {
                std::unique_lock<std::shared_mutex> lock2(mtx1); // lock buffers
                proceed1.wait(lock2, [] { return readyAtoB; });

                bufferD.write('X', temp1);
                bufferD.write('Y', temp2);
                bufferD.write('Z', temp3);

                //printf("Look, Ma, I generate wrote to buffer B & C in round %d!\n", i);

                readyBtoA = false;
                checkBtoA = true;
                lock2.unlock();
            }

            if (lockX==false)
                bufferA.write('X', x1, x2);
            if (lockY==false)
                bufferA.write('Y', y1, y2);
            if (lockZ==false)
                bufferA.write('Z', z1, z2);
            //bufferA.print();

            //printf("\n\n");

            reset_locks();
            proceed2.notify_all();
        }
        usleep(100);
    }
}

void detect_and_avoid_collisions () {
    usleep(100);
    for (auto i = 1; i < 21; i++) {
        collision = false;
        if (i%2 == 0) {
            {
                std::shared_lock<std::shared_mutex> lock(mtx1);
                proceed1.wait(lock, []{return checkAtoB;});

                if (bufferC.read('X')==bufferC.read('Y')) {
                    std::cout << "Collision detected with X and Y in round " << i << " located at ("
                              << bufferC.read('X').first << "," << bufferC.read('X').second << ")\n";
                    collision = true;
                }

                if (bufferC.read('X')==bufferC.read('Z')) {
                    std::cout << "Collision detected with X and Z in round " << i << " located at ("
                              << bufferC.read('X').first << "," << bufferC.read('Z').second << ")\n";
                    collision = true;
                }
                if (bufferC.read('Y')==bufferC.read('Z')) {
                    std::cout << "Collision detected with Y and Z in round " << i << " located at ("
                              << bufferC.read('Y').first << "," << bufferC.read('Z').second << ")\n";
                    collision = true;
                }
                checkAtoB = false;
                readyAtoB = true;
                lock.unlock();
                proceed1.notify_all();
            }
        }
        else {
            {
                std::shared_lock<std::shared_mutex> lock(mtx1);
                proceed2.wait(lock, []{return checkBtoA;});

                if (bufferD.read('X')==bufferD.read('Y')) {
                    std::cout << "Collision detected with X and Y in round " << i << " located at ("
                              << bufferD.read('X').first << "," << bufferD.read('X').second << ")\n";
                    collision = true;
                }

                if (bufferD.read('X')==bufferD.read('Z')) {
                    std::cout << "Collision detected with X and Z in round " << i << " located at ("
                              << bufferD.read('X').first << "," << bufferD.read('Z').second << ")\n";
                    collision = true;
                }
                if (bufferD.read('Y')==bufferD.read('Z')) {
                    std::cout << "Collision detected with Y and Z in round " << i << " located at ("
                              << bufferD.read('Y').first << "," << bufferD.read('Z').second << ")\n";
                    collision = true;
                }
                checkBtoA = false;
                readyBtoA = true;
                lock.unlock();
                proceed2.notify_all();

            }
        }
        if (!collision)
            std::cout<<"No collision detected in round: "<<i<<"\n";
        usleep(100);
    }
}

int main() {
    reset_locks();
    std::thread p1 (generate_positions);
    std::thread p3 (detect_and_avoid_collisions);
    p1.join();
    p3.join();

    return 0;
}