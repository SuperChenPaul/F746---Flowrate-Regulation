#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);

// Initialize pins for Analog Input (Sensor) and PwM Output (Pump)
AnalogIn capteur1(A0), capteur2(A1);
PwmOut pompe1(D11), pompe2(D3);

double t;
int i = 0;
float valuecap1, valuecap2; // Stock sensor value
short int limite = 0;
float puissance = 0;
float puissancepompe1 = 0;
float limitecommande;

int main()
{
    lv_obj_clear_flag(lv_scr_act(),LV_OBJ_FLAG_SCROLLABLE);
    pompe1.period_us(50);
    pompe2.period_us(50);

    threadLvgl.lock();

    /*Create an array for the points of the line*/
    static lv_point_t line_points[] = {{220, 100}, {220, 250}, {300, 250}, {300, 100}};
    static lv_point_t line_pointsbis[] = {{350, 100}, {350, 250}, {430, 250}, {430, 100}};

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BROWN));

    static lv_style_t style_eau;
    lv_style_init(&style_eau);
    lv_style_set_line_width(&style_eau, 72);
    lv_style_set_line_color(&style_eau, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_eau, false);

    static lv_style_t style_indicator;
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, 0);

    /*Create a line and apply the new style*/
    lv_obj_t *line1;
    line1 = lv_line_create(lv_scr_act());
    lv_line_set_points(line1, line_points, 4); /*Set the points*/
    lv_obj_add_style(line1, &style_line, 0);

    /*Create a line and apply the new style*/
    lv_obj_t *line2;
    line2 = lv_line_create(lv_scr_act());
    lv_line_set_points(line2, line_pointsbis, 4); /*Set the points*/
    lv_obj_add_style(line2, &style_line, 0);

    lv_obj_t *NiveauCapteur1;
    NiveauCapteur1 = lv_line_create(lv_scr_act());
    lv_obj_add_style(NiveauCapteur1, &style_eau, 0);

    lv_obj_t *NiveauCapteur2;
    NiveauCapteur2 = lv_line_create(lv_scr_act());
    lv_obj_add_style(NiveauCapteur2, &style_eau, 0);


    /*Create a slider*/
    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_align(slider, LV_ALIGN_CENTER, -20, 25);
    lv_obj_set_size(slider, 8, 65);
    lv_slider_set_range(slider, 255, 590);
    lv_obj_add_style(slider, &style_indicator, LV_PART_MAIN);
    lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);

    /*Create the slider to modify the power of the pump*/

    lv_obj_t *sliderPuissancePompe = lv_slider_create(lv_scr_act());
    lv_obj_align(sliderPuissancePompe, LV_ALIGN_LEFT_MID, 40, 20);
    lv_obj_set_size(sliderPuissancePompe, 100, 15);
    lv_slider_set_range(sliderPuissancePompe, 0, 100);

    static lv_obj_t * sliderPuissancePompe_label = lv_label_create(lv_scr_act());
    lv_obj_align_to(sliderPuissancePompe_label,sliderPuissancePompe, LV_ALIGN_CENTER, 0, 20);
    

    static lv_obj_t * slider_function_name = lv_label_create(lv_scr_act());
    lv_obj_align_to(slider_function_name,sliderPuissancePompe, LV_ALIGN_CENTER, -40, -40);
    lv_label_set_text(slider_function_name,"Reglage puissance \nde la pompe");



    threadLvgl.unlock();

    static lv_point_t line_eau1[] = {{260, /*(short int)valuecap1*/ 0}, {260, 246}};
    static lv_point_t line_eau2[] = {{390, /*(short int)valuecap2*/ 0}, {390, 246}};

    while (1)
    {
        // put your main code here, to run repeatedly:

        ThisThread::sleep_for(100ms);

        valuecap1 = capteur1.read();
        valuecap2 = capteur2.read();


        threadLvgl.lock();

        puissancepompe1 = (lv_slider_get_value(sliderPuissancePompe) / 100.0);
        limitecommande = (1 - (lv_slider_get_value(slider) / 1000.0));

        limite = 210 - (lv_slider_get_value(slider));

        line_eau1[0].y = 246 - ((1 - valuecap1) * 200);
        lv_line_set_points(NiveauCapteur1, line_eau1, 2); /*Set the points*/

        line_eau2[0].y = 246 - ((1 - valuecap2) * 200);
        lv_line_set_points(NiveauCapteur2, line_eau2, 2); /*Set the points*/

       
        pompe2.write(puissancepompe1);
        char buf[8];
        lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(sliderPuissancePompe));
        lv_label_set_text(sliderPuissancePompe_label, buf);


        if ((valuecap1 < (limitecommande)-0.005) & (puissance >= 0))
        {
            puissance = puissance - 0.05;
            pompe1.write(puissance);
        }
        else if ((valuecap1 > (limitecommande) + 0.005) & (puissance <= 1))
        {
            puissance = puissance + 0.05;
            pompe1.write(puissance);  
        }
        else if ((valuecap1 < 0.385) | (valuecap2 <0.385))
        {
            pompe1.write(0.0f);
            pompe2.write(0.0f);
            lv_slider_set_value(sliderPuissancePompe, 0, LV_ANIM_OFF);
            puissance = 0.0f;
        }
        else
        {
        }

        threadLvgl.unlock();
    }
}