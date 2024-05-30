#pragma once
#include <adf.h>
#include "mutual_entropy_kernels.h"
#include "common.h"

using namespace adf;

#if GRAPH == TEST_LOG
class my_graph: public graph
{
private:
	// ------kernel declaration------
	kernel log;

public:
	// ------Input and Output PLIO declaration------
    input_plio test_in;
	output_plio out_1;

	my_graph()
	{
		// ------kernel creation------
	    log = kernel::create(log_kernel_function);

		// ------Input and Output PLIO creation------
		// I argument: a name, that will be used to refer to the port in the block design
		// II argument: the type of the PLIO that will be read/written. Test both plio_32_bits and plio_128_bits to verify the difference
		// III argument: the path to the file that will be read/written for simulation
		test_in = input_plio::create("test_log", plio_32_bits, "data/1k_test_log.txt");
		out_1 = output_plio::create("out_plio_1", plio_32_bits, "data/out_plio_sink_1.txt");

		// ------kernel connection------
        connect<stream>(test_in.out[0], log.in[0]);
		connect<stream>(log.out[0], out_1.in[0]);

		// set kernel source and headers
		source(log)  = "src/entropy_vect.cpp";
		headers(log) = {"src/mutual_entropy_kernels.h","../common/common.h"};

		// set ratio
        runtime<ratio>(log) = 0.9;
	};
};
#endif

#if GRAPH == TEST_ENTROPY
class my_graph: public graph
{
private:
	// ------kernel declaration------
	kernel marginal_entropy;

public:
	// ------Input and Output PLIO declaration------
    input_plio marginal_histo;
	output_plio out_1;

	my_graph()
	{
		// ------kernel creation------
	    marginal_entropy = kernel::create(marginal_entropy_kernel_function);
		// ------Input and Output PLIO creation------
		// I argument: a name, that will be used to refer to the port in the block design
		// II argument: the type of the PLIO that will be read/written. Test both plio_32_bits and plio_128_bits to verify the difference
		// III argument: the path to the file that will be read/written for simulation

		marginal_histo = input_plio::create("marginal_histo", plio_32_bits, "data/marginal.txt");
		out_1 = output_plio::create("out_plio_1", plio_32_bits, "data/out_plio_sink_1.txt");

		// ------kernel connection------
        connect<stream>(marginal_histo.out[0], marginal_entropy.in[0]);
		connect<stream>(marginal_entropy.out[0], out_1.in[0]);

		// set kernel source and headers
		source(marginal_entropy)  = "src/entropy_vect.cpp";
		headers(marginal_entropy) = {"src/mutual_entropy_kernels.h","../common/common.h"};

		// set ratio
        runtime<ratio>(marginal_entropy) = 0.9;
	};

};
#endif

#if GRAPH == ONE_JOINT_ONE_MARGINAL
class my_graph: public graph
{

private:
	// ------kernel declaration------
	kernel joint_entropy;
	kernel marginal_entropy;
    kernel reduce;

public:
	// ------Input and Output PLIO declaration------

	input_plio joint_histo;
    input_plio marginal_histo;
	output_plio out_1;
	// output_plio out_2;

