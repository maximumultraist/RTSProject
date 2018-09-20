#include <iostream>
#include <unistd.h>
#include <cmath>
#include <numeric>
#include <vector>
#include <thread>
#include <mutex>
#include <pthread.h>
#include "BufferType1.h"
#include "BufferType2.h"

BufferType1 bufferA;
BufferType1 bufferB;
BufferType2 bufferC;
BufferType2 bufferD;
std::mutex mtx1,mtx2;;
bool collision, ready1, ready2, ready3;
bool lockX, lockY, lockZ;

void reset_locks()
{
    lockX = false;
    lockY = false;
    lockZ = false;
}

void reset_waits()
{
    ready1 = true;
    ready2 = false;
    ready3 = false;
}

void generate_positions () {
    for (int i = 1; i < 21; i++) {
        while(!ready1){
            //for synchronization
        }
        if (i%2 == 1) {
            std::unique_lock<std::mutex> lock(mtx1); // lock buffers
            auto temp1 = bufferA.read('X'); // logic to generate new positions
            auto temp2 = bufferA.read('Y');
            auto temp3 = bufferA.read('Z');

            if(lockX == false)
                bufferB.write('X', (temp1.first + 1) % 8, (temp1.second + 1) % 7);
            if(lockY == false)
                bufferB.write('Y', (temp2.first + 1) % 8, 2);
            if(lockZ == false)
                bufferB.write('Z', 3, (temp3.second + 1) % 7);
        }
        else {
            std::unique_lock<std::mutex> lock(mtx1);// lock buffers
            auto temp5 = bufferB.read('X'); // logic to generate new positions
            auto temp6 = bufferB.read('Y');
            auto temp7 = bufferB.read('Z');

            if(lockX == false)
                bufferA.write('X', (temp5.first + 1) % 8, (temp5.second + 1) % 7);
            if(lockY == false)
                bufferA.write('Y', (temp6.first + 1) % 8, 2);
            if(lockZ == false)
                bufferA.write('Z', 3, (temp7.second + 1) % 7);
        }
        reset_locks();
        ready1 = false;
        ready2 = true;
        sleep(1);
    }
}

void record_positions () {
    for (int i = 1; i < 21; i++) {
        while(!ready2) {
            //for synchronization
        }
        if (i%2 == 1) {
            std::unique_lock<std::mutex> lock(mtx2);
            bufferC.write('X', bufferA.read('X'));
            bufferC.write('Y', bufferA.read('Y'));
            bufferC.write('Z', bufferA.read('Z'));

        }
        else {
            std::unique_lock<std::mutex> lock(mtx2);
            bufferD.write('X', bufferB.read('X'));
            bufferD.write('Y', bufferB.read('Y'));
            bufferD.write('Z', bufferB.read('Z'));
        }
        ready2 = false;
        ready3 = true;
        sleep(1);
    }
}

