#include "student.h"

#define PIReg 2
#define IReg 1
#define PReg 0

/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -12000, 12000);
    over.setSlider (SliderEnum::prog2, 1000, 8000);
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */

int8_t sgn(float a) {
  if(a > 0) return 1;
  if(a < 0) return -1;
  return 0;
}

void controllerTick (Overlord &over)
{
  static float setPoint;

  static float Ka;

//   Ka = over.getSlider (SliderEnum::prog2) * 0.001;

  setPoint = -over.getSetpoint ();

  static int constexpr regul = PIReg;

  static float constexpr K = 0.6;
  static float constexpr T = 0.06;
  static float constexpr Tu = 0.015;

  //static float constexpr xi = 0.7071;
  //static float constexpr xi = 1;

  static float constexpr Lb = 0.09;
  static float constexpr Ls = 0.02;

  static float constexpr g = 9.8;

  static float constexpr Tp = T;

  static float constexpr Kp = (1 / (5 * (T + Tu) * K)) * (regul == PReg) + (Tp / (4 * Tu * K)) * (regul == PIReg);

  static float constexpr Ki = (1 / (5 * (T + Tu) * K) * 0.006) * (regul == IReg) + (Kp/Tp * 0.006) * (regul == PIReg);
  
  static float constexpr deltaP = K / (K + 1) * (regul == PReg) + (regul != PReg);

  static float constexpr Tu2 = 2 * (Tu + T * (regul == IReg));

  static float KpTheta = 1 / (5 * Tu2 * deltaP);

  static float KpVel = 1 / (4 * 5 * Tu * g * Ls / Lb);

  static float KpX = 1 / (8 * 5 * Tu);

  static float constexpr KiVel = 10*0.006;

  static int constexpr maxVoltage = 12;

  static float constexpr maxTheta = 0.3;

  static float err1;
  static float err2;
  static float err3;
  static float err4;
  static float err4Old;
  static float w;
  static float theta;
  static float vel;
  static float x;
  static float u;
  static float I = 0;
  static float IVel = 0;
  static float setW;
  static float setTheta;
  static float setVel;

  x = -over.getCarX ();
  vel = -over.getCarVel ();
  theta = over.getMotorTheta ();
  w = over.getMotorVel ();

  err4 = setPoint - x;

  setVel = err4 * KpX;

  err3 = setVel - vel;

  setTheta = err3 * KpVel + IVel;

  IVel += err3 * KiVel * (abs(u) <= maxVoltage and u * IVel > 0);

  err4Old = err4;

  err2 = setTheta - theta;

  setW = err2 * KpTheta;

  err1 = setW - w;

  u = err1 * Kp * (regul != IReg) + I;

  I += (Ki * err1 * (regul != PReg)) * (abs(u) <= maxVoltage or sgn(u) != sgn(err1));

  over.setMotorU (u);

//   Serial.print (setPoint);
//   Serial.print ('\t');
//   Serial.print (setVel);
//   Serial.print ('\t');
//   Serial.print (x);
//   Serial.print ('\t');
//   Serial.print (setPoint - vel);
//   Serial.print ('\t');
//   Serial.print (theta);
//   Serial.print ('\t');
//   Serial.print (w);
//   Serial.print ('\t');
//   Serial.print (u);
//   Serial.println ();

}