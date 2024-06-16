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

/***************************************************************
*
* Configuration header file for Versal 3D Image Registration
*
****************************************************************/
#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef float data_t;
#define CONSTANT_1 32

#endif

#define SYMBOLS 256 //2^8

// Possible graph configurations
#define TEST_LOG 0 // testbench for logarithm
#define TEST_ENTROPY 1 //testbench for entropy
#define ONE_JOINT_ONE_MARGINAL 2 //one kernel for marginal entropy and one for joint
#define FOUR_PYRAMID 3 // four kernels for joint entropy and 1 for marginal. Pyramid reducer
#define FOUR_BY_TWO_SYS_ARR 4 // 2 columns, four rows systolic array for joint entropy and 1 kernel for marginal

#define GRAPH FOUR_BY_TWO_SYS_ARR

#if GRAPH == ONE_JOINT_ONE_MARGINAL
	#define ENTROPY_KERNELS 1
	#define MARGINAL_ENTROPY_KERNELS 1
#endif

#if GRAPH == FOUR_BY_TWO_SYS_ARR
	#define KERNEL_ROWS 4
	#define KERNEL_COLUMNS 2
	#define ENTROPY_KERNELS KERNEL_ROWS * KERNEL_COLUMNS
	#define MARGINAL_ENTROPY_KERNELS 1
	#define REDUCERS 3
#endif

#if GRAPH == FOUR_PYRAMID
	#define MARGINAL_ENTROPY_KERNELS 1
	#define ENTROPY_KERNELS 4
	#define REDUCERS 4 // (2 ^ KERNEL_ROWS)
#endif

#ifdef MARGINAL_ENTROPY_KERNELS
	#define LOOPS_M 2*SYMBOLS / (MARGINAL_ENTROPY_KERNELS*8)
#endif

#ifdef ENTROPY_KERNELS
	#define LOOPS_J SYMBOLS*SYMBOLS / (ENTROPY_KERNELS*8)
#endif