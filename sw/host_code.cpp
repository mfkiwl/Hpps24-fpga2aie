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
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
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

int checkResult(float output, float true_result) {
    float error = 1e-2;
    if (output >= true_result+error || output <= true_result-error) {
        printf("Error: %f != %f {output, true_result}", output, true_result);
        return EXIT_FAILURE;
    }

    std::cout << "Test passed!" << std::endl;
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    int32_t image_size = 1024000;

    int32_t nums_joint[SYMBOLS*SYMBOLS];
    for (int32_t i=0; i<SYMBOLS*SYMBOLS; i++) 
        nums_joint[i] = i;

    int32_t nums_marginal[2*SYMBOLS];
    for (int32_t i=0; i<2*SYMBOLS; i++) 
        nums_marginal[i] = i;

//------------------------------------------------LOADING XCLBIN------------------------------------------    
    std::string xclbin_file;
    if (!get_xclbin_path(xclbin_file))
        return EXIT_FAILURE;

    // Load xclbin
    std::cout << "1. Loading bitstream (" << xclbin_file << ")... ";
    xrt::device device = xrt::device(DEVICE_ID);
    xrt::uuid xclbin_uuid = device.load_xclbin(xclbin_file);
    std::cout << "Done" << std::endl;
//----------------------------------------------INITIALIZING THE BOARD------------------------------------------

    // create kernel objects
    std::cout << "Creating kernel objects..." << std::endl;
    xrt::kernel krnl_setup_joint_aie  = xrt::kernel(device, xclbin_uuid, "setup_joint_aie");
    xrt::kernel krnl_setup_marginal_aie  = xrt::kernel(device, xclbin_uuid, "setup_marginal_aie");
    xrt::kernel krnl_sink_from_aie  = xrt::kernel(device, xclbin_uuid, "sink_from_aie");
    std::cout << "Done" << std::endl;

    std::cout << "Getting memory bank groups..." << std::endl;
    // get memory bank groups for device buffer - required for axi master input/ouput
    xrtMemoryGroup bank_output  = krnl_sink_from_aie.group_id(arg_sink_from_aie_output);
    xrtMemoryGroup bank_input_0  = krnl_setup_joint_aie.group_id(arg_setup_joint_aie_histogram_rows);
    xrtMemoryGroup bank_input_1  = krnl_setup_marginal_aie.group_id(arg_setup_marginal_aie_histogram_rows);
    std::cout << "Done" << std::endl;

    std::cout << "Creating device buffers..." << std::endl;
    // create device buffers - if you have to load some data, here they are
    xrt::bo buffer_setup_joint_aie= xrt::bo(device, SYMBOLS*SYMBOLS * sizeof(int32_t), xrt::bo::flags::normal, bank_input_0); 
    xrt::bo buffer_setup_marginal_aie= xrt::bo(device, SYMBOLS*2 * sizeof(int32_t), xrt::bo::flags::normal, bank_input_1); 
    xrt::bo buffer_sink_from_aie = xrt::bo(device, 8 * sizeof(float), xrt::bo::flags::normal, bank_output); 
    std::cout << "Done" << std::endl;

    // create runner instances
    std::cout << "Creating runner buffers..." << std::endl;
    xrt::run run_setup_joint_aie   = xrt::run(krnl_setup_joint_aie);
    xrt::run run_setup_marginal_aie   = xrt::run(krnl_setup_marginal_aie);
    xrt::run run_sink_from_aie = xrt::run(krnl_sink_from_aie);
    std::cout << "Done" << std::endl;

    // set setup_joint_aie kernel arguments
    std::cout << "Setting setup_joint_aie kernel arguments..." << std::endl;
    run_setup_joint_aie.set_arg(arg_setup_joint_aie_image_size,  image_size);
    run_setup_joint_aie.set_arg(arg_setup_joint_aie_histogram_rows, buffer_setup_joint_aie);
    std::cout << "Done" << std::endl;

    // set setup_marginal_aie kernel arguments
    std::cout << "Setting setup_marginal_aie kernel arguments..." << std::endl;
    run_setup_marginal_aie.set_arg(arg_setup_marginal_aie_image_size,  image_size);
    run_setup_marginal_aie.set_arg(arg_setup_marginal_aie_histogram_rows, buffer_setup_marginal_aie);
    std::cout << "Done" << std::endl;

    // set sink_from_aie kernel arguments
    std::cout << "Setting sink_from_aie kernel arguments..." << std::endl;
    run_sink_from_aie.set_arg(arg_sink_from_aie_output, buffer_sink_from_aie);
    std::cout << "Done" << std::endl;

    // write data into the joint input buffer
    std::cout << "Writing data into the joint input buffer..." << std::endl;
    buffer_setup_joint_aie.write(nums_joint);
    buffer_setup_joint_aie.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "Done" << std::endl;

    // write data into the marginal input buffer
    std::cout << "Writing data into the marginal input buffer..." << std::endl;
    buffer_setup_marginal_aie.write(nums_marginal);
    buffer_setup_marginal_aie.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    std::cout << "Done" << std::endl;

    // run the kernels
    std::cout << "Running the kernels..." << std::endl;
    run_setup_joint_aie.start();
    run_setup_marginal_aie.start();
    run_sink_from_aie.start();
    std::cout << "Done" << std::endl;

    // wait for the kernels to finish
    std::cout << "Waiting for the setup_joint_aie kernel to finish..." << std::endl;
    run_setup_joint_aie.wait();
    std::cout << "Waiting for the setup_marginal_aie kernel to finish..." << std::endl;
    run_setup_marginal_aie.wait();
    std::cout << "Waiting for the sink_from_aie kernel to finish..." << std::endl;
    run_sink_from_aie.wait();
    std::cout << "Done" << std::endl;

    // read the output buffer
    std::cout << "Reading the output buffer..." << std::endl;
    buffer_sink_from_aie.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    float output_buffer[8];
    buffer_sink_from_aie.read(output_buffer);
    std::cout << "Done" << std::endl;

    // ---------------------------------CONFRONTO PER VERIFICARE L'ERRORE--------------------------------------
        
    // Here there should be a code for checking correctness of your application, like a software application
    float true_result = -9828.007722251097;

    float tot = 0.0;

    for(int i = 0; i < 8; i++){
        tot += output_buffer[i];
    }

    std::cout << "Sum of Output_buffer:" << std::endl;
    std::cout << tot << std::endl;
    std::cout << "True_result:" << std::endl;
    std::cout << true_result << std::endl;

    checkResult(tot, true_result);

    return 0;
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

