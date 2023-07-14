step_resp = motor_step (:,2);
u = motor_step (:,1);
t = 0:0.005:0.005*(numel(u)-1);

plot (t, u, t, step_resp)

s = tf('s');

K = 0.65;
T = 0.05;
Tu = 0.005;
W = K/((T*s + 1)*(Tu*s + 1));

hold on
lsim (W, u, t)
hold off