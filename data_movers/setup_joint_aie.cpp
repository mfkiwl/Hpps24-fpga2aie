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
	ap_int<256>* histogram_rows, 
	hls::stream<ap_int<256>>& s_1,
	hls::stream<ap_int<256>>& s_2,
	hls::stream<ap_int<256>>& s_3,
	hls::stream<ap_int<256>>& s_4,
	hls::stream<ap_int<256>>& s_5,
	hls::stream<ap_int<256>>& s_6,
	hls::stream<ap_int<256>>& s_7,
	hls::stream<ap_int<256>>& s_8,
	hls::stream<ap_int<256>>& s_9,
	hls::stream<ap_int<256>>& s_10,
	hls::stream<ap_int<256>>& s_11,
	hls::stream<ap_int<256>>& s_12,
	hls::stream<ap_int<256>>& s_13,
	hls::stream<ap_int<256>>& s_14,
	hls::stream<ap_int<256>>& s_15,
	hls::stream<ap_int<256>>& s_16,
	hls::stream<ap_int<256>>& s_17,
	hls::stream<ap_int<256>>& s_18,
	hls::stream<ap_int<256>>& s_19,
	hls::stream<ap_int<256>>& s_20,
	hls::stream<ap_int<256>>& s_21,
	hls::stream<ap_int<256>>& s_22,
	hls::stream<ap_int<256>>& s_23,
	hls::stream<ap_int<256>>& s_24,
	hls::stream<ap_int<256>>& s_25,
	hls::stream<ap_int<256>>& s_26,
	hls::stream<ap_int<256>>& s_27,
	hls::stream<ap_int<256>>& s_28,
	hls::stream<ap_int<256>>& s_29,
	hls::stream<ap_int<256>>& s_30,
	hls::stream<ap_int<256>>& s_31,
	hls::stream<ap_int<256>>& s_32,
	hls::stream<ap_int<256>>& s_33,
	hls::stream<ap_int<256>>& s_34,
	hls::stream<ap_int<256>>& s_35,
	hls::stream<ap_int<256>>& s_36,
	hls::stream<ap_int<256>>& s_37,
	hls::stream<ap_int<256>>& s_38,
	hls::stream<ap_int<256>>& s_39,
	hls::stream<ap_int<256>>& s_40,
	hls::stream<ap_int<256>>& s_41,
	hls::stream<ap_int<256>>& s_42,
	hls::stream<ap_int<256>>& s_43,
	hls::stream<ap_int<256>>& s_44,
	hls::stream<ap_int<256>>& s_45,
	hls::stream<ap_int<256>>& s_46,
	hls::stream<ap_int<256>>& s_47,
	hls::stream<ap_int<256>>& s_48,
	hls::stream<ap_int<256>>& s_49,
	hls::stream<ap_int<256>>& s_50,
	hls::stream<ap_int<256>>& s_51,
	hls::stream<ap_int<256>>& s_52,
	hls::stream<ap_int<256>>& s_53,
	hls::stream<ap_int<256>>& s_54,
	hls::stream<ap_int<256>>& s_55,
	hls::stream<ap_int<256>>& s_56,
	hls::stream<ap_int<256>>& s_57,
	hls::stream<ap_int<256>>& s_58,
	hls::stream<ap_int<256>>& s_59,
	hls::stream<ap_int<256>>& s_60,
	hls::stream<ap_int<256>>& s_61,
	hls::stream<ap_int<256>>& s_62,
	hls::stream<ap_int<256>>& s_63,
	hls::stream<ap_int<256>>& s_64,
	hls::stream<ap_int<256>>& s_65,
	hls::stream<ap_int<256>>& s_66,
	hls::stream<ap_int<256>>& s_67,
	hls::stream<ap_int<256>>& s_68,
	hls::stream<ap_int<256>>& s_69,
	hls::stream<ap_int<256>>& s_70,
	hls::stream<ap_int<256>>& s_71,
	hls::stream<ap_int<256>>& s_72,
	hls::stream<ap_int<256>>& s_73,
	hls::stream<ap_int<256>>& s_74,
	hls::stream<ap_int<256>>& s_75,
	hls::stream<ap_int<256>>& s_76,
	hls::stream<ap_int<256>>& s_77,
	hls::stream<ap_int<256>>& s_78,
	hls::stream<ap_int<256>>& s_79,
	hls::stream<ap_int<256>>& s_80,
	hls::stream<ap_int<256>>& s_81,
	hls::stream<ap_int<256>>& s_82,
	hls::stream<ap_int<256>>& s_83,
	hls::stream<ap_int<256>>& s_84,
	hls::stream<ap_int<256>>& s_85,
	hls::stream<ap_int<256>>& s_86,
	hls::stream<ap_int<256>>& s_87,
	hls::stream<ap_int<256>>& s_88,
	hls::stream<ap_int<256>>& s_89,
	hls::stream<ap_int<256>>& s_90,
	hls::stream<ap_int<256>>& s_91,
	hls::stream<ap_int<256>>& s_92,
	hls::stream<ap_int<256>>& s_93,
	hls::stream<ap_int<256>>& s_94,
	hls::stream<ap_int<256>>& s_95,
	hls::stream<ap_int<256>>& s_96,
	hls::stream<ap_int<256>>& s_97,
	hls::stream<ap_int<256>>& s_98,
	hls::stream<ap_int<256>>& s_99,
	hls::stream<ap_int<256>>& s_100,
	hls::stream<ap_int<256>>& s_101,
	hls::stream<ap_int<256>>& s_102,
	hls::stream<ap_int<256>>& s_103,
	hls::stream<ap_int<256>>& s_104,
	hls::stream<ap_int<256>>& s_105,
	hls::stream<ap_int<256>>& s_106,
	hls::stream<ap_int<256>>& s_107,
	hls::stream<ap_int<256>>& s_108,
	hls::stream<ap_int<256>>& s_109,
	hls::stream<ap_int<256>>& s_110,
	hls::stream<ap_int<256>>& s_111,
	hls::stream<ap_int<256>>& s_112,
	hls::stream<ap_int<256>>& s_113,
	hls::stream<ap_int<256>>& s_114,
	hls::stream<ap_int<256>>& s_115,
	hls::stream<ap_int<256>>& s_116,
	hls::stream<ap_int<256>>& s_117,
	hls::stream<ap_int<256>>& s_118,
	hls::stream<ap_int<256>>& s_119,
	hls::stream<ap_int<256>>& s_120,
	hls::stream<ap_int<256>>& s_121,
	hls::stream<ap_int<256>>& s_122,
	hls::stream<ap_int<256>>& s_123,
	hls::stream<ap_int<256>>& s_124,
	hls::stream<ap_int<256>>& s_125,
	hls::stream<ap_int<256>>& s_126,
	hls::stream<ap_int<256>>& s_127,
	hls::stream<ap_int<256>>& s_128
	) {

	// ap_int<256>> histogram_rows 

	#pragma HLS interface m_axi port=histogram_rows depth=256*257 offset=slave bundle=gmem1
		#pragma HLS interface axis port=s_1
		#pragma HLS interface axis port=s_2
		#pragma HLS interface axis port=s_3
		#pragma HLS interface axis port=s_4
		#pragma HLS interface axis port=s_5
		#pragma HLS interface axis port=s_6
		#pragma HLS interface axis port=s_7
		#pragma HLS interface axis port=s_8
		#pragma HLS interface axis port=s_9
		#pragma HLS interface axis port=s_10
		#pragma HLS interface axis port=s_11
		#pragma HLS interface axis port=s_12
		#pragma HLS interface axis port=s_13
		#pragma HLS interface axis port=s_14
		#pragma HLS interface axis port=s_15
		#pragma HLS interface axis port=s_16
		#pragma HLS interface axis port=s_17
		#pragma HLS interface axis port=s_18
		#pragma HLS interface axis port=s_19
		#pragma HLS interface axis port=s_20
		#pragma HLS interface axis port=s_21
		#pragma HLS interface axis port=s_22
		#pragma HLS interface axis port=s_23
		#pragma HLS interface axis port=s_24
		#pragma HLS interface axis port=s_25
		#pragma HLS interface axis port=s_26
		#pragma HLS interface axis port=s_27
		#pragma HLS interface axis port=s_28
		#pragma HLS interface axis port=s_29
		#pragma HLS interface axis port=s_30
		#pragma HLS interface axis port=s_31
		#pragma HLS interface axis port=s_32
		#pragma HLS interface axis port=s_33
		#pragma HLS interface axis port=s_34
		#pragma HLS interface axis port=s_35
		#pragma HLS interface axis port=s_36
		#pragma HLS interface axis port=s_37
		#pragma HLS interface axis port=s_38
		#pragma HLS interface axis port=s_39
		#pragma HLS interface axis port=s_40
		#pragma HLS interface axis port=s_41
		#pragma HLS interface axis port=s_42
		#pragma HLS interface axis port=s_43
		#pragma HLS interface axis port=s_44
		#pragma HLS interface axis port=s_45
		#pragma HLS interface axis port=s_46
		#pragma HLS interface axis port=s_47
		#pragma HLS interface axis port=s_48
		#pragma HLS interface axis port=s_49
		#pragma HLS interface axis port=s_50
		#pragma HLS interface axis port=s_51
		#pragma HLS interface axis port=s_52
		#pragma HLS interface axis port=s_53
		#pragma HLS interface axis port=s_54
		#pragma HLS interface axis port=s_55
		#pragma HLS interface axis port=s_56
		#pragma HLS interface axis port=s_57
		#pragma HLS interface axis port=s_58
		#pragma HLS interface axis port=s_59
		#pragma HLS interface axis port=s_60
		#pragma HLS interface axis port=s_61
		#pragma HLS interface axis port=s_62
		#pragma HLS interface axis port=s_63
		#pragma HLS interface axis port=s_64
		#pragma HLS interface axis port=s_65
		#pragma HLS interface axis port=s_66
		#pragma HLS interface axis port=s_67
		#pragma HLS interface axis port=s_68
		#pragma HLS interface axis port=s_69
		#pragma HLS interface axis port=s_70
		#pragma HLS interface axis port=s_71
		#pragma HLS interface axis port=s_72
		#pragma HLS interface axis port=s_73
		#pragma HLS interface axis port=s_74
		#pragma HLS interface axis port=s_75
		#pragma HLS interface axis port=s_76
		#pragma HLS interface axis port=s_77
		#pragma HLS interface axis port=s_78
		#pragma HLS interface axis port=s_79
		#pragma HLS interface axis port=s_80
		#pragma HLS interface axis port=s_81
		#pragma HLS interface axis port=s_82
		#pragma HLS interface axis port=s_83
		#pragma HLS interface axis port=s_84
		#pragma HLS interface axis port=s_85
		#pragma HLS interface axis port=s_86
		#pragma HLS interface axis port=s_87
		#pragma HLS interface axis port=s_88
		#pragma HLS interface axis port=s_89
		#pragma HLS interface axis port=s_90
		#pragma HLS interface axis port=s_91
		#pragma HLS interface axis port=s_92
		#pragma HLS interface axis port=s_93
		#pragma HLS interface axis port=s_94
		#pragma HLS interface axis port=s_95
		#pragma HLS interface axis port=s_96
		#pragma HLS interface axis port=s_97
		#pragma HLS interface axis port=s_98
		#pragma HLS interface axis port=s_99
		#pragma HLS interface axis port=s_100
		#pragma HLS interface axis port=s_101
		#pragma HLS interface axis port=s_102
		#pragma HLS interface axis port=s_103
		#pragma HLS interface axis port=s_104
		#pragma HLS interface axis port=s_105
		#pragma HLS interface axis port=s_106
		#pragma HLS interface axis port=s_107
		#pragma HLS interface axis port=s_108
		#pragma HLS interface axis port=s_109
		#pragma HLS interface axis port=s_110
		#pragma HLS interface axis port=s_111
		#pragma HLS interface axis port=s_112
		#pragma HLS interface axis port=s_113
		#pragma HLS interface axis port=s_114
		#pragma HLS interface axis port=s_115
		#pragma HLS interface axis port=s_116
		#pragma HLS interface axis port=s_117
		#pragma HLS interface axis port=s_118
		#pragma HLS interface axis port=s_119
		#pragma HLS interface axis port=s_120
		#pragma HLS interface axis port=s_121
		#pragma HLS interface axis port=s_122
		#pragma HLS interface axis port=s_123
		#pragma HLS interface axis port=s_124
		#pragma HLS interface axis port=s_125
		#pragma HLS interface axis port=s_126
		#pragma HLS interface axis port=s_127
		#pragma HLS interface axis port=s_128
		#pragma HLS interface s_axilite port=histogram_rows bundle=control
		#pragma HLS interface s_axilite port=image_size bundle=control
		#pragma HLS interface s_axilite port=return bundle=control


		ap_int<256> input_1;
		input_1.range(31,0) = image_size;

		ap_int<256> input_2;
		input_2.range(31,0) = image_size;

		ap_int<256> input_3;
		input_3.range(31,0) = image_size;

		ap_int<256> input_4;
		input_4.range(31,0) = image_size;

		ap_int<256> input_5;
		input_5.range(31,0) = image_size;

		ap_int<256> input_6;
		input_6.range(31,0) = image_size;

		ap_int<256> input_7;
		input_7.range(31,0) = image_size;

		ap_int<256> input_8;
		input_8.range(31,0) = image_size;

		ap_int<256> input_9;
		input_9.range(31,0) = image_size;

		ap_int<256> input_10;
		input_10.range(31,0) = image_size;

		ap_int<256> input_11;
		input_11.range(31,0) = image_size;

		ap_int<256> input_12;
		input_12.range(31,0) = image_size;

		ap_int<256> input_13;
		input_13.range(31,0) = image_size;

		ap_int<256> input_14;
		input_14.range(31,0) = image_size;

		ap_int<256> input_15;
		input_15.range(31,0) = image_size;

		ap_int<256> input_16;
		input_16.range(31,0) = image_size;

		ap_int<256> input_17;
		input_17.range(31,0) = image_size;

		ap_int<256> input_18;
		input_18.range(31,0) = image_size;

		ap_int<256> input_19;
		input_19.range(31,0) = image_size;

		ap_int<256> input_20;
		input_20.range(31,0) = image_size;

		ap_int<256> input_21;
		input_21.range(31,0) = image_size;

		ap_int<256> input_22;
		input_22.range(31,0) = image_size;

		ap_int<256> input_23;
		input_23.range(31,0) = image_size;

		ap_int<256> input_24;
		input_24.range(31,0) = image_size;

		ap_int<256> input_25;
		input_25.range(31,0) = image_size;

		ap_int<256> input_26;
		input_26.range(31,0) = image_size;

		ap_int<256> input_27;
		input_27.range(31,0) = image_size;

		ap_int<256> input_28;
		input_28.range(31,0) = image_size;

		ap_int<256> input_29;
		input_29.range(31,0) = image_size;

		ap_int<256> input_30;
		input_30.range(31,0) = image_size;

		ap_int<256> input_31;
		input_31.range(31,0) = image_size;

		ap_int<256> input_32;
		input_32.range(31,0) = image_size;

		ap_int<256> input_33;
		input_33.range(31,0) = image_size;

		ap_int<256> input_34;
		input_34.range(31,0) = image_size;

		ap_int<256> input_35;
		input_35.range(31,0) = image_size;

		ap_int<256> input_36;
		input_36.range(31,0) = image_size;

		ap_int<256> input_37;
		input_37.range(31,0) = image_size;

		ap_int<256> input_38;
		input_38.range(31,0) = image_size;

		ap_int<256> input_39;
		input_39.range(31,0) = image_size;

		ap_int<256> input_40;
		input_40.range(31,0) = image_size;

		ap_int<256> input_41;
		input_41.range(31,0) = image_size;

		ap_int<256> input_42;
		input_42.range(31,0) = image_size;

		ap_int<256> input_43;
		input_43.range(31,0) = image_size;

		ap_int<256> input_44;
		input_44.range(31,0) = image_size;

		ap_int<256> input_45;
		input_45.range(31,0) = image_size;

		ap_int<256> input_46;
		input_46.range(31,0) = image_size;

		ap_int<256> input_47;
		input_47.range(31,0) = image_size;

		ap_int<256> input_48;
		input_48.range(31,0) = image_size;

		ap_int<256> input_49;
		input_49.range(31,0) = image_size;

		ap_int<256> input_50;
		input_50.range(31,0) = image_size;

		ap_int<256> input_51;
		input_51.range(31,0) = image_size;

		ap_int<256> input_52;
		input_52.range(31,0) = image_size;

		ap_int<256> input_53;
		input_53.range(31,0) = image_size;

		ap_int<256> input_54;
		input_54.range(31,0) = image_size;

		ap_int<256> input_55;
		input_55.range(31,0) = image_size;

		ap_int<256> input_56;
		input_56.range(31,0) = image_size;

		ap_int<256> input_57;
		input_57.range(31,0) = image_size;

		ap_int<256> input_58;
		input_58.range(31,0) = image_size;

		ap_int<256> input_59;
		input_59.range(31,0) = image_size;

		ap_int<256> input_60;
		input_60.range(31,0) = image_size;

		ap_int<256> input_61;
		input_61.range(31,0) = image_size;

		ap_int<256> input_62;
		input_62.range(31,0) = image_size;

		ap_int<256> input_63;
		input_63.range(31,0) = image_size;

		ap_int<256> input_64;
		input_64.range(31,0) = image_size;

		ap_int<256> input_65;
		input_65.range(31,0) = image_size;

		ap_int<256> input_66;
		input_66.range(31,0) = image_size;

		ap_int<256> input_67;
		input_67.range(31,0) = image_size;

		ap_int<256> input_68;
		input_68.range(31,0) = image_size;

		ap_int<256> input_69;
		input_69.range(31,0) = image_size;

		ap_int<256> input_70;
		input_70.range(31,0) = image_size;

		ap_int<256> input_71;
		input_71.range(31,0) = image_size;

		ap_int<256> input_72;
		input_72.range(31,0) = image_size;

		ap_int<256> input_73;
		input_73.range(31,0) = image_size;

		ap_int<256> input_74;
		input_74.range(31,0) = image_size;

		ap_int<256> input_75;
		input_75.range(31,0) = image_size;

		ap_int<256> input_76;
		input_76.range(31,0) = image_size;

		ap_int<256> input_77;
		input_77.range(31,0) = image_size;

		ap_int<256> input_78;
		input_78.range(31,0) = image_size;

		ap_int<256> input_79;
		input_79.range(31,0) = image_size;

		ap_int<256> input_80;
		input_80.range(31,0) = image_size;

		ap_int<256> input_81;
		input_81.range(31,0) = image_size;

		ap_int<256> input_82;
		input_82.range(31,0) = image_size;

		ap_int<256> input_83;
		input_83.range(31,0) = image_size;

		ap_int<256> input_84;
		input_84.range(31,0) = image_size;

		ap_int<256> input_85;
		input_85.range(31,0) = image_size;

		ap_int<256> input_86;
		input_86.range(31,0) = image_size;

		ap_int<256> input_87;
		input_87.range(31,0) = image_size;

		ap_int<256> input_88;
		input_88.range(31,0) = image_size;

		ap_int<256> input_89;
		input_89.range(31,0) = image_size;

		ap_int<256> input_90;
		input_90.range(31,0) = image_size;

		ap_int<256> input_91;
		input_91.range(31,0) = image_size;

		ap_int<256> input_92;
		input_92.range(31,0) = image_size;

		ap_int<256> input_93;
		input_93.range(31,0) = image_size;

		ap_int<256> input_94;
		input_94.range(31,0) = image_size;

		ap_int<256> input_95;
		input_95.range(31,0) = image_size;

		ap_int<256> input_96;
		input_96.range(31,0) = image_size;

		ap_int<256> input_97;
		input_97.range(31,0) = image_size;

		ap_int<256> input_98;
		input_98.range(31,0) = image_size;

		ap_int<256> input_99;
		input_99.range(31,0) = image_size;

		ap_int<256> input_100;
		input_100.range(31,0) = image_size;

		ap_int<256> input_101;
		input_101.range(31,0) = image_size;

		ap_int<256> input_102;
		input_102.range(31,0) = image_size;

		ap_int<256> input_103;
		input_103.range(31,0) = image_size;

		ap_int<256> input_104;
		input_104.range(31,0) = image_size;

		ap_int<256> input_105;
		input_105.range(31,0) = image_size;

		ap_int<256> input_106;
		input_106.range(31,0) = image_size;

		ap_int<256> input_107;
		input_107.range(31,0) = image_size;

		ap_int<256> input_108;
		input_108.range(31,0) = image_size;

		ap_int<256> input_109;
		input_109.range(31,0) = image_size;

		ap_int<256> input_110;
		input_110.range(31,0) = image_size;

		ap_int<256> input_111;
		input_111.range(31,0) = image_size;

		ap_int<256> input_112;
		input_112.range(31,0) = image_size;

		ap_int<256> input_113;
		input_113.range(31,0) = image_size;

		ap_int<256> input_114;
		input_114.range(31,0) = image_size;

		ap_int<256> input_115;
		input_115.range(31,0) = image_size;

		ap_int<256> input_116;
		input_116.range(31,0) = image_size;

		ap_int<256> input_117;
		input_117.range(31,0) = image_size;

		ap_int<256> input_118;
		input_118.range(31,0) = image_size;

		ap_int<256> input_119;
		input_119.range(31,0) = image_size;

		ap_int<256> input_120;
		input_120.range(31,0) = image_size;

		ap_int<256> input_121;
		input_121.range(31,0) = image_size;

		ap_int<256> input_122;
		input_122.range(31,0) = image_size;

		ap_int<256> input_123;
		input_123.range(31,0) = image_size;

		ap_int<256> input_124;
		input_124.range(31,0) = image_size;

		ap_int<256> input_125;
		input_125.range(31,0) = image_size;

		ap_int<256> input_126;
		input_126.range(31,0) = image_size;

		ap_int<256> input_127;
		input_127.range(31,0) = image_size;

		ap_int<256> input_128;
		input_128.range(31,0) = image_size;


	for (int32_t j = 0; j < LOOPS_J*ENTROPY_KERNELS; j++) {
		// no 8 input_1.range, just s.write(input_1)
		s_1.write(histogram_rows[j]);
		s_2.write(histogram_rows[j+1]);
		s_3.write(histogram_rows[j+2]);
		s_4.write(histogram_rows[j+3]);
		s_5.write(histogram_rows[j+4]);
		s_6.write(histogram_rows[j+5]);
		s_7.write(histogram_rows[j+6]);
		s_8.write(histogram_rows[j+7]);
		s_9.write(histogram_rows[j+8]);
		s_10.write(histogram_rows[j+9]);
		s_11.write(histogram_rows[j+10]);
		s_12.write(histogram_rows[j+11]);
		s_13.write(histogram_rows[j+12]);
		s_14.write(histogram_rows[j+13]);
		s_15.write(histogram_rows[j+14]);
		s_16.write(histogram_rows[j+15]);
		s_17.write(histogram_rows[j+16]);
		s_18.write(histogram_rows[j+17]);
		s_19.write(histogram_rows[j+18]);
		s_20.write(histogram_rows[j+19]);
		s_21.write(histogram_rows[j+20]);
		s_22.write(histogram_rows[j+21]);
		s_23.write(histogram_rows[j+22]);
		s_24.write(histogram_rows[j+23]);
		s_25.write(histogram_rows[j+24]);
		s_26.write(histogram_rows[j+25]);
		s_27.write(histogram_rows[j+26]);
		s_28.write(histogram_rows[j+27]);
		s_29.write(histogram_rows[j+28]);
		s_30.write(histogram_rows[j+29]);
		s_31.write(histogram_rows[j+30]);
		s_32.write(histogram_rows[j+31]);
		s_33.write(histogram_rows[j+32]);
		s_34.write(histogram_rows[j+33]);
		s_35.write(histogram_rows[j+34]);
		s_36.write(histogram_rows[j+35]);
		s_37.write(histogram_rows[j+36]);
		s_38.write(histogram_rows[j+37]);
		s_39.write(histogram_rows[j+38]);
		s_40.write(histogram_rows[j+39]);
		s_41.write(histogram_rows[j+40]);
		s_42.write(histogram_rows[j+41]);
		s_43.write(histogram_rows[j+42]);
		s_44.write(histogram_rows[j+43]);
		s_45.write(histogram_rows[j+44]);
		s_46.write(histogram_rows[j+45]);
		s_47.write(histogram_rows[j+46]);
		s_48.write(histogram_rows[j+47]);
		s_49.write(histogram_rows[j+48]);
		s_50.write(histogram_rows[j+49]);
		s_51.write(histogram_rows[j+50]);
		s_52.write(histogram_rows[j+51]);
		s_53.write(histogram_rows[j+52]);
		s_54.write(histogram_rows[j+53]);
		s_55.write(histogram_rows[j+54]);
		s_56.write(histogram_rows[j+55]);
		s_57.write(histogram_rows[j+56]);
		s_58.write(histogram_rows[j+57]);
		s_59.write(histogram_rows[j+58]);
		s_60.write(histogram_rows[j+59]);
		s_61.write(histogram_rows[j+60]);
		s_62.write(histogram_rows[j+61]);
		s_63.write(histogram_rows[j+62]);
		s_64.write(histogram_rows[j+63]);
		s_65.write(histogram_rows[j+64]);
		s_66.write(histogram_rows[j+65]);
		s_67.write(histogram_rows[j+66]);
		s_68.write(histogram_rows[j+67]);
		s_69.write(histogram_rows[j+68]);
		s_70.write(histogram_rows[j+69]);
		s_71.write(histogram_rows[j+70]);
		s_72.write(histogram_rows[j+71]);
		s_73.write(histogram_rows[j+72]);
		s_74.write(histogram_rows[j+73]);
		s_75.write(histogram_rows[j+74]);
		s_76.write(histogram_rows[j+75]);
		s_77.write(histogram_rows[j+76]);
		s_78.write(histogram_rows[j+77]);
		s_79.write(histogram_rows[j+78]);
		s_80.write(histogram_rows[j+79]);
		s_81.write(histogram_rows[j+80]);
		s_82.write(histogram_rows[j+81]);
		s_83.write(histogram_rows[j+82]);
		s_84.write(histogram_rows[j+83]);
		s_85.write(histogram_rows[j+84]);
		s_86.write(histogram_rows[j+85]);
		s_87.write(histogram_rows[j+86]);
		s_88.write(histogram_rows[j+87]);
		s_89.write(histogram_rows[j+88]);
		s_90.write(histogram_rows[j+89]);
		s_91.write(histogram_rows[j+90]);
		s_92.write(histogram_rows[j+91]);
		s_93.write(histogram_rows[j+92]);
		s_94.write(histogram_rows[j+93]);
		s_95.write(histogram_rows[j+94]);
		s_96.write(histogram_rows[j+95]);
		s_97.write(histogram_rows[j+96]);
		s_98.write(histogram_rows[j+97]);
		s_99.write(histogram_rows[j+98]);
		s_100.write(histogram_rows[j+99]);
		s_101.write(histogram_rows[j+100]);
		s_102.write(histogram_rows[j+101]);
		s_103.write(histogram_rows[j+102]);
		s_104.write(histogram_rows[j+103]);
		s_105.write(histogram_rows[j+104]);
		s_106.write(histogram_rows[j+105]);
		s_107.write(histogram_rows[j+106]);
		s_108.write(histogram_rows[j+107]);
		s_109.write(histogram_rows[j+108]);
		s_110.write(histogram_rows[j+109]);
		s_111.write(histogram_rows[j+110]);
		s_112.write(histogram_rows[j+111]);
		s_113.write(histogram_rows[j+112]);
		s_114.write(histogram_rows[j+113]);
		s_115.write(histogram_rows[j+114]);
		s_116.write(histogram_rows[j+115]);
		s_117.write(histogram_rows[j+116]);
		s_118.write(histogram_rows[j+117]);
		s_119.write(histogram_rows[j+118]);
		s_120.write(histogram_rows[j+119]);
		s_121.write(histogram_rows[j+120]);
		s_122.write(histogram_rows[j+121]);
		s_123.write(histogram_rows[j+122]);
		s_124.write(histogram_rows[j+123]);
		s_125.write(histogram_rows[j+124]);
		s_126.write(histogram_rows[j+125]);
		s_127.write(histogram_rows[j+126]);
		s_128.write(histogram_rows[j+127]);
	}
}
}
