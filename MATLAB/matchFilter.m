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