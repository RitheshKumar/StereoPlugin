function [b, a] = getButtCoeffs(Wp,Ws,Rp,Rs, draw, type)

    if nargin < 5
        draw = 0;
    end
    
    [n, Wn ] = buttord( Wp, Ws, Rp, Rs );
    if nargin < 6
        [b, a ] = butter( n, Wn );
        [z,p,k] = butter(n,Wn);
    else
        [b, a  ] = butter( n, Wn,'high' );
        [z,p,k] = butter(  n, Wn,'high' );
    end
    
    if ( draw == 1)        
        sos = zp2sos(z,p,k);
        
        figure
        freqz(sos,128,44100)
        title(sprintf('n = %d Butterworth Filter',n))
%         hold on;
    end
end