#include <mbed.h>
#include <threadLvgl.h>

#include "demos/lv_demos.h"
#include <cstdio>



ThreadLvgl threadLvgl(30);

// Initialize pins for Analog Input (Sensor) and PwM Output (Pump)
AnalogIn   capteur1(A0), capteur2(A1);
PwmOut pompe1(D11), pompe2(D3);


double t;
int i = 0;
float valuecap1, valuecap2; // Stock sensor value
short int limite;
float puissance=0;



int main() {

    pompe1.period_us(50);
    pompe2.period_us(50);

    threadLvgl.lock();







    /*Create an array for the points of the line*/
    static lv_point_t line_points[] = { {220, 100}, {220, 250}, {300, 250}, {300, 100}};
    static lv_point_t line_pointsbis[] = { {350, 100}, {350, 250}, {430, 250}, {430, 100}};

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

    static lv_style_t style_limite;
    lv_style_init(&style_limite);
    lv_style_set_line_width(&style_limite, 3);
    lv_style_set_line_color(&style_limite, lv_palette_main(LV_PALETTE_RED));
    
    static lv_style_t style_indicator;
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator,0);


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
 

        lv_obj_t * NiveauCapteur1;
        NiveauCapteur1 = lv_line_create(lv_scr_act());

        lv_obj_t * NiveauCapteur2;
        NiveauCapteur2 = lv_line_create(lv_scr_act()); 
        lv_obj_add_style(NiveauCapteur1, &style_eau, 0);
               
        lv_obj_t * LineCommandeLim;
        LineCommandeLim = lv_line_create(lv_scr_act());
        lv_obj_add_style(NiveauCapteur2, &style_eau, 0);

            
        /*Create a slider*/
        lv_obj_t * slider = lv_slider_create(lv_scr_act());
        lv_obj_align(slider, LV_ALIGN_CENTER, -20, 35);
        lv_obj_set_size(slider,8,80);
        lv_slider_set_range(slider,0,80);
        lv_obj_add_style(slider,&style_indicator,LV_PART_MAIN);
        lv_obj_add_style(slider,&style_indicator,LV_PART_INDICATOR);


        /*Create the slider to modify the power of the pump*/
        lv_obj_t * sliderPuissancePompe = lv_slider_create(lv_scr_act());
        lv_obj_align(sliderPuissancePompe, LV_ALIGN_LEFT_MID, 20, 50);
        lv_obj_set_size(sliderPuissancePompe,100,15);
        lv_slider_set_range(sliderPuissancePompe,0,100);



        
    threadLvgl.unlock();

    while (1) {
        // put your main code here, to run repeatedly:
        


        ThisThread::sleep_for(100ms);


        valuecap1=246-((1-capteur1.read())*100);
        valuecap2=246-((1-capteur2.read())*100);
        limite =210-(lv_slider_get_value(slider));

        threadLvgl.lock();
   


        lv_point_t line_eau1[]= {{260,(short int)valuecap1},{260,246}};
        lv_line_set_points(NiveauCapteur1, line_eau1, 2);     /*Set the points*/
        

        lv_point_t line_eau2[]= {{390,(short int)valuecap2},{390,246}};
        lv_line_set_points(NiveauCapteur2, line_eau2, 2);     /*Set the points*/
        
        lv_point_t line_CommandeLim[]= {{220,limite},{300,limite}};
        lv_line_set_points(LineCommandeLim, line_CommandeLim, 2);     /*Set the points*/
        lv_obj_add_style(LineCommandeLim, &style_limite, 0);


        threadLvgl.unlock();

        pompe1.write(lv_slider_get_value(sliderPuissancePompe)/100);


        pompe2.write(1.00f);
        if (valuecap1>(0.007*(lv_slider_get_value(slider))+0.1)){
        puissance=puissance-0.1;
        pompe2.write(puissance);
        }
        else if(valuecap1<(0.007*(lv_slider_get_value(slider))-0.1)){
        puissance--;
        puissance=puissance+0.1;
        pompe2.write(puissance);

        }
        else if(((valuecap1>=(0.007*(lv_slider_get_value(slider))+0.1))) && ((valuecap1<=(0.007*(lv_slider_get_value(slider))-0.1)))){

        }


  
            printf("\nValeur Capteur 1 : %f\n", capteur1.read());
            printf("\nValeur Capteur 2 : %f\n", capteur2.read());
            printf("\nMachin :%d\n",lv_slider_get_value(slider));

            

    }
    
}