/*
MIT License

Copyright (c) 2023 Paolo Salvatore Galfano, Giuseppe Sorrentino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <ap_axi_sdata.h>
#include <cmath>
#include "../setup_joint_aie.cpp"
#include <iostream>
#include "../common/common.h"

void read_from_stream(float *buffer, hls::stream<float> &stream, size_t size) {
    for (unsigned int i = 0; i < size; i++) {
        buffer[i] = stream.read();
    }
}

int main(int argc, char* argv[]) {
    // In a testbench, you will use you kernel as a C function
    // You will need to create the input and output of your function
    hls::stream<int> s_0;
    hls::stream<int> s_1;
    hls::stream<int> s_2;
    hls::stream<int> s_3;
    hls::stream<int> s_4;
    hls::stream<int> s_5;
    hls::stream<int> s_6;
    hls::stream<int> s_7;
    int image_size = 1024000;
    int *histogram_rows = new int[LOOPS_J*8*ENTROPY_KERNELS];
    for (unsigned int i = 0; i < (int)LOOPS_J*8*ENTROPY_KERNELS; i++) {
        histogram_rows[i] = i;
    }
    setup_joint_aie(image_size, histogram_rows, s_0, s_1, s_2, s_3, s_4, s_5, s_6, s_7);

    // If the function worked I can print values in the stream and check them
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_0.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_1.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_2.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_3.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_4.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_5.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_6.read() << std::endl;
    }
    for(unsigned int i = 0; i < (int) LOOPS_J*8 +1 ; i++) {
        std::cout << s_7.read() << std::endl;
    }
    // Here you will se a warning: THIS IS THE MOST IMPORTANT PART OF THE TESTBENCH

    // Indeed, in testbench you can check if you stream and loop are correctly sized. 
    // Warning may be caused by your testbench, as now, or by wrongly sized loops in your code.

    // try to change the for-loop size, from "size" to "size+1" and see that the output is correct...
    // why we have to add +1?

    // And now? Since you want to effectively test your AIE...this code may practically write the AIE input
    // write into data 
    
    std::ofstream file_1;
    file_1.open("../../aie/data/joint_test_0.txt");
    if (file_1.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_1 << image_size << std::endl;
        for (unsigned int i = 0; i < (int) LOOPS_J*8; i++) {
            file_1 << histogram_rows[i] << std::endl;
        }
        file_1.close();
    } else {
        std::cout << "Error opening file_1" << std::endl;
    }

    std::ofstream file_2;
    file_2.open("../../aie/data/joint_test_1.txt");
    if (file_2.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_2 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8; i < (int) LOOPS_J*8*2; i++) {
            file_2 << histogram_rows[i] << std::endl;
        }
        file_2.close();
    } else {
        std::cout << "Error opening file_2" << std::endl;
    }

    std::ofstream file_3;
    file_3.open("../../aie/data/joint_test_2.txt");
    if (file_3.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_3 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*2; i < (int) LOOPS_J*8*3; i++) {
            file_3 << histogram_rows[i] << std::endl;
        }
        file_3.close();
    } else {
        std::cout << "Error opening file_3" << std::endl;
    }

    std::ofstream file_4;
    file_4.open("../../aie/data/joint_test_3.txt");
    if (file_4.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_4 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*3; i < (int) LOOPS_J*8*4; i++) {
            file_4 << histogram_rows[i] << std::endl;
        }
        file_4.close();
    } else {
        std::cout << "Error opening file_4" << std::endl;
    }

    std::ofstream file_5;
    file_5.open("../../aie/data/joint_test_4.txt");
    if (file_5.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_5 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*4; i < (int) LOOPS_J*8*5; i++) {
            file_5 << histogram_rows[i] << std::endl;
        }
        file_5.close();
    } else {
        std::cout << "Error opening file_5" << std::endl;
    }

    std::ofstream file_6;
    file_6.open("../../aie/data/joint_test_5.txt");
    if (file_6.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_6 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*5; i < (int) LOOPS_J*8*6; i++) {
            file_6 << histogram_rows[i] << std::endl;
        }
        file_6.close();
    } else {
        std::cout << "Error opening file_6" << std::endl;
    }

    std::ofstream file_7;
    file_7.open("../../aie/data/joint_test_6.txt");
    if (file_7.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_7 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*6; i < (int) LOOPS_J*8*7; i++) {
            file_7 << histogram_rows[i] << std::endl;
        }
        file_7.close();
    } else {
        std::cout << "Error opening file_7" << std::endl;
    }

    std::ofstream file_8;
    file_8.open("../../aie/data/joint_test_7.txt");
    if (file_8.is_open()) {
        //firstly I write the size, which will be, according to setup_aie, size/4
        file_8 << image_size << std::endl;
        for (unsigned int i = (int) LOOPS_J*8*7; i < (int) LOOPS_J*8*8; i++) {
            file_8 << histogram_rows[i] << std::endl;
        }
        file_8.close();
    } else {
        std::cout << "Error opening file_8" << std::endl;
    }



    // In a different, complete, test, here you may even run the AIE and then continue your test. But for this
    // modular test...it's enough to check the stream and the file :=).
    return 0;
}
