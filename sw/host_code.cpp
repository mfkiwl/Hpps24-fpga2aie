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

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_uuid.h"
#include "../common/common.h"

// For hw emulation, run in sw directory: source ./setup_emu.sh -s on

#define DEVICE_ID 0


// every top function input that must be passed from the host to the kernel must have a unique index starting from 0

// args indexes for setup_joint_aie kernel
#define arg_setup_joint_aie_image_size 0
#define arg_setup_joint_aie_histogram_rows 1

// args indexes for setup_marginal_aie kernel
#define arg_setup_marginal_aie_image_size 0
#define arg_setup_marginal_aie_histogram_rows 1

// args indexes for sink_from_aie kernel
#define arg_sink_from_aie_output 1

bool get_xclbin_path(std::string& xclbin_file);
std::ostream& bold_on(std::ostream& os);
std::ostream& bold_off(std::ostream& os);

double H(int32_t* x, int32_t size, int32_t image_size) {
  double result = 0.0;
  for (int32_t i = 0; i < size; i++) {
    if(x[i] != 0){
        float p = (float) x[i] /image_size;
        result += p * log2f(p);
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
    int32_t image_size = 0;

    int32_t* nums_joint = new int32_t[SYMBOLS * SYMBOLS];
    int32_t* nums_marginal = new int32_t[2 * SYMBOLS];

    for(int j = 1; j <= 11; j++){

		std::ostringstream file_name_marginal;
        std::ostringstream file_name_joint;
        file_name_marginal << "data/marginal_" << j << ".txt";
        file_name_joint << "data/joint_" << j << ".txt";

        std::ifstream marginalFile(file_name_marginal.str());
        // Read data from marginal.txt
        for (int32_t i = 0; i < 2 * SYMBOLS; i++) {
            marginalFile >> nums_marginal[i];
            image_size += nums_marginal[i];
        }
        marginalFile.close();

        std::ifstream jointFile(file_name_joint.str());
        // Read data from joint.txt
        for (int32_t i = 0; i < SYMBOLS * SYMBOLS; i++) {
            jointFile >> nums_joint[i];
        }
        jointFile.close();

    //------------------------------------------------LOADING XCLBIN------------------------------------------    
        std::string xclbin_file;
        if (!get_xclbin_path(xclbin_file))
            return EXIT_FAILURE;

        // Load xclbin
        xrt::device device = xrt::device(DEVICE_ID);
        xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);
    //----------------------------------------------INITIALIZING THE BOARD------------------------------------------

        // create kernel objects
        xrt::kernel krnl_setup_joint_aie  = xrt::kernel(device, xclbin_uuid, "setup_joint_aie");
        xrt::kernel krnl_setup_marginal_aie  = xrt::kernel(device, xclbin_uuid, "setup_marginal_aie");
        xrt::kernel krnl_sink_from_aie  = xrt::kernel(device, xclbin_uuid, "sink_from_aie");

        // get memory bank groups for device buffer - required for axi master input/ouput
        xrtMemoryGroup bank_output  = krnl_sink_from_aie.group_id(arg_sink_from_aie_output);
        xrtMemoryGroup bank_input_0  = krnl_setup_joint_aie.group_id(arg_setup_joint_aie_histogram_rows);
        xrtMemoryGroup bank_input_1  = krnl_setup_marginal_aie.group_id(arg_setup_marginal_aie_histogram_rows);

        // create device buffers - if you have to load some data, here they are
        xrt::bo buffer_setup_joint_aie= xrt::bo(device, SYMBOLS*SYMBOLS * sizeof(int32_t), xrt::bo::flags::normal, bank_input_0); 
        xrt::bo buffer_setup_marginal_aie= xrt::bo(device, SYMBOLS*2 * sizeof(int32_t), xrt::bo::flags::normal, bank_input_1); 
        xrt::bo buffer_sink_from_aie = xrt::bo(device, 4 * sizeof(float), xrt::bo::flags::normal, bank_output); 

        // create runner instances
        xrt::run run_setup_joint_aie   = xrt::run(krnl_setup_joint_aie);
        xrt::run run_setup_marginal_aie   = xrt::run(krnl_setup_marginal_aie);
        xrt::run run_sink_from_aie = xrt::run(krnl_sink_from_aie);

        // set setup_joint_aie kernel arguments
        run_setup_joint_aie.set_arg(arg_setup_joint_aie_image_size,  image_size);
        run_setup_joint_aie.set_arg(arg_setup_joint_aie_histogram_rows, buffer_setup_joint_aie);

        // set setup_marginal_aie kernel arguments
        run_setup_marginal_aie.set_arg(arg_setup_marginal_aie_image_size,  image_size);
        run_setup_marginal_aie.set_arg(arg_setup_marginal_aie_histogram_rows, buffer_setup_marginal_aie);

        // set sink_from_aie kernel arguments
        run_sink_from_aie.set_arg(arg_sink_from_aie_output, buffer_sink_from_aie);

        // write data into the joint input buffer
        buffer_setup_joint_aie.write(nums_joint);
        buffer_setup_joint_aie.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        // write data into the marginal input buffer
        auto start_dt1 = std::chrono::high_resolution_clock::now();

        buffer_setup_marginal_aie.write(nums_marginal);
        buffer_setup_marginal_aie.sync(XCL_BO_SYNC_BO_TO_DEVICE);

        auto end_dt1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration_dt1 = end_dt1 - start_dt1;
        std::cout << "Time for data transfer (Host->PL): " << duration_dt1.count() << " ns" << std::endl;

        // run the kernels
        auto start_aie = std::chrono::high_resolution_clock::now();

        run_setup_joint_aie.start();
        run_setup_marginal_aie.start();
        run_sink_from_aie.start();

        // wait for the kernels to finish
        run_setup_joint_aie.wait();
        run_setup_marginal_aie.wait();
        run_sink_from_aie.wait();

        auto end_aie = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration_aie = end_aie - start_aie;
        std::cout << "Time for execution (AIE): " << duration_aie.count() << " ns" << std::endl;

        //------------------------------------------------GROUND TRUTH------------------------------------------  
        auto start_cpu = std::chrono::high_resolution_clock::now();
        float mi = H(nums_joint,SYMBOLS*SYMBOLS, image_size) - H(nums_marginal,SYMBOLS*2, image_size);
        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration_cpu = end_cpu - start_cpu;

        std::cout << "Time for execution (CPU): " << duration_cpu.count() << " ns" << std::endl;

        // read the output buffer
        auto start_dt2 = std::chrono::high_resolution_clock::now();

        buffer_sink_from_aie.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        float output_buffer[4];
        buffer_sink_from_aie.read(output_buffer);

        auto end_dt2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> duration_dt2 = end_dt2 - start_dt2;

        std::cout << "Time for data transfer (PL->Host): " << duration_dt2.count() << " ns" << std::endl;

        std::cout << "Speedup CPU/AIE (\\wo data transf): " << duration_cpu.count()/duration_aie.count() << std::endl;
        std::cout << "Speedup CPU/AIE (\\w data transf): " << duration_cpu.count()/(duration_aie.count()+duration_dt1.count()+duration_dt2.count()) << std::endl;

        // ---------------------------------CONFRONTO PER VERIFICARE L'ERRORE--------------------------------------
            
        // Here there should be a code for checking correctness of your application, like a software application

        std::cout << "CPU result: " << std::endl;
        std::cout << mi << std::endl;
        std::cout << "Output_buffer:" << std::endl;
        std::cout << output_buffer[0] << std::endl;
        std::cout << "Diff:" << std::endl;
        std::cout << output_buffer[0] - mi << std::endl << std::endl;
    }

    delete[] nums_joint;
    delete[] nums_marginal;

    return EXIT_SUCCESS;
}


bool get_xclbin_path(std::string& xclbin_file) {
    // Judge emulation mode accoring to env variable
    char *env_emu;
    if (env_emu = getenv("XCL_EMULATION_MODE")) {
        std::string mode(env_emu);
        if (mode == "hw_emu")
        {
            std::cout << "Program running in hardware emulation mode" << std::endl;
            xclbin_file = "overlay_hw_emu.xclbin";
        }
        else
        {
            std::cout << "[ERROR] Unsupported Emulation Mode: " << mode << std::endl;
            return false;
        }
    }
    else {
        std::cout << bold_on << "Program running in hardware mode" << bold_off << std::endl;
        xclbin_file = "overlay_hw.xclbin";
    }

    std::cout << std::endl << std::endl;
    return true;
}

std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}