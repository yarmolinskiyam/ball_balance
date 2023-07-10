z = tf('z', Ts);

PID2.Kp = 0.174434851321571;
PID2.Ki = 0.0141830161656454;
PID2.Kd = 0.52012336703347;
PID2.N = 20.7311977750407;

PID2.ztf = PID2.Kp + PID2.Ki * Ts / (z - 1) + ...
    PID2.Kd * PID2.N / (1 + PID2.N*Ts / (z - 1));

get_diffeq (PID2.ztf, 'carX', 'enc');


PID1.Kp = 31.3682906205616;
PID1.Ki = 206.027838823462;
PID1.Kd = -0.159973449585125;
PID1.N = 57.9319552493887;

PID1.ztf = PID1.Kp + PID1.Ki * Ts / (z - 1) + ...
    PID1.Kd * PID1.N / (1 + PID1.N*Ts / (z - 1));

get_diffeq (PID1.ztf, 'enc', 'U');
