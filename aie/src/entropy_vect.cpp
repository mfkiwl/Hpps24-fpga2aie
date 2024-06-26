#include "mutual_entropy_kernels.h"
#include <stdint.h>

#define POLY_GRADE 6
inline aie::vector<float, 8> log2v(aie::vector<float, 8> x);

////////////////////////////////////////////KERNELS/////////////////////////////////////////////////
void log_kernel_function(input_stream<float>* restrict input, output_stream<float>* restrict output){
    for(int i = 0; i < 128; i++){
        aie::vector<float,8> x;
        x = readincr_v<8>(input);
        aie::vector<float,8> y = log2v(x);
        writeincr(output, y);
    }
}

#ifdef ENTROPY_KERNELS
void entropy_vec_kernel_function(input_stream<int32_t>* restrict input, output_stream<float>* restrict output){
    aie::vector<float, 8> x;
    aie::vector<int32_t, 8> xint;
    aie::vector<float, 8> div;
    aie::vector<float, 8> acc = aie::zeros<float, 8>();

    aie::vector<int32_t,8> first_inputs = readincr_v<8>(input);
    int32_t img_size = first_inputs[0];
    div = aie::broadcast<float, 8>(1.0/img_size); //initialize a vector

    aie::vector<int32_t, 8> null_vector = aie::zeros<int32_t, 8>();

    for (int32_t i = 0; i < LOOPS_J; i++) chess_loop_range(LOOPS_J, LOOPS_J) {
        // read 8 i nt from input stream and cast to flow
        xint = readincr_v<8>(input);
        if(aie::not_equal(xint,null_vector)){
            x = aie::to_float(xint,0);
            x = fpmul(div, x);
            aie::vector<float, 8> log2x = log2v(x);
            acc = fpmac(acc,log2x, x); 
        }
    }

    writeincr(output, acc);
}

void entropy_vec_pass_kernel_function(input_stream<int32_t>* restrict input_PL, input_stream<float>* restrict input_AIE, output_stream<float>* restrict output){
    aie::vector<float, 8> x;
    aie::vector<int32_t, 8> xint;
    aie::vector<int32_t, 8> null_vector = aie::zeros<int32_t, 8>();
    aie::vector<float, 8> div;
    aie::vector<float, 8> acc = aie::zeros<float, 8>();

    aie::vector<int32_t,8> first_inputs = readincr_v<8>(input_PL);
    int32_t img_size = first_inputs[0];
    div = aie::broadcast<float, 8>(1.0/img_size); //initialize a vector 

    for (int32_t i = 0; i < LOOPS_J; i++) chess_loop_range(LOOPS_J, LOOPS_J) {
        // read 8 int from input stream and cast to flow
        xint = readincr_v<8>(input_PL);
        if(aie::not_equal(xint,null_vector)){
            x = aie::to_float(xint,0);
            x = fpmul(div, x);
            aie::vector<float, 8> log2x = log2v(x);
            acc = fpmac(acc,log2x, x); 
        }
    }
    
    aie::vector<float, 8> y = readincr_v<8>(input_AIE);
    acc = fpadd(y,acc);
    writeincr(output, acc);
}
#endif

#ifdef MARGINAL_ENTROPY_KERNELS
void marginal_entropy_kernel_function(input_stream<int32_t>* restrict input, output_stream<float>* restrict output){
    aie::vector<float, 8> x;
    aie::vector<float, 8> div;
    aie::vector<float, 8> acc = aie::zeros<float, 8>();

    aie::vector<int32_t,8> first_inputs = readincr_v<8>(input);
    int32_t img_size = first_inputs[0];
    div = aie::broadcast<float, 8>(1.0/img_size); //initialize a vector 

    for (int32_t i = 0; i < LOOPS_M; i++) chess_loop_range(LOOPS_M, LOOPS_M) {
        // read 8 int from input stream and cast to flow
        x = aie::to_float(readincr_v<8>(input),0);
        x = fpmul(div, x);
        aie::vector<float, 8> log2x = log2v(x);
        acc = fpmac(acc,log2x, x);  
    }
    acc = fpneg(acc);
    writeincr(output, acc);
}

