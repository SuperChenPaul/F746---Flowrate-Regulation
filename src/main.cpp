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

    lv_obj_t * bar1 = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar1, 0, 200);
    lv_obj_center(bar1);
    lv_bar_set_value(bar1, 70, LV_ANIM_OFF);
    lv_style_set_



    pompe1.period_us(50);
    pompe2.period_us(50);
    pompe1.write(1.00f);
    pompe2.write(1.00f);



            /*Create an array for the points of the line*/
    static lv_point_t line_points[] = { {70, 30}, {70, 200}, {150, 200}, {150, 30}};
    static lv_point_t line_pointsbis[] = { {300, 30}, {300, 200}, {380, 200}, {380, 30}};

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, false);

    static lv_style_t style_eau;
    lv_style_init(&style_eau);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, false);
    

    /*Create a line and apply the new style*/
    lv_obj_t * line1;
    line1 = lv_line_create(lv_scr_act());
    lv_line_set_points(line1, line_points, 4);     /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);

        /*Create a line and apply the new style*/
    lv_obj_t * line2;
    line2 = lv_line_create(lv_scr_act());
    lv_line_set_points(line2, line_pointsbis, 4);     /*Set the points*/
    lv_obj_add_style(line2, &style_line, 0);
 
    

    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        ThisThread::sleep_for(500ms);



            printf("\nValeur Capteur 1 : %f\n", capteur1.read());
            printf("\nValeur Capteur 2 : %f\n", capteur2.read());


    }
}