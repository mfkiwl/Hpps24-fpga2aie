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
#include <ap_int.h>
#include <hls_stream.h>
#include <hls_math.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"


extern "C" {

void setup_joint_aie(
	int32_t image_size, 
	int32_t* histogram_rows, 
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_1,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_2,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_3,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_4,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_5,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_6,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_7,
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s_8
	) {

	// ap_int<sizeof(int32_t) * 8 * 8>> histogram_rows 

	#pragma HLS interface m_axi port=histogram_rows depth=100000 offset=slave bundle=gmem1
	#pragma HLS interface axis port=s_1
	#pragma HLS interface axis port=s_2
	#pragma HLS interface axis port=s_3
	#pragma HLS interface axis port=s_4
	#pragma HLS interface axis port=s_5
	#pragma HLS interface axis port=s_6
	#pragma HLS interface axis port=s_7
	#pragma HLS interface axis port=s_8
	#pragma HLS interface s_axilite port=histogram_rows bundle=control
	#pragma HLS interface s_axilite port=image_size bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	ap_int<sizeof(int32_t)*8*8> input_1;
	input_1.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_2;
	input_2.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_3;
	input_3.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_4;
	input_4.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_5;
	input_5.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_6;
	input_6.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_7;
	input_7.range(31,0) = image_size;

	ap_int<sizeof(int32_t)*8*8> input_8;
	input_8.range(31,0) = image_size;

	s_1.write(input_1);
	s_2.write(input_2);
	s_3.write(input_3);
	s_4.write(input_4);
	s_5.write(input_5);
	s_6.write(input_6);
	s_7.write(input_7);
	s_8.write(input_8);

	for (int32_t j = 0; j < LOOPS_J; j++) {
		// no 8 input_1.range, just s.write(input_1)
		input_1.range(31,0) = histogram_rows[j*8+0];
		input_1.range(63,32) = histogram_rows[j*8+1];
		input_1.range(95,64) = histogram_rows[j*8+2];
		input_1.range(127,96) = histogram_rows[j*8+3];
		input_1.range(159,128) = histogram_rows[j*8+4];
		input_1.range(191,160) = histogram_rows[j*8+5];
		input_1.range(223,192) = histogram_rows[j*8+6];
		input_1.range(255,224) = histogram_rows[j*8+7];

		input_2.range(31,0) = histogram_rows[j*8+0+LOOPS_J];
		input_2.range(63,32) = histogram_rows[j*8+1+LOOPS_J];
		input_2.range(95,64) = histogram_rows[j*8+2+LOOPS_J];
		input_2.range(127,96) = histogram_rows[j*8+3+LOOPS_J];
		input_2.range(159,128) = histogram_rows[j*8+4+LOOPS_J];
		input_2.range(191,160) = histogram_rows[j*8+5+LOOPS_J];
		input_2.range(223,192) = histogram_rows[j*8+6+LOOPS_J];
		input_2.range(255,224) = histogram_rows[j*8+7+LOOPS_J];

		input_3.range(31,0) = histogram_rows[j*8+0+LOOPS_J*2];
		input_3.range(63,32) = histogram_rows[j*8+1+LOOPS_J*2];
		input_3.range(95,64) = histogram_rows[j*8+2+LOOPS_J*2];
		input_3.range(127,96) = histogram_rows[j*8+3+LOOPS_J*2];
		input_3.range(159,128) = histogram_rows[j*8+4+LOOPS_J*2];
		input_3.range(191,160) = histogram_rows[j*8+5+LOOPS_J*2];
		input_3.range(223,192) = histogram_rows[j*8+6+LOOPS_J*2];
		input_3.range(255,224) = histogram_rows[j*8+7+LOOPS_J*2];

		input_4.range(31,0) = histogram_rows[j*8+0+LOOPS_J*3];
		input_4.range(63,32) = histogram_rows[j*8+1+LOOPS_J*3];
		input_4.range(95,64) = histogram_rows[j*8+2+LOOPS_J*3];
		input_4.range(127,96) = histogram_rows[j*8+3+LOOPS_J*3];
		input_4.range(159,128) = histogram_rows[j*8+4+LOOPS_J*3];
		input_4.range(191,160) = histogram_rows[j*8+5+LOOPS_J*3];
		input_4.range(223,192) = histogram_rows[j*8+6+LOOPS_J*3];
		input_4.range(255,224) = histogram_rows[j*8+7+LOOPS_J*3];

		input_5.range(31,0) = histogram_rows[j*8+0+LOOPS_J*4];
		input_5.range(63,32) = histogram_rows[j*8+1+LOOPS_J*4];
		input_5.range(95,64) = histogram_rows[j*8+2+LOOPS_J*4];
		input_5.range(127,96) = histogram_rows[j*8+3+LOOPS_J*4];
		input_5.range(159,128) = histogram_rows[j*8+4+LOOPS_J*4];
		input_5.range(191,160) = histogram_rows[j*8+5+LOOPS_J*4];
		input_5.range(223,192) = histogram_rows[j*8+6+LOOPS_J*4];
		input_5.range(255,224) = histogram_rows[j*8+7+LOOPS_J*4];

		input_6.range(31,0) = histogram_rows[j*8+0+LOOPS_J*5];
		input_6.range(63,32) = histogram_rows[j*8+1+LOOPS_J*5];
		input_6.range(95,64) = histogram_rows[j*8+2+LOOPS_J*5];
		input_6.range(127,96) = histogram_rows[j*8+3+LOOPS_J*5];
		input_6.range(159,128) = histogram_rows[j*8+4+LOOPS_J*5];
		input_6.range(191,160) = histogram_rows[j*8+5+LOOPS_J*5];
		input_6.range(223,192) = histogram_rows[j*8+6+LOOPS_J*5];
		input_6.range(255,224) = histogram_rows[j*8+7+LOOPS_J*5];

		input_7.range(31,0) = histogram_rows[j*8+0+LOOPS_J*6];
		input_7.range(63,32) = histogram_rows[j*8+1+LOOPS_J*6];
		input_7.range(95,64) = histogram_rows[j*8+2+LOOPS_J*6];
		input_7.range(127,96) = histogram_rows[j*8+3+LOOPS_J*6];
		input_7.range(159,128) = histogram_rows[j*8+4+LOOPS_J*6];
		input_7.range(191,160) = histogram_rows[j*8+5+LOOPS_J*6];
		input_7.range(223,192) = histogram_rows[j*8+6+LOOPS_J*6];
		input_7.range(255,224) = histogram_rows[j*8+7+LOOPS_J*6];

		input_8.range(31,0) = histogram_rows[j*8+0+LOOPS_J*7];
		input_8.range(63,32) = histogram_rows[j*8+1+LOOPS_J*7];
		input_8.range(95,64) = histogram_rows[j*8+2+LOOPS_J*7];
		input_8.range(127,96) = histogram_rows[j*8+3+LOOPS_J*7];
		input_8.range(159,128) = histogram_rows[j*8+4+LOOPS_J*7];
		input_8.range(191,160) = histogram_rows[j*8+5+LOOPS_J*7];
		input_8.range(223,192) = histogram_rows[j*8+6+LOOPS_J*7];
		input_8.range(255,224) = histogram_rows[j*8+7+LOOPS_J*7];

		s_1.write(input_1);
		s_2.write(input_2);
		s_3.write(input_3);
		s_4.write(input_4);
		s_5.write(input_5);
		s_6.write(input_6);
		s_7.write(input_7);
		s_8.write(input_8);
	}
}
}
