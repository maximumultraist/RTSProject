#include <iostream>
#include <unistd.h>
#include <cmath>
#include <numeric>
#include <vector>
#include <pthread.h>
#include "BufferType1.h"
#include "BufferType2.h"

/* void testBuffers() //for testing buffers and their functions
{
    //testing buffertype 1 and making sure both functions work
    BufferType1 bufferA; // creating buffer A

    //testing read function
    std::pair<int,int> pair1, pair2, pair3; //pairs to store value locations
    pair1 = bufferA.read('X'); //pulling location of X
    pair2 = bufferA.read('Y'); //pulling location of Y
    pair3 = bufferA.read('Z'); //pulling location of Z
    std::cout << "buffer A location of X: " << pair1.first <<"," << pair1.second << "\n"; //printing location of X
    std::cout << "buffer A location of Y: " << pair2.first <<"," << pair2.second << "\n"; //printing location of Y
    std::cout << "buffer A location of Z: " << pair3.first <<"," << pair3.second << "\n"; //printing location of Z

    //testing write function
    std::pair<int,int> pair4 (5,3); //pair to store new desired location of value at 5,3
    bufferA.write('X',pair4); //moving X from 0,0 to 5,3
    std::pair<int,int> pair5(bufferA.read('X')); //pair to pull X value from buffer A to check if it moved
    std::cout<< "buffer A new location of X: " << pair5.first << "," << pair5.second << "\n"; //printing new location of X
    bufferA.print(); //testing BufferType1 print functionality


    //testing BufferType2 and making sure both functions work
    BufferType2 bufferC; //creating buffer C
    bufferC.print(); //testing print on newly created blank buffer

    bufferC.write('X',std::make_pair(4,5)); //inserting new X coordinate
    bufferC.print(); //testing print to show write was successful

    std::pair<int,int> pair7 (bufferC.read('X')); //testing bufferType2 read function on X
    std::cout<< "X located at :" << pair7.first << "," << pair7.second << "\n"; //printing just read points
} */
BufferType1 bufferA;
BufferType1 bufferB;
BufferType2 bufferC;
BufferType2 bufferD;
pthread_mutex_t mtxa, mtxb, mtxc, mtxd;
pthread_cond_t genb, gena, reca_c, recb_d, detc, detd;
void freakout() {
    std::cout << "COLLISION DETECT!!!11one" << std::endl;
}


void generate_positions () {
    for (int i = 1; i < 21; i++) {
        if (i%2 == 1) {
            //std::unique_lock<std::mutex> lk1(mtxa);
            //gena.wait(lk1, []{return run_gena;});
            std::cout << "Generating Buffer B..." << std::endl;
            std::pair temp1 = bufferA.read('X');
            std::pair temp2 = bufferA.read('Y');
            std::pair temp3 = bufferA.read('Z');

            bufferB.write('X', (temp1.first + 1) % 8, (temp1.second + 1) % 7);
            bufferB.write('Y', (temp2.first + 1) % 8, 2);
            bufferB.write('Z', 3, (temp3.second + 1) % 7);
            bufferB.print();
            std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++\n" << std::endl;
        }
        else {
            //std::unique_lock<std::mutex> lk2(mtxb);
            std::cout << "Generating Buffer A..." << std::endl;
            std::pair temp5 = bufferB.read('X');
            std::pair temp6 = bufferB.read('Y');
            std::pair temp7 = bufferB.read('Z');

            bufferA.write('X', (temp5.first + 1) % 8, (temp5.second + 1) % 7);
            bufferA.write('Y', (temp6.first + 1) % 8, 2);
            bufferA.write('Z', 3, (temp7.second + 1) % 7);
            bufferA.print();
            std::cout << "---------------------------------------------------\n" << std::endl;
        }
    }
}

void record_positions () {
    for (int i = 1; i < 21; i++) {
        //add locking logic here, pt 1
        bufferC.write('X', bufferA.read('X'));
        bufferC.write('Y', bufferA.read('Y'));
        bufferC.write('Z', bufferA.read('Z'));
        bufferC.print();
        std::cout << "*********************************************************\n" << std::endl;

        //add locking logic here, pt 2
        bufferD.write('X', bufferB.read('X'));
        bufferD.write('Y', bufferB.read('Y'));
        bufferD.write('Z', bufferB.read('Z'));
        bufferD.print();
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" << std::endl;
    }

}

void detect_collisions () {
    for (int i = 1; i < 21; i++) {

        // locks again pt 4.
        if (bufferC.read('X') == bufferC.read('Y'))
            freakout();
        if (bufferC.read('X') == bufferC.read('Z'))
            freakout();
        if (bufferC.read('Y') == bufferC.read('Z'))
            freakout();

        // locks again pt. 5
        if (bufferD.read('X') == bufferD.read('Y'))
            freakout();
        if (bufferD.read('X') == bufferD.read('Z'))
            freakout();
        if (bufferD.read('Y') == bufferD.read('Z'))
            freakout();
    }
}

int main(void) {
    BufferType1 bufferA;
    BufferType1 bufferB;
    BufferType2 bufferC;
    BufferType2 bufferD;
    generate_positions();
    std::cout << "\n AND NOW, FOR THE RECORDS:\n" << std::endl;
    record_positions();
    detect_collisions();

    return 0;
}