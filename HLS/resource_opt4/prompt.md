## Use Vitis HLS library (FIR Filter IP Library) to reduce resource usage

You are an expert in Vitis HLS C++ programming and FPGA architecture design.

**Task:** Replace the existing FIR filter implementation in `pulseDetector.cpp` with the Vitis HLS FIR Filter IP core for improved resource utilization. Strictly adhere to the FIR IP user guide, using the streaming interface.

**FIR IP Configuration:**

*   Utilize the `hls::ip_fir::params_t` struct to configure static parameters.
*   Coefficients are mandatory and must be explicitly defined.
*   Input and output data should be passed as `hls::stream<>`.
*   AMD recommends using the FIR function within a `DATAFLOW` region.

**Implementation Steps:**

1.  **Include Header:** Add the necessary header file.
2.  **Define Static Parameters:** Create a `config1` struct (or similar) derived from `hls::ip_fir::params_t`, defining constants for bit widths and other parameters consistent with your design.
3.  **Specify Coefficients:** Define the FIR filter coefficients.
4.  **Top-Level Function:**
    *   Receive input data (8-bit).
    *   Pass the data to the FIR IP.

**Important Notes:**

*   The FIR IP cannot be used in a pipelined region. Pipeline loops/functions before and after the FIR.
*   Apply the `DATAFLOW` pragma/directive to all loops and functions in the region containing the FIR.

**Example Code (Complex FIR with Three Real-Number FIR IP Cores):**

```C++
// Static parameters for the FIR filter IP
struct config1 : hls::ip_fir::params_t {
    static const unsigned num_channels = 1;
    static const unsigned total_num_coeff = COEFF_NUM;
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
const double param1::coeff_vec[total_num_coeff] = {
#include "coeff1.txt"
};


template<typename data_t, int LENGTH>
void process_fe(hls::stream<std::complex<data_t>> &in, hls::stream<data_t> &out1, hls::stream<data_t> &out2, hls::stream<data_t> &out3) {

    for(unsigned i = 0; i < LENGTH; i++) {
#pragma HLS PIPELINE II=1 rewind=true
        std::complex<data_t> val = in.read();
        out1.write(val.real());
        out2.write(val.imag());
        out3.write(val.real() + val.imag());
    }
}

template<typename data_t, int LENGTH>
void process_be(hls::stream<m_data_t> &in1, hls::stream<m_data_t> &in2, hls::stream<m_data_t> &in3, hls::stream<data_t> &out) {

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
    static hls::FIR<param1> fir1;
    static hls::FIR<param2> fir2;
    static hls::FIR<param3> fir3;

    hls::stream<s_data_t> fe1_out, fe2_out, fe3_out;
#pragma HLS STREAM variable=fe1_out depth=2
#pragma HLS STREAM variable=fe2_out depth=2
#pragma HLS STREAM variable=fe3_out depth=2
    hls::stream<m_data_t> be1_out, be2_out, be3_out;
#pragma HLS STREAM variable=be1_out depth=2
#pragma HLS STREAM variable=be2_out depth=2
#pragma HLS STREAM variable=be3_out depth=2

    process_fe<fixed_point, SIGNAL_LENGTH>(RxSignal, fe1_out, fe2_out, fe3_out);
    fir1.run(fe1_out, be1_out);
    fir2.run(fe2_out, be2_out);
    fir3.run(fe3_out, be3_out);
    process_be<fixed_point, SIGNAL_LENGTH>(be1_out, be2_out, be3_out, FilterOut);
}
```

**Coefficient Conversion:**

Convert your complex filter coefficients (`corrFilterArray`) into three sets of real-number coefficients for the FIR IPs:

*   `param1::coeff_vec[]`: `corrFilterArray[j].real() + corrFilterArray[j].imag()`
*   `param2::coeff_vec[]`: `corrFilterArray[j].real() - corrFilterArray[j].imag()`
*   `param3::coeff_vec[]`: `corrFilterArray[j].imag()`

**Coefficient Initialization:**

1.  **Generate TXT Files:** In your `*_tb.cpp` file, print the three sets of real-valued coefficients to three separate TXT files, formatted as array initializers.
2.  **Include in HLS Code:**  Use `#include "coeff1.txt"` (and similar) to include these coefficient files in your HLS C++ code.

**Applying Filter Coefficients:**

*   **FIR IP 1:** Filter `dataBuff.real()` to obtain `conv_real`.
*   **FIR IP 2:** Filter `dataBuff.imag()` to obtain `conv_imag`.
*   **FIR IP 3:** Filter `dataBuff.real() + dataBuff.imag()` to obtain `conv_plus`.

**Final Complex FIR Output:**

*   `convSum.real() = conv_real - conv_plus`
*   `convSum.imag() = conv_imag + conv_plus`

**Verification:**

Thoroughly test your modified implementation to ensure correct functionality.
