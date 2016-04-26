function [b,a] = getEllipticCoeffs(Wp, Ws, Rp, Rs, draw, ftype)
    if nargin < 5
        draw = 0;
    end
    [n, ~] = ellipord(Wp, Ws, Rp, Rs);
    if nargin < 6
        [b, a] = ellip(n, Rp, Rs, Wp);
        [z, p, k] = ellip(n, Rp, Rs, Wp);
    else
        [b, a] = ellip(n, Rp, Rs, Wp, 'high');
        [z, p, k] = ellip(n, Rp, Rs, Wp, 'high');
    end
    
    if (draw == 1)
        sos = zp2sos(z, p, k);
        
        freqz(sos, 128, 44100);
        title (sprintf('n = %d elliptic filter', n))
        hold on;
    end

        
end

