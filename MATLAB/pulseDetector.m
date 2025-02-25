% Pulse Detector refercence function
% Author: Jie Lei
% Date: 2025-02-14
% 
% Pulse detection function
function [peak, location] = pulseDetector(RxSignal, CorrFilter)

    % Correlate Rx signal against matched filter using matchFilter
    FilterOut = matchFilter(RxSignal, CorrFilter);

    % Find peak magnitude & location using peakFinder
    [peak, location] = peakFinder(FilterOut);
end

% Matched filter for pulse detection
function [FilterOut] = matchFilter(RxSignal, CorrFilter)
    
    filLen = length(CorrFilter);
    dataBuff = zeros(filLen,1);
    FilterOut = zeros(length(RxSignal),1);

    for i = 1:length(RxSignal)
        dataBuff(2:filLen) = dataBuff(1:filLen-1);
        dataBuff(1) = RxSignal(i);
        FilterOut(i) = sum(dataBuff.*CorrFilter);
    end
end

% Hardware friendly implementation of peak finder
function [peak, location] = peakFinder(FilterOut)

    peak = 0;
    location = 0;

    % Sliding window operation
    for n = 1:length(FilterOut)

        % Compute magnitude squared to avoid sqrt operation
        MagSqVal = abs(FilterOut(n)).^2;

        if MagSqVal(1) > peak
            peak = MagSqVal(1);
            location = n;
        end
    end
end
