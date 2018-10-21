#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <numeric>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <condition_variable>
#include "BufferType1.h"
#include "BufferType2.h"

BufferType1 bufferA;
BufferType1 bufferB;
BufferType2 bufferC;
BufferType2 bufferD;
std::mutex mtx1,mtx2;
std::condition_variable proceed1, proceed2;
bool collision = false, readyAtoB = true, checkAtoB = false, checkBtoA = true, readyBtoA = false;
bool lockX, lockY, lockZ, xf = false, yf = false;
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
                std::unique_lock<std::mutex> lock1(mtx1); // lock buffers
                proceed1.wait(lock1, [] { return readyAtoB; });

                bufferC.write('X', temp1);
                bufferC.write('Y', temp2);
                bufferC.write('Z', temp3);

                readyAtoB = false;
                checkAtoB = true;
                lock1.unlock();
            }
            proceed1.notify_all();

            if (!lockX)
                bufferB.write('X', x1, x2);
            if (!lockY)
                bufferB.write('Y', y1, y2);
            if (!lockZ)
                bufferB.write('Z', z1, z2);

            // bufferB.print();
            // printf("\n\n");

            reset_locks();
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
                std::unique_lock<std::mutex> lock2(mtx2); // lock buffers
                proceed1.wait(lock2, [] { return readyAtoB; });

                bufferD.write('X', temp1);
                bufferD.write('Y', temp2);
                bufferD.write('Z', temp3);

                readyBtoA = false;
                checkBtoA = true;
                lock2.unlock();
            }
            proceed2.notify_all();

            if (!lockX)
                bufferA.write('X', x1, x2);
            if (!lockY)
                bufferA.write('Y', y1, y2);
            if (!lockZ)
                bufferA.write('Z', z1, z2);

            // bufferA.print();
            // printf("\n\n");

            reset_locks();

        }
        //usleep(10);
    }
}