void alt_marginal_entropy_kernel_function(input_stream<int32_t>* restrict input, output_stream<float>* restrict output){
    aie::vector<int32, 8> x;
    aie::vector<float, 8> xf;
    aie::vector<float, 8> div;
    aie::vector<float, 8> acc = aie::zeros<float, 8>();

    aie::vector<int32_t,8> first_inputs = readincr_v<8>(input);
    int32_t img_size = first_inputs[0];
    div = aie::broadcast<float, 8>(1.0/img_size); //initialize a vector 

    float log_img_size = aie::detail::utils::log2(img_size);
    aie::vector<float, 8> divlog = aie::broadcast<float, 8>(log_img_size); //initialize a vector

    for (int32_t i = 0; i < LOOPS_M; i++) chess_loop_range(LOOPS_M, LOOPS_M) {
        // read 8 int from input stream and cast to flow
        aie::vector<float, 8> log2x;
        x = readincr_v<8>(input);
        for(int j = 0; j < 8; j++){
            int xj = x[j];
            float log_xj = aie::detail::utils::log2(xj);
            log2x[j] = log_xj;
        }
        log2x = fpsub(log2x,divlog);
        xf = fpmul(aie::to_float(x,0),div);
        acc = fpmac(acc,log2x, xf); 
    }
    acc = fpneg(acc);
    writeincr(output, acc);
}
#endif

////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////
#if POLY_GRADE == 20
const aie::vector<float,8> coeff[21] = {
    aie::broadcast<float, 8>(-2.162482451650974),
    aie::broadcast<float, 8>(2.438341095966947),
    aie::broadcast<float, 8>(0.1493355999272119),
    aie::broadcast<float, 8>(-0.10156277250211566),
    aie::broadcast<float, 8>(1.2234728535527661),
    aie::broadcast<float, 8>(-5.324351056014543),
    aie::broadcast<float, 8>(5.200737004308196),
    aie::broadcast<float, 8>(-0.7632114475116348),
    aie::broadcast<float, 8>(-0.020930198443109775),
    aie::broadcast<float, 8>(-0.705127185161469),
    aie::broadcast<float, 8>(-1.798454036517851),
    aie::broadcast<float, 8>(2.936086969124477),
    aie::broadcast<float, 8>(-1.1340306122427366),
    aie::broadcast<float, 8>(1.1201036691771118),
    aie::broadcast<float, 8>(-2.7095280572914784),
    aie::broadcast<float, 8>(2.499820379959874),
    aie::broadcast<float, 8>(-0.9161342140592404),
    aie::broadcast<float, 8>(-0.02673743091012911),
    aie::broadcast<float, 8>(0.1301258567700421),
    aie::broadcast<float, 8>(-0.03943747177117771),
    aie::broadcast<float, 8>(0.003973118995235309)
};
#endif
#if POLY_GRADE == 15
const aie::vector<float,8> coeff[16] = {
    aie::broadcast<float, 8>(-2.185021107954832),
    aie::broadcast<float, 8>(2.591126066768347),
    aie::broadcast<float, 8>(-0.026689732297413428),
    aie::broadcast<float, 8>(-0.5653597555753339),
    aie::broadcast<float, 8>(1.8013709226882162),
    aie::broadcast<float, 8>(-3.5486894733585115),
    aie::broadcast<float, 8>(0.8475182012769057),
    aie::broadcast<float, 8>(3.5572164442597867),
    aie::broadcast<float, 8>(-3.031059045029442),
    aie::broadcast<float, 8>(0.07582731703385469),
    aie::broadcast<float, 8>(0.18115335115596323),
    aie::broadcast<float, 8>(1.063885110149508),
    aie::broadcast<float, 8>(-1.2025742109151865),
    aie::broadcast<float, 8>(0.5534438026572503),
    aie::broadcast<float, 8>(-0.12312655978226861),
    aie::broadcast<float, 8>(0.01098118140455557)
};
#endif
#if POLY_GRADE == 12
const aie::vector<float,8> coeff[13] = {
    aie::broadcast<float,8>(-2.887605696836763),
    aie::broadcast<float,8>(5.0464806274001),
    aie::broadcast<float,8>(-2.5040164131633027),
    aie::broadcast<float,8>(-0.22960175106168026),
    aie::broadcast<float,8>(0.34330126054346494),
    aie::broadcast<float,8>(0.7639952680805482),
    aie::broadcast<float,8>(-0.026745680160907007),
    aie::broadcast<float,8>(-1.7560161212418721),
    aie::broadcast<float,8>(2.199667175739304),
    aie::broadcast<float,8>(-1.3178314092001424),
    aie::broadcast<float,8>(0.44277487803782706),
    aie::broadcast<float,8>(-0.08064062078328034),
    aie::broadcast<float,8>(0.00623878010000407)
};
#endif
#if POLY_GRADE == 10
const aie::vector<float,8> coeff[11] = {
    aie::broadcast<float, 8>(-2.937870076739335),
    aie::broadcast<float, 8>(5.308370375277232),
    aie::broadcast<float, 8>(-2.8884561054059446),
    aie::broadcast<float, 8>(-0.5094995806092324),
    aie::broadcast<float, 8>(1.7453375454279374),
    aie::broadcast<float, 8>(-0.4214249631147719),
    aie::broadcast<float, 8>(-0.879761230247557),
    aie::broadcast<float, 8>(0.8894297057080814),
    aie::broadcast<float, 8>(-0.380244031535825),
    aie::broadcast<float, 8>(0.08122343694549493),
    aie::broadcast<float, 8>(-0.007104553835066775),
};
#endif
#if POLY_GRADE == 9
const aie::vector<float,8> coeff[10] = {
    aie::broadcast<float, 8>(-3.1750414844983847),
    aie::broadcast<float, 8>(6.5780956852036),
    aie::broadcast<float, 8>(-5.6253832154062655),
    aie::broadcast<float, 8>(2.23309977464333),
    aie::broadcast<float, 8>(1.2332943045661937),
    aie::broadcast<float, 8>(-2.290818406640599),
    aie::broadcast<float, 8>(1.472005859006733),
    aie::broadcast<float, 8>(-0.5147740106591683),
    aie::broadcast<float, 8>(0.09735686235880867),
    aie::broadcast<float, 8>(-0.007835143779211835)
};
#endif
#if POLY_GRADE == 8
const aie::vector<float,8> coeff[9] = {
    aie::broadcast<float, 8>(-3.418518623611719),
    aie::broadcast<float, 8>(8.117256024257928),
    aie::broadcast<float, 8>(-9.918274048480985),
    aie::broadcast<float, 8>(9.166963440090885),
    aie::broadcast<float, 8>(-5.91474330907493),
    aie::broadcast<float, 8>(2.586892446040646),
    aie::broadcast<float, 8>(-0.7314682220728904),
    aie::broadcast<float, 8>(0.12073244533374487),
    aie::broadcast<float, 8>(-0.008840066164581651)
};
#endif
#if POLY_GRADE == 7
const aie::vector<float,8> coeff[8] = {
    aie::broadcast<float, 8>(-3.236836230162079),
    aie::broadcast<float, 8>(7.092516614528918),
    aie::broadcast<float, 8>(-7.410437824819416),
    aie::broadcast<float, 8>(5.688598691849024),
    aie::broadcast<float, 8>(-2.923898103463342),
    aie::broadcast<float, 8>(0.9542069936543227),
    aie::broadcast<float, 8>(-0.17880707148503514),
    aie::broadcast<float, 8>(0.014657528483584896)
};
#endif
#if POLY_GRADE == 6
const aie::vector<float,8> coeff[7] = {
    aie::broadcast<float, 8>(-3.0298541069195313),
    aie::broadcast<float, 8>(6.071708738859809),
    aie::broadcast<float, 8>(-5.2733111083994615),
    aie::broadcast<float, 8>(3.2263640057692955),
    aie::broadcast<float, 8>(-1.2376578316577886),
    aie::broadcast<float, 8>(0.2676558268561194),
    aie::broadcast<float, 8>(-0.02490148634581278)
};
#endif

