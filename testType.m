clear all;
clc;
close all;

[x,fs] = audioread('Day.wav');
x = mean(x,2);

%% 
Wp = [2000 5300]/(fs/2);
Ws = [1500 5800]/(fs/2);
% Wp = [1000]/(fs/2);
% Ws = [1500]/(fs/2);
Rp = 3;
Rs = 40;

[n, Wn ] = buttord( Wp, Ws, Rp, Rs );
[bL1, aL1 ] = butter( n, Wn );

yL1 = filter( bL1, aL1, x );

soundsc(yL1,fs)