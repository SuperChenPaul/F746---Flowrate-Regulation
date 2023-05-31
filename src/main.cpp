#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);
// Initialize a pins to perform analog input and digital output functions
AnalogIn   capteur1(A0), capteur2(A1);
DigitalOut pompe1(D1), pompe2(D2);

int main() {

    threadLvgl.lock();

    printf("Valeur Capteur1 : %d", capteur1);
    printf("Valeur Capteur2 : %d", capteur2);
    
    lv_demo_widgets();

    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(10ms);
    }
}