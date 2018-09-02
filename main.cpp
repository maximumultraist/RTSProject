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
std::mutex mtx1,mtx2,mtx3;
bool collision;
pthread_mutex_t mtxa, mtxb, mtxc, mtxd;
pthread_cond_t genb, gena, reca_c, recb_d, detc, detd;
void freakout(int time) {
    std::cout << "COLLISION DETECT AT SECOND " << time << std::endl;
    bufferC.print();
    bufferD.print();
}


void generate_positions () {
    for (int i = 1; i < 21; i++) {
        if (i%2 == 1) {
            std::lock_guard<std::mutex> lock(mtx1);
            std::pair temp1 = bufferA.read('X');
            std::pair temp2 = bufferA.read('Y');
            std::pair temp3 = bufferA.read('Z');

            bufferB.write('X', (temp1.first + 1) % 8, (temp1.second + 1) % 7);
            bufferB.write('Y', (temp2.first + 1) % 8, 2);
            bufferB.write('Z', 3, (temp3.second + 1) % 7);
        }
        else {
            std::lock_guard<std::mutex> lock(mtx1);
            std::pair temp5 = bufferB.read('X');
            std::pair temp6 = bufferB.read('Y');
            std::pair temp7 = bufferB.read('Z');

            bufferA.write('X', (temp5.first + 1) % 8, (temp5.second + 1) % 7);
            bufferA.write('Y', (temp6.first + 1) % 8, 2);
            bufferA.write('Z', 3, (temp7.second + 1) % 7);
        }
        sleep(1);
    }
}

void record_positions () {
    for (int i = 1; i < 21; i++) {
        if (i%2 == 1) {
            std::lock_guard<std::mutex> lock(mtx2);
            bufferC.write('X', bufferA.read('X'));
            bufferC.write('Y', bufferA.read('Y'));
            bufferC.write('Z', bufferA.read('Z'));

        }
        else {
            std::lock_guard<std::mutex> lock(mtx2);
            bufferD.write('X', bufferB.read('X'));
            bufferD.write('Y', bufferB.read('Y'));
            bufferD.write('Z', bufferB.read('Z'));
        }
        sleep(1);
    }

}

void detect_collisions () {
    sleep(1);
    for (int i = 2; i < 21; i++) {
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
        sleep(1);
    }
}

int main(void) {
    BufferType1 bufferA;
    BufferType1 bufferB;
    BufferType2 bufferC;
    BufferType2 bufferD;
    std::thread p1 (generate_positions);
    std::thread p2 (record_positions);
    std::thread p3 (detect_collisions);
    p1.join();
    p2.join();
    p3.join();

    return 0;
}