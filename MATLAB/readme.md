#### Pulse Detector in MATLAB

This project contains a pulse detector implemented in MATLAB. It includes a reference model, a testbench, and a hardware-friendly implementation of the matched filter and peak finder.

##### Overview

The pulse detector is designed to detect the presence and location of a pulse within a received signal. The implementation consists of the following main components:

-   `pulseDetector.m`: The main pulse detection function that correlates the received signal with a matched filter and then uses a peak finder to identify the pulse location. It contains a matched filter function used for correlating the received signal with a predefined filter. And a hardware-friendly implementation of a peak finder algorithm as well.
-   `pulseDetector_tb.m`: A testbench that loads a test signal and filter, runs the pulse detector, and compares the output to a reference.
-   `RxSignal.mat`: A sample received signal data file.
-   `CorrFilter.mat`: A sample correlation filter data file.
-   `peak_ref.mat`: A sample peak reference data file.
-   `location_ref.mat`: A sample location reference data file.

##### Usage

1.  **Run the Testbench:**
    Execute `pulseDetector_tb.m` to run the pulse detector with the generated test data and compare the results against the reference.

##### Functions

-   **`pulseDetector(RxSignal, CorrFilter)`**
    -   **Description:** Detects pulses in the received signal.
    -   **Inputs:**
        -   `RxSignal`: The received signal vector.
        -   `CorrFilter`: The correlation filter vector.
    -   **Outputs:**
        -   `peak`: The peak magnitude of the detected pulse.
        -   `location`: The location (index) of the detected pulse.

-   **`matchFilter(RxSignal, CorrFilter)`**
    -   **Description:** Implements a matched filter to correlate the received signal with the correlation filter.
    -   **Inputs:**
        -   `RxSignal`: The received signal vector.
        -   `CorrFilter`: The correlation filter vector.
    -   **Output:**
        -   `FilterOut`: The output of the matched filter.

-   **`peakFinder(FilterOut)`**
    -   **Description:** Finds the peak magnitude and location in the filter output.
    -   **Input:**
        -   `FilterOut`: The output of the matched filter.
    -   **Outputs:**
        -   `peak`: The peak magnitude.
        -   `location`: The location of the peak.

##### Testbenches

-   **`pulseDetector_tb.m`**
    -   Loads the generated `RxSignal.mat`, `CorrFilter.mat`, `peak_ref.mat`, and `location_ref.mat` files.
    -   Runs the `pulseDetector` function.
    -   Compares the detected location with the reference location and displays whether the test passed or failed.
    -   Saves the input stimulus to TXT files (`RxSignal_in.txt`, `CorrFilter_in.txt`) and the output to TXT files (`peak_out.txt`, `location_out.txt`).

##### Data Files

-   **`RxSignal.mat`**: Received signal data.
-   **`CorrFilter.mat`**: Correlation filter data.
-   **`peak_ref.mat`**: Reference peak magnitude.
-   **`location_ref.mat`**: Reference peak location.

##### TXT Output Files

-   **`RxSignal_in.txt`**: Received signal input stimulus.
-   **`CorrFilter_in.txt`**: Correlation filter input stimulus.
-   **`peak_out.txt`**: Peak magnitude output.
-   **`location_out.txt`**: Peak location output.

##### Dependencies

-   MATLAB

##### Author

Jie Lei

##### Date

2025-02-14
