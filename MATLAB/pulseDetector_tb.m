% This is the testbench for the pulse detector reference model. It loads a
% test signal, runs the reference model and compares the output to the expected
% result. The testbench is implemented in MATLAB.
%
% Author: Jie Lei
% Date: 2025-02-14
%
load('RxSignal.mat');
load('CorrFilter.mat');
% Save input stimulus to TXT file
% writematrix(RxSignal, 'RxSignal_in.txt', 'Delimiter', 'tab');
writematrix([real(RxSignal), imag(RxSignal)], 'RxSignal_in.txt', 'Delimiter', 'tab');
% writematrix(CorrFilter, 'CorrFilter_in.txt', 'Delimiter', 'tab');
writematrix([real(CorrFilter), imag(CorrFilter)], 'CorrFilter_in.txt', 'Delimiter', 'tab');
[peak_hw, location_hw] = pulseDetector(RxSignal, CorrFilter);
% Save output to TXT file
writematrix(peak_hw, 'peak_out.txt', 'Delimiter', 'tab');
writematrix(location_hw, 'location_out.txt', 'Delimiter', 'tab');
load('peak_ref.mat');
load('location_ref.mat');
if isequal(location_hw,location)
    disp('Test passed');
else
    disp('Test failed');
end