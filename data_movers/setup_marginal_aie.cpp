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

void setup_marginal_aie(
	int32_t image_size, 
	int32_t* histogram_rows, 
	hls::stream<ap_int<sizeof(int32_t) * 8 * 8>>& s) {

	#pragma HLS interface m_axi port=histogram_rows depth=100000 offset=slave bundle=gmem0
	#pragma HLS interface axis port=s
	#pragma HLS interface s_axilite port=histogram_rows bundle=control
	#pragma HLS interface s_axilite port=image_size bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	ap_int<sizeof(int32_t)*8*8> input;
	input.range(31,0) = image_size;
	input.range(63,32) = 0;
	input.range(95,64) = 0;
	input.range(127,96) = 0;
	input.range(159,128) = 0;
	input.range(191,160) = 0;
	input.range(223,192) = 0;
	input.range(255,224) = 0;

	s.write(input);

	for (int32_t j = 0; j < LOOPS_M; j++) {
		input.range(31,0) = histogram_rows[j*8+0];
		input.range(63,32) = histogram_rows[j*8+1];
		input.range(95,64) = histogram_rows[j*8+2];
		input.range(127,96) = histogram_rows[j*8+3];
		input.range(159,128) = histogram_rows[j*8+4];
		input.range(191,160) = histogram_rows[j*8+5];
		input.range(223,192) = histogram_rows[j*8+6];
		input.range(255,224) = histogram_rows[j*8+7];
		s.write(input);
	}
}
}
