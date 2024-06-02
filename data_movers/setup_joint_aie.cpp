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

void setup_joint_aie(
	int image_size, 
	int* histogram_rows, 
	hls::stream<int>& s_0, 
	hls::stream<int>& s_1, 
	hls::stream<int>& s_2, 
	hls::stream<int>& s_3, 
	hls::stream<int>& s_4,
	hls::stream<int>& s_5,
	hls::stream<int>& s_6,
	hls::stream<int>& s_7) {

	#pragma HLS interface m_axi port=histogram_rows depth=100 offset=slave bundle=gmem0
	#pragma HLS interface axis port=s_0
	#pragma HLS interface axis port=s_1
	#pragma HLS interface axis port=s_2
	#pragma HLS interface axis port=s_3
	#pragma HLS interface axis port=s_4
	#pragma HLS interface axis port=s_5
	#pragma HLS interface axis port=s_6
	#pragma HLS interface axis port=s_7
	#pragma HLS interface s_axilite port=histogram_rows bundle=control
	#pragma HLS interface s_axilite port=image_size bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	s_0.write(image_size);
	s_1.write(image_size);
	s_2.write(image_size);
	s_3.write(image_size);
	s_4.write(image_size);
	s_5.write(image_size);
	s_6.write(image_size);
	s_7.write(image_size);

	for (int j = 0; j < LOOPS_J; j++) {

		s_0.write(histogram_rows[j*8+0]);
		s_0.write(histogram_rows[j*8+1]);
		s_0.write(histogram_rows[j*8+2]);
		s_0.write(histogram_rows[j*8+3]);
		s_0.write(histogram_rows[j*8+4]);
		s_0.write(histogram_rows[j*8+5]);
		s_0.write(histogram_rows[j*8+6]);
		s_0.write(histogram_rows[j*8+7]);
		
		s_1.write(histogram_rows[j*8+0+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+1+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+2+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+3+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+4+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+5+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+6+LOOPS_J*8]);
		s_1.write(histogram_rows[j*8+7+LOOPS_J*8]);

		s_2.write(histogram_rows[j*8+0+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+1+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+2+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+3+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+4+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+5+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+6+LOOPS_J*8*2]);
		s_2.write(histogram_rows[j*8+7+LOOPS_J*8*2]);

		s_3.write(histogram_rows[j*8+0+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+1+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+2+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+3+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+4+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+5+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+6+LOOPS_J*8*3]);
		s_3.write(histogram_rows[j*8+7+LOOPS_J*8*3]);

		s_4.write(histogram_rows[j*8+0+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+1+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+2+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+3+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+4+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+5+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+6+LOOPS_J*8*4]);
		s_4.write(histogram_rows[j*8+7+LOOPS_J*8*4]);

		s_5.write(histogram_rows[j*8+0+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+1+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+2+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+3+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+4+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+5+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+6+LOOPS_J*8*5]);
		s_5.write(histogram_rows[j*8+7+LOOPS_J*8*5]);

		s_6.write(histogram_rows[j*8+0+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+1+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+2+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+3+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+4+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+5+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+6+LOOPS_J*8*6]);
		s_6.write(histogram_rows[j*8+7+LOOPS_J*8*6]);

		s_7.write(histogram_rows[j*8+0+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+1+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+2+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+3+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+4+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+5+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+6+LOOPS_J*8*7]);
		s_7.write(histogram_rows[j*8+7+LOOPS_J*8*7]);
	}
}
}
