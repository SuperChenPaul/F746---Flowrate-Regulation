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
float val_puissancepompe1 = 0;
float limitecommande;

int main()
{
    lv_obj_clear_flag(lv_scr_act(),LV_OBJ_FLAG_SCROLLABLE);
    pompe1.period_us(50);
    pompe2.period_us(50);

    threadLvgl.lock();

    /*Coordonnée des points pour crée les réservoirs*/
    static lv_point_t coord_points_res1[] = {{220, 100}, {220, 250}, {300, 250}, {300, 100}};
    static lv_point_t coord_points_res2[] = {{350, 100}, {350, 250}, {430, 250}, {430, 100}};

    /*Style des traits du réservoir */
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BROWN));

    /*Style de l'eau*/
    static lv_style_t style_eau;
    lv_style_init(&style_eau);
    lv_style_set_line_width(&style_eau, 72);
    lv_style_set_line_color(&style_eau, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_eau, false);

    /* Style du slider de niveau*/
    static lv_style_t style_indicator;
    lv_style_init(&style_indicator);
    lv_style_set_bg_opa(&style_indicator, 0);

    /*Création réservoir 1*/
    lv_obj_t *res1;
    res1 = lv_line_create(lv_scr_act());
    lv_line_set_points(res1, coord_points_res1, 4); /*Set the points*/
    lv_obj_add_style(res1, &style_line, 0);

    /*Création réservoir 2*/
    lv_obj_t *res2;
    res2 = lv_line_create(lv_scr_act());
    lv_line_set_points(res2, coord_points_res2, 4); /*Set the points*/
    lv_obj_add_style(res2, &style_line, 0);

    /* Création du niveau de capteur 1*/
    lv_obj_t *NiveauCapteur1;
    NiveauCapteur1 = lv_line_create(lv_scr_act());
    lv_obj_add_style(NiveauCapteur1, &style_eau, 0);

    /* Création du niveau de capteur 2*/
    lv_obj_t *NiveauCapteur2;
    NiveauCapteur2 = lv_line_create(lv_scr_act());
    lv_obj_add_style(NiveauCapteur2, &style_eau, 0);


    /*Slider de la def du  niveau d'eau*/
    lv_obj_t *sliderNiveau = lv_slider_create(lv_scr_act());
    lv_obj_align(sliderNiveau, LV_ALIGN_CENTER, -20, 25);
    lv_obj_set_size(sliderNiveau, 8, 65);
    lv_slider_set_range(sliderNiveau, 255, 590);
    lv_obj_add_style(sliderNiveau, &style_indicator, LV_PART_MAIN);
    lv_obj_add_style(sliderNiveau, &style_indicator, LV_PART_INDICATOR);

    /*Slider pour modifier la puissance de la pompe 1*/
    lv_obj_t *sliderPuissancePompe = lv_slider_create(lv_scr_act());
    lv_obj_align(sliderPuissancePompe, LV_ALIGN_LEFT_MID, 40, -40);
    lv_obj_set_size(sliderPuissancePompe, 100, 15);
    lv_slider_set_range(sliderPuissancePompe, 0, 100);

/* Texte pour indiquer le role du slider puissance pompe*/
    static lv_obj_t * Text1 = lv_label_create(lv_scr_act());
    lv_obj_align_to(Text1,sliderPuissancePompe, LV_ALIGN_CENTER, -40, -30);
    lv_label_set_text(Text1,"POMPE 1 :");

    static lv_obj_t * Text2 = lv_label_create(lv_scr_act());
    lv_obj_align_to(Text2,sliderPuissancePompe, LV_ALIGN_CENTER, -40, 100);
    lv_label_set_text(Text2,"POMPE 2 :");

    /* Valeur en % de la puissance des pompes */
    static lv_obj_t * PuissancePompe1_label = lv_label_create(lv_scr_act());
    lv_obj_align_to(PuissancePompe1_label,sliderPuissancePompe, LV_ALIGN_CENTER, 0, 20);
    
    static lv_obj_t * PuissancePompe2_label = lv_label_create(lv_scr_act());
    lv_obj_align_to(PuissancePompe2_label,sliderPuissancePompe, LV_ALIGN_CENTER, 0, 120);



    threadLvgl.unlock();

    static lv_point_t line_eau1[] = {{260,  0}, {260, 246}};
    static lv_point_t line_eau2[] = {{390,  0}, {390, 246}};

    while (1)
    {
        // put your main code here, to run repeatedly:

        ThisThread::sleep_for(100ms);

        /* Lecture capteur 1 et mise à jour niveau d'eau*/
        valuecap1 = capteur1.read();
        line_eau1[0].y = 246 - ((1 - valuecap1) * 200);
        lv_line_set_points(NiveauCapteur1, line_eau1, 2); /*Set the points*/

        /* Lecture capteur 2 et mise à jour niveau d'eau*/
        valuecap2 = capteur2.read();
        line_eau2[0].y = 246 - ((1 - valuecap2) * 200);
        lv_line_set_points(NiveauCapteur2, line_eau2, 2); /*Set the points*/



        threadLvgl.lock();

        /* Lecture valeur slider pussance pompe et affichage en %*/
        val_puissancepompe1 = (lv_slider_get_value(sliderPuissancePompe) / 100.0);
        pompe1.write(val_puissancepompe1);
        char buf[8];
        lv_snprintf(buf, sizeof(buf), "%d%%", (int)lv_slider_get_value(sliderPuissancePompe));
        lv_label_set_text(PuissancePompe1_label, buf);



        limitecommande = (1 - (lv_slider_get_value(sliderNiveau) / 1000.0));




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
        lv_snprintf(buf, sizeof(buf), "%d%%", (int)puissance*100);
        lv_label_set_text(PuissancePompe2_label, buf);


        threadLvgl.unlock();
    }
}