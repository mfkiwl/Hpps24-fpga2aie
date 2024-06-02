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


#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../common/common.h"


extern "C" {

void setup_marginal_aie(int image_size, int* histogram_rows, hls::stream<int>& s) {

	#pragma HLS interface m_axi port=histogram_rows depth=100 offset=slave bundle=gmem0
	#pragma HLS interface axis port=s
	#pragma HLS interface s_axilite port=histogram_rows bundle=control
	#pragma HLS interface s_axilite port=image_size bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	s.write(image_size);
	for (int j = 0; j < LOOPS_M; j++) {
		s.write(histogram_rows[j*8+0]);
		s.write(histogram_rows[j*8+1]);
		s.write(histogram_rows[j*8+2]);
		s.write(histogram_rows[j*8+3]);
		s.write(histogram_rows[j*8+4]);
		s.write(histogram_rows[j*8+5]);
		s.write(histogram_rows[j*8+6]);
		s.write(histogram_rows[j*8+7]);
	}
}
}