	my_graph()
	{
		// ------kernel creation------
        joint_entropy = kernel::create(entropy_vec_kernel_function);
	    marginal_entropy = kernel::create(marginal_entropy_kernel_function);
        reduce = kernel::create(reduce_kernel_function);
		// ------Input and Output PLIO creation------
		// I argument: a name, that will be used to refer to the port in the block design
		// II argument: the type of the PLIO that will be read/written. Test both plio_32_bits and plio_128_bits to verify the difference
		// III argument: the path to the file that will be read/written for simulation

		joint_histo = input_plio::create("joint_histo", plio_32_bits, "data/joint.txt");
		marginal_histo = input_plio::create("marginal_histo", plio_32_bits, "data/marginal.txt");
		out_1 = output_plio::create("out_plio_1", plio_32_bits, "data/out_plio_sink_1.txt");

		// ------kernel connection------
		connect<stream>(joint_histo.out[0], joint_entropy.in[0]);
		connect<stream>(joint_entropy.out[0], reduce.in[0]);
        connect<stream>(marginal_histo.out[0], marginal_entropy.in[0]);
		connect<stream>(marginal_entropy.out[0], reduce.in[1]);
		connect<stream>(reduce.out[0], out_1.in[0]);

		// set kernel source and headers
		source(joint_entropy)  = "src/entropy_vect.cpp";
		headers(joint_entropy) = {"src/mutual_entropy_kernels.h","../common/common.h"};
		source(marginal_entropy)  = "src/entropy_vect.cpp";
		headers(marginal_entropy) = {"src/mutual_entropy_kernels.h","../common/common.h"};
        source(reduce)  = "src/reduce.cpp";
		headers(reduce) = {"src/mutual_entropy_kernels.h","../common/common.h"};


		// set ratio
		runtime<ratio>(joint_entropy) = 0.9; // 90% of the time the kernel will be executed. This means that 1 AIE will be able to execute just 1 Kernel
        runtime<ratio>(marginal_entropy) = 0.9;
        runtime<ratio>(reduce) = 0.9;
	};

};
#endif

#if GRAPH == FOUR_BY_TWO_SYS_ARR
class my_graph: public graph
{

private:
	// ------kernel declaration------
	kernel marginal_entropy;
	kernel joint_entropy[ENTROPY_KERNELS];
    kernel reduce[REDUCERS];

public:
	// ------Input and Output PLIO declaration------

	input_plio joint_histo[ENTROPY_KERNELS];
    input_plio marginal_histo;
	output_plio out_1;

	my_graph()
	{
        // Create Kernels for the entropy of the joint histogram
        for(int i = 0; i < ENTROPY_KERNELS; i++){
            if(i <= KERNEL_ROWS){
                joint_entropy[i] = kernel::create(entropy_vec_pass_kernel_function); 
            }
            else{
                joint_entropy[i] = kernel::create(entropy_vec_kernel_function); 
            }
            std::ostringstream input_name;
            std::ostringstream file_name;
            input_name << "joint_histo_" << i;
            file_name << "data/joint_histo_" << i << ".txt";
		    joint_histo[i] = input_plio::create(input_name.str(), plio_32_bits, file_name.str());

            source(joint_entropy[i])  = "src/entropy_vect.cpp";
		    headers(joint_entropy[i]) = {"src/mutual_entropy_kernels.h","../common/common.h"};
                    
            connect<stream>(joint_histo[i].out[0], joint_entropy[i].in[0]);
                    
            runtime<ratio>(joint_entropy[i]) = 0.9;
            }

	        marginal_entropy = kernel::create(marginal_entropy_kernel_function);

		    marginal_histo = input_plio::create("marginal_histo", plio_32_bits, "data/marginal.txt");

            source(marginal_entropy)  = "src/entropy_vect.cpp";
		    headers(marginal_entropy) = {"src/mutual_entropy_kernels.h","../common/common.h"};

            connect<stream>(marginal_histo.out[0], marginal_entropy.in[0]);
                
            runtime<ratio>(marginal_entropy) = 0.9;

            for(int i = 0; i < REDUCERS; i++){
                if(i == 0){
                    reduce[i] = kernel::create(reduce_kernel_function);
                }
                else{
                    reduce[i] = kernel::create(reduce_vec_kernel_function);
                }
                source(reduce[i])  = "src/reduce.cpp";
		        headers(reduce[i]) = {"src/mutual_entropy_kernels.h","../common/common.h"};
                runtime<ratio>(reduce[i]) = 0.9;
            }

		out_1 = output_plio::create("out_plio_1", plio_32_bits, "data/out_plio_sink_1.txt");

        // connect marginal entropy
        connect<stream>(marginal_entropy.out[0], joint_entropy[4].in[1]);

        // connect first with second row
        connect<stream>(joint_entropy[4].out[0], joint_entropy[0].in[1]);
        connect<stream>(joint_entropy[5].out[0], joint_entropy[1].in[1]);
        connect<stream>(joint_entropy[6].out[0], joint_entropy[2].in[1]);
        connect<stream>(joint_entropy[7].out[0], joint_entropy[3].in[1]);

        // connect last row with reducer
        connect<stream>(joint_entropy[0].out[0], reduce[1].in[0]);
        connect<stream>(joint_entropy[1].out[0], reduce[1].in[1]);
        connect<stream>(joint_entropy[2].out[0], reduce[2].in[0]);
        connect<stream>(joint_entropy[3].out[0], reduce[2].in[1]);

        // tree-like reducer
		connect<stream>(reduce[2].out[0], reduce[0].in[0]);
        connect<stream>(reduce[1].out[0], reduce[0].in[1]);
		connect<stream>(reduce[0].out[0], out_1.in[0]);
	};

};
#endif

