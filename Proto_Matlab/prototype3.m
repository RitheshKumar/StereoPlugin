% clear all;
clc;
close all;

% [x,fs] = audioread('Day.wav');
% [x,fs] = audioread('L_Dopa.wav');
[x,fs] = audioread('Making_Plans_for_Nigel.wav');
x1 = x;
x = mean(x,2);
freq = 15000;
% divide the freq into two parts: lower part is for both L and R, upper
% part is either for L or R
division = 0.2;
% further divide the upper part into 6 bands
division2 = 1.0 / 6;
bandwidth = freq*(1-division)*division2;

%20 ms delay in samples
numDelaysPerSec= 1/(20/1000);
delayInSamples = fs/numDelaysPerSec;

%% pass bands after frequency division
freqL1 = [freq*division freq*division+bandwidth];
freqR1 = [freq*division+bandwidth freq*division+2*bandwidth];
freqL2 = [freq*division+2*bandwidth freq*division+3*bandwidth];
freqR2 = [freq*division+3*bandwidth freq*division+4*bandwidth];
freqL3 = [freq*division+4*bandwidth freq*division+5*bandwidth];
freqR3 = [freq*division+5*bandwidth freq*division+6*bandwidth];
freqC  = freq * division;

%% Left Channels

Wp = freqL1 / (fs / 2);
Ws = [freqL1(1)-500 freqL1(2)+500] / (fs / 2);
Rp = 3;
Rs = 20;
[bL1, aL1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = freqL2 / (fs / 2);
Ws = [freqL2(1)-500 freqL2(2)+500] / (fs / 2);
[bL2, aL2] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = freqL3 / (fs / 2);
Ws = [freqL3(1)-500 freqL3(2)+500] / (fs / 2);
[bL3, aL3] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


%% Right Channels

Wp = freqR1 / (fs / 2);
Ws = [freqR1(1)-500 freqR1(2)+500] / (fs / 2);
[bR1, aR1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = freqR2 / (fs / 2);
Ws = [freqR2(1)-500 freqR2(2)+500] / (fs / 2);
[bR2, aR2] = getButtCoeffs( Wp, Ws, Rp, Rs, 1 );


Wp = [freqR3(1) freqR3(2)-500] / (fs / 2);
Ws = [freqR3(1)-500 freqR3(2)] / (fs / 2);
[bR3, aR3] = getButtCoeffs( Wp, Ws, Rp, Rs, 1);

%% Common LPF

Wp = (freqC - 500) / (fs / 2);
Ws = freqC / (fs / 2);
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
 yC1 = 1.23*yC1;


%% Getting stereo channels

% Left Channel
yL = yL1 + yL2 + yL3 + yC1;
% yL = (yL - min(yL))./(max(yL) - min(yL));

% Right Channel
yR = yR1 + yR2 + yR3 + yC1;
% yR = (yR - min(yR))./(max(yR) - min(yR));


%% Add Delay and Attenuate Selected Frequencies

yRDel = [zeros(delayInSamples,1); yR ];
yLDel = [zeros(delayInSamples,1); yL ];

% Eliminate vocal frequencies from delayed sig
Wp = 2500 / (fs/2);
Ws = 2000/ (fs/2);
Rp = 3;
Rs = 15;

[bDel1, aDel1] = getButtCoeffs( Wp, Ws, Rp, Rs, 1, 1 );
yRDel1 = filter( bDel1, aDel1, yRDel );
yLDel1 = filter( bDel1, aDel1, yLDel );

%Emphasize Higher Frequencies %3dB Gain
yRDel2 = yRDel1*1.4125;
yLDel2 = yRDel1*1.4125;

%Add them up
yR    = [yR; zeros(delayInSamples,1);];
yL    = [yL; zeros(delayInSamples,1);];

y     = [yLDel2+yL, yRDel2+yR ];
yClean= [ yL, yR ];
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