#if POLY_GRADE == 5
const aie::vector<float,8> coeff[6] = {
    aie::broadcast<float, 8>(-2.788370597139466),
    aie::broadcast<float, 8>(5.051742828537882),
    aie::broadcast<float, 8>(-3.498433370197163),
    aie::broadcast<float, 8>(1.5974318817181195),
    aie::broadcast<float, 8>(-0.40588618962231643),
    aie::broadcast<float, 8>(0.043542578304217594)
};
#endif

#if POLY_GRADE == 2
const aie::vector<float,8> coeff[3] = {
    aie::broadcast<float, 8>(-1.650456069306855),
    aie::broadcast<float, 8>(1.9964060767155798),
    aie::broadcast<float, 8>(-0.33722549505883914)
};
#endif

#if POLY_GRADE > 0
inline aie::vector<float, 8> log2v(aie::vector<float, 8> x){
    //get the exponent
    aie::vector<int32, 8> e_int = as_v8int32(x);
    e_int = aie::downshift(e_int,23);
    e_int = aie::sub(e_int,127); // remove bias
    aie::vector<float, 8> exp =  aie::to_float(e_int,0);

    // overwrite the exponent with 128
    aie::vector<int32,8> m_int = as_v8int32(x);
    m_int = aie::bit_and(0x007fffff, m_int);
    m_int = aie::bit_or(0x3f800000, m_int);
    aie::vector<float, 8> m = as_v8float(m_int); // m = 1+ mantissa

    // Grade G polynomial approx of log2(1+mantissa)
    aie::vector<float,8> y;
    y = fpmac(coeff[POLY_GRADE-1], m, coeff[POLY_GRADE]);
    for(int i = POLY_GRADE - 2; i >= 0; i--){
        y = fpmac(coeff[i], m, y); // y = coeff[i] +  m*y
    }
    
    return fpadd(exp, y); // log2(2^(exp) * (1+mantissa)) = exp + log2(1+mantissa)
}
#endif