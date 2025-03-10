#ifndef PULSE_DETECTOR_HPP
#define PULSE_DETECTOR_HPP

#include <ap_fixed.h>
#include <hls_stream.h>
#include <hls_fir.h> // Include FIR IP header

// Define fixed-point data types
typedef ap_fixed<18, 2> fixed_point; // Example: 16-bit fixed-point with 8 integer bits
typedef std::complex<fixed_point> complex_fixed_point;

// Define float data types
typedef float float_point;
typedef std::complex<float_point> complex_float_point;

// Define stream types
typedef hls::stream<complex_fixed_point> complex_stream;
typedef hls::stream<fixed_point> real_stream;
typedef hls::stream<int> int_stream;

// Define parameters (example) using macro definitions
#define FILTER_LENGTH 64
#define SIGNAL_LENGTH 5000

// Define constant array
//const fixed_point corrFilterBuff[FILTER_LENGTH][3] = { /* Initialize with appropriate values */ };

// Function declarations
void matchFilter(complex_stream& RxSignal, real_stream& FilterOut);
void peakFinder(real_stream& FilterOut, fixed_point& peak, int& location);
void pulseDetector(complex_stream& RxSignal, fixed_point& peak, int& location);

// For FIR IP core
const unsigned INPUT_WIDTH = 16;
const unsigned INPUT_FRACTIONAL_BITS = 15;
const unsigned OUTPUT_WIDTH = 32;
const unsigned OUTPUT_FRACTIONAL_BITS = 30;
const unsigned COEFF_WIDTH = 16;
const unsigned COEFF_FRACTIONAL_BITS = 15;
const unsigned COEFF_NUM = FILTER_LENGTH;
const unsigned COEFF_SETS = 1;
const unsigned INPUT_LENGTH = SIGNAL_LENGTH;
const unsigned OUTPUT_LENGTH = SIGNAL_LENGTH;
const unsigned CHAN_NUM = 1;
const unsigned SAMPLE_PERIOD = 4;
const unsigned SAMPLE_FREQUENCY = 64;
typedef ap_fixed<INPUT_WIDTH, INPUT_WIDTH - INPUT_FRACTIONAL_BITS> s_data_t;
typedef ap_fixed<OUTPUT_WIDTH, OUTPUT_WIDTH - OUTPUT_FRACTIONAL_BITS> m_data_t;
typedef ap_uint<8> config_t;

#endif
