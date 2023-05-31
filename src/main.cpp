#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);
// Initialize a pins to perform analog input and digital output functions
AnalogIn   capteur1(A0), capteur2(A1);
PwmOut pompe1(D11), pompe2(D3);
double t;

int main() {

    threadLvgl.lock();

    pompe1.period_us(50);
    pompe2.period_us(50);
    pompe1.write(0.50f);
    pompe2.write(0.50f);


    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(500ms);

            printf("\nValeur Capteur 1 : %f\n", capteur1.read());
            printf("\nValeur Capteur 2 : %f\n", capteur2.read());


    }
}