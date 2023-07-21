#include "student.h"
//by Андрей Селин
int sgn(float a)
{
    if(a>0) return 1;
    else
    {
        if(a==0) return 0;
        else
        {
            return -1;
        }
    }
}

float diff(float a)
{
static float constexpr Tf=0.01;
static  float e, u=0, i=0;
i+=u*0.006;
e=a-i;
u=e*(1/Tf);
return u;
}
/**
 * @brief Инициализировать систему управления
 * @details Вызывется один раз перед вызовом
 * функции \p controllerTick
 */
void controllerInit (Overlord &over)
{
    over.setSlider (SliderEnum::prog1, -12000, 12000);
    over.setSlider (SliderEnum::prog2, -10000, 10000);
}

/**
 * @brief Выполнить одну итерацию системы управления
 * @details Вызывается раз в 5мс
 */
void controllerTick (Overlord &over)
{
    /*!v Опрос слайдера чтобы получить инфу из внешнего мира */
    static float setPoint;
    //setPoint = over.getSlider (SliderEnum::prog2);
    setPoint = -over.getSetpoint();
    /*!v Опрос кнопки и перевод */
    //setPoint *= !digitalRead (13) * 0.00002;

    /*!v Переменные для хранения информации о мире */
    static float x, x_i, theta, w;

    static float constexpr K=0.775, T=0.064, Tu=0.016, g=9.8, rychag=9/2;
    
    /*!v Опрос реального мира */
    x = -over.getCarX ();
    x_i = diff(x);
    theta = over.getMotorTheta ();
    w = over.getMotorVel ();

    /*!v Регулятор положения вагончика */
    static float err4, x_i0, am=1.5;
    static float Kp4 = 1/(am*4*8*(Tu));
    err4 = setPoint - x;
    x_i0 = err4 * Kp4;

    /*!v Фильтр для симметричного контура ВЫКЛЮЧЕН!!!*/

    float static errf=0, uf=0;
    errf=x_i0-uf;
    uf+=errf/(16*4*Tu)*0.006;

    /*!v Регулятор скорости вагончика ПИ*/
    static float err3, theta_0, i3=0, ac=4;
    static float Kp3 = (rychag)/(4*Tu*g), kpi3=rychag/(16*(4*Tu)*(4*Tu)*g);
    err3 = x_i0 - x_i;
    
    
    theta_0 = (err3 * Kp3 +i3)/ac;
    if(abs(theta_0) > 0.52) {theta_0=sgn(theta_0)*0.52;} else {i3 += kpi3*err3*0.006;};

    /*!v Регулятор положения мотора */
    static float err2, w_0;
    am=4;
    static float Kp2 = 1/(am*2*(Tu));
    err2 = theta_0 - theta;
    w_0 = err2 * Kp2;

    /*!v Регулятор скорости мотора */
    static float err1, u, i=0, Ki;
    static float constexpr Kp1 = T / (2*Tu*K);
    err1 = w_0 - w;
    Ki=1/(2*Tu*K);
    i += Ki*err1*0.006;
    if(abs(i)>12) i=12*sgn(i);
    u = err1 * Kp1 + i;

    /*!v Выдача напряжения на мотор */
    over.setMotorU (u);

    /*!v Вывод отладочной информации */
    // Serial.print (setPoint*100);
    // Serial.print ('\t');
    // Serial.print (x*100);
    // Serial.println ();
}
