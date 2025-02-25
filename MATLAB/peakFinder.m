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