#include "pulseDetector.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main() {
    complex_stream RxSignal;
    // complex_stream CorrFilter;
    complex_fixed_point corrFilterArray[FILTER_LENGTH];
    fixed_point peak_hw;
    int location_hw;
    fixed_point peak_ref;
    int location_ref;
    int i;

    // Read RxSignal from file
    ifstream rx_file("RxSignal_in.txt");
    if (!rx_file.is_open()) {
        cerr << "Error opening RxSignal_in.txt" << endl;
        return 1;
    }
    string line;
    fixed_point real_part, imag_part;
    i = 0;
    while (getline(rx_file, line) && i < SIGNAL_LENGTH) {
        stringstream ss(line);
        ss >> real_part >> imag_part;
        RxSignal.write(complex_fixed_point(real_part, imag_part));
        i++;
    }
    rx_file.close();

    // Read CorrFilter from file
    ifstream corr_file("CorrFilter_in.txt");
    if (!corr_file.is_open()) {
        cerr << "Error opening CorrFilter_in.txt" << endl;
        return 1;
    }

    i = 0;
    while (getline(corr_file, line) && i < FILTER_LENGTH) {
        stringstream ss(line);
        ss >> real_part >> imag_part;
        corrFilterArray[i] = complex_fixed_point(real_part, imag_part);
        i++;
    }
    corr_file.close();

    // Print corrFilterArray to three files
    ofstream coeff1_file("coeff1.txt");
    if (!coeff1_file.is_open()) {
        cerr << "Error opening coeff1.txt" << endl;
        return 1;
    }

    ofstream coeff2_file("coeff2.txt");
    if (!coeff2_file.is_open()) {
        cerr << "Error opening coeff2.txt" << endl;
        return 1;
    }

    ofstream coeff3_file("coeff3.txt");
    if (!coeff3_file.is_open()) {
        cerr << "Error opening coeff3.txt" << endl;
        return 1;
    }

    for (int k = 0; k < FILTER_LENGTH; k++) {
        fixed_point val1 = corrFilterArray[k].real() + corrFilterArray[k].imag();
        fixed_point val2 = corrFilterArray[k].real() - corrFilterArray[k].imag();
        fixed_point val3 = corrFilterArray[k].imag();

        coeff1_file << val1;
        if (k < FILTER_LENGTH - 1) {
            coeff1_file << "," << endl;
        } else {
            coeff1_file << endl;
        }

        coeff2_file << val2;
        if (k < FILTER_LENGTH - 1) {
            coeff2_file << "," << endl;
        } else {
            coeff2_file << endl;
        }

        coeff3_file << val3;
        if (k < FILTER_LENGTH - 1) {
            coeff3_file << "," << endl;
        } else {
            coeff3_file << endl;
        }
    }
    coeff1_file.close();
    coeff2_file.close();
    coeff3_file.close();

    // Print corrFilterArray to file
    ofstream init_file("corrFilterArray.txt");
    if (!init_file.is_open()) {
        cerr << "Error opening corrFilterArray.txt" << endl;
        return 1;
    }
    //init_file << "{" << endl;
    for (int k = 0; k < FILTER_LENGTH; k++) {
        fixed_point val1 = corrFilterArray[k].real() + corrFilterArray[k].imag();
        fixed_point val2 = corrFilterArray[k].real() - corrFilterArray[k].imag();
        fixed_point val3 = corrFilterArray[k].imag();

        init_file << "{" << val1 << ", " << val2 << ", " << val3 << "}";
        if (k < FILTER_LENGTH - 1) {
            init_file << "," << endl;
        } else {
            init_file << endl;
        }
    }
    //init_file << "}";
    init_file.close();

    // Run the pulse detector
    pulseDetector(RxSignal, peak_hw, location_hw);

    // Read reference peak from file
    ifstream peak_file("peak_out.txt");
    if (!peak_file.is_open()) {
        cerr << "Error opening peak_out.txt" << endl;
        return 1;
    }
    peak_file >> peak_ref;
    peak_file.close();

    // Read reference location from file
    ifstream location_file("location_out.txt");
    if (!location_file.is_open()) {
        cerr << "Error opening location_out.txt" << endl;
        return 1;
    }
    location_file >> location_ref;
    location_file.close();

    // Compare results
    cout << "Hardware Peak: " << peak_hw << ", Location: " << location_hw << endl;
    cout << "Reference Peak: " << peak_ref << ", Location: " << location_ref << endl;

    if (location_hw + 1 == location_ref) {
        cout << "Test passed!" << endl;
        return 0;
    } else {
        cout << "Test failed!" << endl;
        return 1;
    }
}