void avoid_collisions () {
    for(int i = 1; i < 21; i++) {
        while(!ready3){
            //for synchronization
        }
        if(i%2 == 1) {
            //pulling initial location of values
            std::pair pairX = bufferC.read('X');
            std::pair pairY = bufferC.read('Y');
            std::pair pairZ = bufferC.read('Z');

            //predicting 2 moves ahead
            for(int i = 0; i < 2; i++) {
                pairX.first = (pairX.first + 1) % 8;
                pairX.second = (pairX.second + 1 ) % 7;
                pairY.first = (pairY.first + 1) % 8;
                pairZ.second = (pairZ.second + 1) % 7;
            }

            if(pairX.first == pairY.first && pairX.second == pairY.second) { //if X and Y collide 2 moves ahead
                if(pairX.first == pairZ.first && pairX.second == pairZ.second) { //if X, Y, and Z all collide 2 moves ahead
                    lockX = true;
                    lockY = true;
                    std::cout<<"Collision predicted and avoided between X,Y,and Z at second "<<i+2<<std::endl;
                }
                else{
                    lockX = true;
                    std::cout<<"Collision predicted and avoided between X and Y at second "<<i+2<<std::endl;
                }
            }

            else if(pairX.first == pairZ.first && pairX.second == pairZ.second) { //if X and Z collide 2 moves ahead
                lockY = true;
                std::cout<<"Collision predicted and avoided between X and Z at second "<<i+2<<std::endl;
            }

            else if(pairY.first == pairZ.first && pairY.second == pairZ.second) { //if Y and Z collide 2 moves ahead
                lockZ = true;
                std::cout<<"Collision predicted and avoided between Y and Z at second "<<i+2<<std::endl;
            }
        }
        else {
            //pulling initial location of values
            std::pair pairX = bufferD.read('X');
            std::pair pairY = bufferD.read('Y');
            std::pair pairZ = bufferD.read('Z');

            //predicting 2 moves ahead
            for(int i = 0; i < 2; i++) {
                pairX.first = (pairX.first + 1) % 8;
                pairX.second = (pairX.second + 1 ) % 7;
                pairY.first = (pairY.first + 1) % 8;
                pairZ.second = (pairZ.second + 1) % 7;
            }

            if(pairX.first == pairY.first && pairX.second == pairY.second) { //if X and Y collide 2 moves ahead
                if(pairY.first == pairZ.first && pairY.second == pairZ.second) { //if X, Y, and Z all collide 2 moves ahead
                    lockX = true;
                    lockY = true;
                    std::cout<<"Collision predicted and avoided between X, Y, and Z at second "<<i+2<<std::endl;
                }
                else{
                    lockX = true;
                    std::cout<<"Collision predicted and avoided between X and Y at second "<<i+2<<std::endl;
                }
            }

            else if(pairX.first == pairZ.first && pairX.second == pairZ.second) { //if X and Z collide 2 moves ahead
                lockY = true;
                std::cout<<"Collision predicted and avoided between X and Z at second "<<i+2<<std::endl;
            }

            else if(pairY.first == pairZ.first && pairY.second == pairZ.second) { //if Y and Z collide 2 moves ahead
                lockZ = true;
                std::cout<<"Collision predicted and avoided between Y and Z at second "<<i+2<<std::endl;
            }
        }
        ready3 = false;
        ready1 = true;
        sleep(1);
    }
}

void detect_collisions () {
    sleep(1);
    for (int i = 2; i < 21; i++) {
        while(!ready3){

        }
        collision = false;
        if (i%2 != 1) {
            if (bufferC.read('X') == bufferC.read('Y')) {
                std::cout << "Collision detected with X and Y at second " << i << " located at ("
                          << bufferC.read('X').first << "," << bufferC.read('X').second << ")\n";
                collision = true;
            }

            if (bufferC.read('X') == bufferC.read('Z')) {
                std::cout << "Collision detected with X and Z at second " << i << " located at ("
                          << bufferC.read('X').first << "," << bufferC.read('Z').second << ")\n";
                collision = true;
            }
            if (bufferC.read('Y') == bufferC.read('Z')) {
                std::cout << "Collision detected with Y and Z at second " << i << " located at ("
                          << bufferC.read('Y').first << "," << bufferC.read('Z').second << ")\n";
                collision = true;
            }
        }
        else {
            if (bufferD.read('X') == bufferD.read('Y')) {
                std::cout << "Collision detected with X and Y at second " << i << " located at ("
                          << bufferD.read('X').first << "," << bufferD.read('X').second << ")\n";
                collision = true;
            }

            if (bufferD.read('X') == bufferD.read('Z')) {
                std::cout << "Collision detected with X and Z at second " << i << " located at ("
                          << bufferD.read('X').first << "," << bufferD.read('Z').second << ")\n";
                collision = true;
            }

            if (bufferD.read('Y') == bufferD.read('Z')) {
                std::cout << "Collision detected with Y and Z at second " << i << " located at ("
                          << bufferD.read('Y').first << "," << bufferD.read('Z').second << ")\n";
                collision = true;
            }
        }
        if ( collision == false)
            std::cout<<"No collision detected at second: "<<i<<"\n";
        ready3 = false;
        ready1 = true;
        sleep(1);
    }
}

int main(void) {
    reset_locks();
    reset_waits();
    BufferType1 bufferA;
    BufferType1 bufferB;
    BufferType2 bufferC;
    BufferType2 bufferD;
    std::thread p1 (generate_positions);
    std::thread p2 (record_positions);
    std::thread p3 (avoid_collisions);
    std::thread p4 (detect_collisions);
    p1.join();
    p2.join();
    p3.join();
    p4.join();

    return 0;
}