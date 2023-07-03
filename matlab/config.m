% \file Описание конфигурации палки

%% Параметры мира

g = 9.81; % m/s^2
Ts = 1e-3; % ms

%% Кинематические параметры (геометрия)

mOffset = 80e-3; % mm
mLever = 50e-3; % mm
arm = mOffset + mLever;
armLhalfed = 0.25;

ia = arm / mLever;

%% Массовые параметры

armI = 0.0004; % kg*m^2
ballK = 1;

%% Параметры двигателя

im = 75;

Unom = 12; % V
Istall = 5; % A
noLoadRPM = 130; % rpm
noLoadRads = noLoadRPM * (2*pi/60); % rad/s
stallTorque = 22 * (9.81/100); % kg*cm

R = Unom/Istall;
L = 12e-6;
ke = Unom/noLoadRads;
km = stallTorque/Istall;

%% Параметры энкодера

encRes = (2*pi) / 4; % ticks/rev

%% Параметры датчика расстояния

sensorDelay = 60e-3; %1 / 40; % 1/Hz
sensorResolution = 5e-3; % mm