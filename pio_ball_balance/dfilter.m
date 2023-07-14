s = tf ('s');

T = 0.001;
Ts = 0.0001;

W = s / (T*s + 1);
Wz = c2d (W, Ts);

get_diffeq (Wz, 'in', 'out');