void detect_and_avoid_collisions (int failx1, int failx2, int faily) {
    usleep(10);
    for (auto i = 1; i < 21; i++) {
        collision = false;
        if (i%2 == 0) {
            {
                std::unique_lock<std::mutex> lock(mtx1);
                proceed1.wait(lock, []{return checkAtoB;});

                if (bufferB.read('X')==bufferB.read('Y')) {
                    std::cout << "Collision detected with X and Y in round " << i << " located at ("
                             << bufferB.read('X').first << "," << bufferB.read('X').second << ")\n";
                    collision = true;
                }

                if (bufferB.read('X')==bufferB.read('Z')) {
                    std::cout << "Collision detected with X and Z in round " << i << " located at ("
                             << bufferB.read('X').first << "," << bufferB.read('Z').second << ")\n";
                    collision = true;
                }
                if (bufferB.read('Y')==bufferB.read('Z')) {
                    std::cout << "Collision detected with Y and Z in round " << i << " located at ("
                             << bufferB.read('Y').first << "," << bufferB.read('Z').second << ")\n";
                    collision = true;
                }

                auto temp1 = bufferC.read('X'); // logic to generate new positions
                auto temp2 = bufferC.read('Y');
                auto temp3 = bufferC.read('Z');


                for (auto k = 0; k < 2; k++) {
                    temp1.first = (temp1.first+1)%8;
                    temp1.second = (temp1.second+1)%7;
                    temp2.first = (temp2.first+1)%8;
                    temp2.second = 2;
                    temp3.first = 3;
                    temp3.second = (temp3.second+1)%7;
                }

                if (failx1 == i || failx2 == i || xf) {
                    xf = true;
                    std::cout << "X failed to stop, trying to manage fault!!" << std::endl;
                    if (temp1==temp2) {
                        lockY = true;
                        if (temp1==temp3) {
                            lockZ = true;
                            std::cout << "Collision avoidance attempted in failure of X between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision avoidance attempted in failure of X between X and Y in round " << i+2
                                      << std::endl;
                    }
                    if (temp1==temp3) {
                        lockY = true;
                        if (temp2==temp3) {
                            lockZ = true;
                            std::cout << "Collision predicted and avoided between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Z in round " << i+2
                                      << std::endl;
                    }
                    if (temp2==temp3) {
                        lockZ = true;
                        if (temp1==temp3) {
                            lockY = true;
                            std::cout << "Collision avoidance attempted in failure of X between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between Y and Z in round " << i+2
                                      << std::endl;
                    }
                }
                if (faily == i || yf) {
                    yf = true;
                    std::cout << "Y failed to stop, trying to manage fault!!" << std::endl;
                    if (temp1==temp2) {
                        lockX = true;
                        if (temp1==temp3) {
                            lockZ = true;
                            std::cout << "Collision avoidance attempted in failure of Y between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Y in round " << i+2
                                      << std::endl;
                    }
                    if (temp1==temp3) {
                        lockZ = true;
                        if (temp2==temp3) {
                            lockX = true;
                            std::cout << "Collision avoidance attempted in failure of Y between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision avoidance attempted in failure of Y between X and Z in round " << i+2
                                      << std::endl;
                    }
                    if (temp2==temp3) {
                        lockZ = true;
                        if (temp1==temp3) {
                            lockX = true;
                            std::cout << "Collision predicted and avoided between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between Y and Z in round " << i+2
                                      << std::endl;
                    }
                } else {
                    if (temp1 == temp2) {
                        lockX = true;
                        if (temp1 == temp3) {
                            lockY = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Y in round " << i+2 << std::endl;
                    }
                    if (temp1 == temp3) {
                        lockY = true;
                        if (temp2 == temp3) {
                            lockZ = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout<<"Collision predicted and avoided between X and Z in round "<<i+2<<std::endl;
                    }
                    if (temp2 == temp3) {
                        lockZ = true;
                        if (temp1 == temp3) {
                            lockX = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout<<"Collision predicted and avoided between Y and Z in round "<<i+2<<std::endl;
                    }
                }

                checkAtoB = false;
                readyAtoB = true;
                lock.unlock();
                proceed1.notify_all();
            }
        }
        else {
            {
                std::unique_lock<std::mutex> lock(mtx2);
                proceed2.wait(lock, []{return checkBtoA;});

                if (bufferA.read('X')==bufferA.read('Y')) {
                    std::cout << "Collision detected with X and Y in round " << i << " located at ("
                              << bufferA.read('X').first << "," << bufferA.read('X').second << ")\n";
                    collision = true;
                    collision = true;
                }

                if (bufferA.read('X')==bufferA.read('Z')) {
                    std::cout << "Collision detected with X and Z in round " << i << " located at ("
                              << bufferA.read('X').first << "," << bufferA.read('Z').second << ")\n";
                    collision = true;
                }
                if (bufferA.read('Y')==bufferA.read('Z')) {
                    std::cout << "Collision detected with Y and Z in round " << i << " located at ("
                             << bufferA.read('Y').first << "," << bufferA.read('Z').second << ")\n";
                    collision = true;
                }

                auto temp1 = bufferD.read('X');
                auto temp2 = bufferD.read('Y');
                auto temp3 = bufferD.read('Z');

                for (auto k = 0; k < 2; k++) {
                    temp1.first = (temp1.first+1)%8;
                    temp1.second = (temp1.second+1)%7;
                    temp2.first = (temp2.first+1)%8;
                    temp2.second = 2;
                    temp3.first = 3;
                    temp3.second = (temp3.second+1)%7;
                }

                if (failx1 == i || failx2 == i || xf) {
                    xf = true;
                    std::cout << "X failed to stop, trying to manage fault!!" << std::endl;
                    if (temp1==temp2) {
                        lockY = true;
                        if (temp1==temp3) {
                            lockZ = true;
                            std::cout << "Collision avoidance attempted in failure of X between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision avoidance attempted in failure of X between X and Y in round " << i+2
                                      << std::endl;
                    }
                    if (temp1==temp3) {
                        lockY = true;
                        if (temp2==temp3) {
                            lockZ = true;
                            std::cout << "Collision predicted and avoided between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Z in round " << i+2
                                      << std::endl;
                    }
                    if (temp2==temp3) {
                        lockZ = true;
                        if (temp1==temp3) {
                            lockY = true;
                            std::cout << "Collision avoidance attempted in failure of X between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between Y and Z in round " << i+2
                                      << std::endl;
                    }
                }
                if (faily == i || yf) {
                    yf = true;
                    std::cout << "Y failed to stop, trying to manage fault!!" << std::endl;
                    if (temp1==temp2) {
                        lockX = true;
                        if (temp1==temp3) {
                            lockZ = true;
                            std::cout << "Collision avoidance attempted in failure of Y between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Y in round " << i+2
                                      << std::endl;
                    }
                    if (temp1==temp3) {
                        lockZ = true;
                        if (temp2==temp3) {
                            lockX = true;
                            std::cout << "Collision avoidance attempted in failure of Y between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision avoidance attempted in failure of Y between X and Z in round " << i+2
                                      << std::endl;
                    }
                    if (temp2==temp3) {
                        lockZ = true;
                        if (temp1==temp3) {
                            lockX = true;
                            std::cout << "Collision predicted and avoided between X, Y, and Z in round " << i+2
                                      << std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between Y and Z in round " << i+2
                                      << std::endl;
                    }
                } else {
                    if (temp1 == temp2) {
                        lockX = true;
                        if (temp1 == temp3) {
                            lockY = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout << "Collision predicted and avoided between X and Y in round " << i+2 << std::endl;
                    }
                    if (temp1 == temp3) {
                        lockY = true;
                        if (temp2 == temp3) {
                            lockZ = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout<<"Collision predicted and avoided between X and Z in round "<<i+2<<std::endl;
                    }
                    if (temp2 == temp3) {
                        lockZ = true;
                        if (temp1 == temp3) {
                            lockX = true;
                            std::cout<<"Collision predicted and avoided between X, Y, and Z in round "<<i+2<<std::endl;
                        }
                        else
                            std::cout<<"Collision predicted and avoided between Y and Z in round "<<i+2<<std::endl;
                    }
                }


                checkBtoA = false;
                readyBtoA = true;
                lock.unlock();
                proceed2.notify_all();

            }
        }
        if (!collision)
            std::cout<<"No collision detected in round: "<<i<<"\n";
        usleep(10);
    }
}

int main() {
    FILE *random = fopen("/dev/urandom", "r");
    auto seed = (unsigned int)getc(random);
    srand(seed);
    int failx1 = rand() % 20;
    int failx2 = rand() % 20;
    int faily = rand() % 20;

    reset_locks();
    std::thread p1 (generate_positions);
    std::thread p3 (detect_and_avoid_collisions, failx1, failx2, faily);
    p1.join();
    p3.join();

    return 0;
}