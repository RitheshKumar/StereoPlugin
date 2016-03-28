clear all;
clc;
close all;

% [x,fs] = audioread('Day.wav');
[x,fs] = audioread('L_Dopa.wav');
x = mean(x,2);

%% Left Channels


Wp = [2000 5300]/(fs/2);
Ws = [1000 6300]/(fs/2);
Rp = 3;
Rs = 30;
[bL1, aL1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = [8600 11900]/(fs/2);
Ws = [8200 12400]/(fs/2);
Rp = 3;
Rs = 30;
[bL2, aL2] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = [15200 18500]/(fs/2);
Ws = [14200 19500]/(fs/2);
Rp = 3;
Rs = 30;
[bL3, aL3] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


%% Right Channels

Wp = [5300 8600]/(fs/2);
Ws = [4300 9600]/(fs/2);
Rp = 3;
Rs = 30;
[bR1, aR1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );

Wp = [11900 15200]/(fs/2);
Ws = [11400 15700]/(fs/2);
Rp = 3;
Rs = 30;
[bR2, aR2] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = 18500/(fs/2);
Ws = 17500/(fs/2);
Rp = 3;
Rs = 30;
[bR3, aR3] = getButtCoeffs( Wp, Ws, Rp, Rs, 1, 1 );

%% Common LPF

Wp = 1700 / (fs/2);
Ws = 2500/ (fs/2);
Rp = 3;
Rs = 30;
[bC1, aC1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


%% Do filtering

%Left Channel
 yL1 = filter( bL1, aL1, x );
 yL2 = filter( bL2, aL2, x );
 yL2 = 1.6788*yL2; % 4.5 dB
 yL3 = filter( bL3, aL3, x );
 yL3 = 1.6788*yL3; % 4.5 dB
 
 %Right Channel
 yR1 = filter( bR1, aR1, x );
 yR1 = 1.4125*yR1; % 3dB Gain
 yR2 = filter( bR2, aR2, x );
 yR2 = 1.5849*yR2; % 4 dB Gain
 yR3 = filter( bR3, aR3, x );
 yR3 = 1.6788*yR3; % 4.5 dB
 
 %LFC
 yC1 = filter( bC1, aC1, x );
%  yC1 = 0.8*yC1;


%% Mixing them all into stereo

% Left Channel
yL = yL1 + yL2 + yL3 + yC1;
% yL = (yL - min(yL))./(max(yL) - min(yL));

% Right Channel
yR = yR1 + yR2 + yR3 + yC1;
% yR = (yR - min(yR))./(max(yR) - min(yR));

%MixThem
y = [yL, yR];

%% If you wanna cross test
% fs = 1000;
% Wp = [60 200]/(fs/2);
% Ws = [50 250]/(fs/2);
% Rp = 3;
% Rs = 40;
% Wp = [2000 5300]/(fs/2);
% Ws = [1 5500]/(fs/2);
% Rp = 3;
% Rs = 40;
% 
% [n,Wn] = buttord(Wp,Ws,Rp,Rs);
% [z,p,k] = butter(n,Wn);
% sos = zp2sos(z,p,k);
% 
% freqz(sos,128,1000)
% title(sprintf('n = %d Butterworth Bandpass Filter',n))
% 
% [a , b    ] = butter(n,Wn);
% [aL1, bL1 ] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );