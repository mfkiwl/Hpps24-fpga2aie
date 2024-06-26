#include "mutual_entropy_kernels.h"
#include <stdint.h>

void reduce_vec_kernel_function(input_stream<float>* restrict input_1, input_stream<float>* restrict input_2, output_stream<float>* restrict output){
    aie::vector<float, 8> x;
    aie::vector<float, 8> y;
    aie::vector<float, 8> z;
    x = readincr_v<8>(input_1);
    y = readincr_v<8>(input_2);
    z = fpadd(x,y);
    writeincr(output, z);
}

void reduce_kernel_function(input_stream<float>* restrict input_1, input_stream<float>* restrict input_2, output_stream<float>* restrict output){
    aie::vector<float, 8> x;
    aie::vector<float, 8> y;
    aie::vector<float, 8> z;
    x = readincr_v<8>(input_1);
    y = readincr_v<8>(input_2);
    z = fpadd(x,y);
    aie::vector<float, 4> out = aie::broadcast<float,4>(0.0);
    float tot = aie::reduce_add(z);
    out[0] = tot;
    writeincr(output, out);
}