#if GRAPH == FOUR_PYRAMID
class my_graph: public graph
{

private:
	// ------kernel declaration------
	kernel marginal_entropy;
	kernel joint_entropy[ENTROPY_KERNELS];
    kernel reduce[REDUCERS];

public:
	// ------Input and Output PLIO declaration------
	input_plio joint_histo[ENTROPY_KERNELS];
    input_plio marginal_histo;
	output_plio out_1;

	my_graph()
	{
        for(int i = 0; i < ENTROPY_KERNELS; i++){
        joint_entropy[i] = kernel::create(entropy_vec_kernel_function);

        std::ostringstream input_name;
        std::ostringstream file_name;
        input_name << "joint_histo_" << i;
        file_name << "data/joint_histo_" << i << "_of_4.txt";
	    joint_histo[i] = input_plio::create(input_name.str(), plio_32_bits, file_name.str());

        source(joint_entropy[i])  = "src/entropy_vect.cpp";
		headers(joint_entropy[i]) = {"src/mutual_entropy_kernels.h","../common/common.h"};
                    
        connect<stream>(joint_histo[i].out[0], joint_entropy[i].in[0]);
                    
        runtime<ratio>(joint_entropy[i]) = 0.9;
        }

	    marginal_entropy = kernel::create(marginal_entropy_kernel_function);

		marginal_histo = input_plio::create("marginal_histo", plio_32_bits, "data/marginal.txt");

        source(marginal_entropy)  = "src/entropy_vect.cpp";
		headers(marginal_entropy) = {"src/mutual_entropy_kernels.h","../common/common.h"};

        connect<stream>(marginal_histo.out[0], marginal_entropy.in[0]);
                
        runtime<ratio>(marginal_entropy) = 0.9;

        for(int i = 0; i < REDUCERS; i++){
            if(i == 0){
                reduce[i] = kernel::create(reduce_kernel_function);
            }
            else{
                reduce[i] = kernel::create(reduce_vec_kernel_function);
            }
            source(reduce[i])  = "src/reduce.cpp";
		    headers(reduce[i]) = {"src/mutual_entropy_kernels.h","../common/common.h"};
            runtime<ratio>(reduce[i]) = 0.9;
        }

		out_1 = output_plio::create("out_plio_1", plio_32_bits, "data/out_plio_sink_1.txt");

        // connect marginal entropy
        connect<stream>(marginal_entropy.out[0], reduce[0].in[0]);

        // connect last row with reducer
        connect<stream>(joint_entropy[0].out[0], reduce[2].in[0]);
        connect<stream>(joint_entropy[1].out[0], reduce[2].in[1]);
        connect<stream>(joint_entropy[2].out[0], reduce[3].in[0]);
        connect<stream>(joint_entropy[3].out[0], reduce[3].in[1]);

        // tree-like reducer
		connect<stream>(reduce[2].out[0], reduce[1].in[0]);
		connect<stream>(reduce[3].out[0], reduce[1].in[1]);
        connect<stream>(reduce[1].out[0], reduce[0].in[1]);
		connect<stream>(reduce[0].out[0], out_1.in[0]);
	};
};
#endif
