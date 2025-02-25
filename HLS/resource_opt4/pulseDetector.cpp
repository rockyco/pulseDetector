#include "pulseDetector.hpp"
#include <cmath>
#include <complex>

// Static parameters for the FIR filter IP
struct config1 : hls::ip_fir::params_t {
    static const unsigned num_channels = 1;
    static const unsigned total_num_coeff = FILTER_LENGTH;
    static const double coeff_vec[total_num_coeff];
    static const unsigned input_width = INPUT_WIDTH;
    static const unsigned input_fractional_bits = INPUT_FRACTIONAL_BITS;
    static const unsigned output_width = OUTPUT_WIDTH;
    static const unsigned output_fractional_bits = OUTPUT_FRACTIONAL_BITS;
    static const unsigned coeff_width = COEFF_WIDTH;
    static const unsigned coeff_fractional_bits = COEFF_FRACTIONAL_BITS;
    static const unsigned input_length = INPUT_LENGTH;
    static const unsigned output_length = OUTPUT_LENGTH;
    static const unsigned num_coeffs = COEFF_NUM;
    static const unsigned coeff_sets = COEFF_SETS;
    static const unsigned quantization = 1;
};
const double config1::coeff_vec[FILTER_LENGTH] = {
#include "coeff1.txt"
};

struct config2 : hls::ip_fir::params_t {
    static const unsigned num_channels = 1;
    static const unsigned total_num_coeff = FILTER_LENGTH;
    static const double coeff_vec[total_num_coeff];
    static const unsigned input_width = INPUT_WIDTH;
    static const unsigned input_fractional_bits = INPUT_FRACTIONAL_BITS;
    static const unsigned output_width = OUTPUT_WIDTH;
    static const unsigned output_fractional_bits = OUTPUT_FRACTIONAL_BITS;
    static const unsigned coeff_width = COEFF_WIDTH;
    static const unsigned coeff_fractional_bits = COEFF_FRACTIONAL_BITS;
    static const unsigned input_length = INPUT_LENGTH;
    static const unsigned output_length = OUTPUT_LENGTH;
    static const unsigned num_coeffs = COEFF_NUM;
    static const unsigned coeff_sets = COEFF_SETS;
    static const unsigned quantization = 1;
};
const double config2::coeff_vec[FILTER_LENGTH] = {
#include "coeff2.txt"
};

struct config3 : hls::ip_fir::params_t {
    static const unsigned num_channels = 1;
    static const unsigned total_num_coeff = FILTER_LENGTH;
    static const double coeff_vec[total_num_coeff];
    static const unsigned input_width = INPUT_WIDTH;
    static const unsigned input_fractional_bits = INPUT_FRACTIONAL_BITS;
    static const unsigned output_width = OUTPUT_WIDTH;
    static const unsigned output_fractional_bits = OUTPUT_FRACTIONAL_BITS;
    static const unsigned coeff_width = COEFF_WIDTH;
    static const unsigned coeff_fractional_bits = COEFF_FRACTIONAL_BITS;
    static const unsigned input_length = INPUT_LENGTH;
    static const unsigned output_length = OUTPUT_LENGTH;
    static const unsigned num_coeffs = COEFF_NUM;
    static const unsigned coeff_sets = COEFF_SETS;
    static const unsigned quantization = 1;
};
const double config3::coeff_vec[FILTER_LENGTH] = {
#include "coeff3.txt"
};

template<typename data_t, int LENGTH>
void process_fe(complex_stream &in, hls::stream<data_t> &out1, hls::stream<data_t> &out2, hls::stream<data_t> &out3) {

    for(unsigned i = 0; i < LENGTH; i++) {
#pragma HLS PIPELINE II=1 rewind=true
        std::complex<data_t> val = in.read();
        out1.write(val.real());
        out2.write(val.imag());
        out3.write(val.real() + val.imag());
    }
}

template<typename data_t, int LENGTH>
void process_be(hls::stream<m_data_t> &in1, hls::stream<m_data_t> &in2, hls::stream<m_data_t> &in3, real_stream &out) {

    for(unsigned i = 0; i < LENGTH; ++i) {
#pragma HLS PIPELINE II=1 rewind=true

        data_t val1 = in1.read();
        data_t val2 = in2.read();
        data_t val3 = in3.read();
        data_t real = val1 - val3;
        data_t imag = val2 + val3;
        out.write(real * real + imag * imag);
    }
}

void matchFilter(complex_stream& RxSignal, real_stream& FilterOut) {
#pragma HLS DATAFLOW

    // Create FIR instances
    static hls::FIR<config1> fir1;
    static hls::FIR<config2> fir2;
    static hls::FIR<config3> fir3;

    hls::stream<s_data_t> fe1_out, fe2_out, fe3_out;
#pragma HLS STREAM variable=fe1_out depth=2
#pragma HLS STREAM variable=fe2_out depth=2
#pragma HLS STREAM variable=fe3_out depth=2
    hls::stream<m_data_t> be1_out, be2_out, be3_out;
#pragma HLS STREAM variable=be1_out depth=2
#pragma HLS STREAM variable=be2_out depth=2
#pragma HLS STREAM variable=be3_out depth=2

    process_fe<s_data_t, SIGNAL_LENGTH>(RxSignal, fe1_out, fe2_out, fe3_out);
    fir1.run(fe1_out, be1_out);
    fir2.run(fe2_out, be2_out);
    fir3.run(fe3_out, be3_out);
    process_be<fixed_point, SIGNAL_LENGTH>(be1_out, be2_out, be3_out, FilterOut);
}

void peakFinder(real_stream& FilterOut, fixed_point& peak, int& location) {
    fixed_point current_peak = 0;
    int current_location = 0;

    for (int n = 0; n < SIGNAL_LENGTH; n++) {
#pragma HLS PIPELINE II=1
        fixed_point magVal = FilterOut.read();

        if (magVal > current_peak) {
            current_peak = magVal;
            current_location = n;
        }
    }

    peak = current_peak;
    location = current_location;
}

void pulseDetector(complex_stream& RxSignal, fixed_point& peak, int& location) {
#pragma HLS DATAFLOW
    real_stream FilterOut;
#pragma HLS STREAM variable=FilterOut depth=4 dim=1

    matchFilter(RxSignal, FilterOut);
    peakFinder(FilterOut, peak, location);
}
