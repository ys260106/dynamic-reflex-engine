/*
 *  PROJE: LNN (Liquid Neural Network) Çekirdek Kütüphanesi
 *  SURUM: v1.0
 *  YAZAR: [Yusuf Şahin]
 *  TARIH: [28/12/2025]
 *
 *  ACIKLAMA:
 *  Bu kutuphane, zamanla degisen (Time-Continuous) dinamik nöron yapısını
 *  simüle eden fizik motorunu ve hafiza yonetimini icerir.
 *  Diferansiyel denklemler ve Leaky Integrator modeli kullanilmistir.
 */

#include <stdlib.h>
#include <math.h>
#include "lnn_cekirdek.h"


Lnn_ag* katman_olustur(int adet){
    Lnn_ag *beyin = (Lnn_ag*)malloc(sizeof(Lnn_ag));

    beyin->noron_sayisi = adet;
    beyin->noronlar = (Lnn_Noron*)malloc(adet * sizeof(Lnn_Noron));

    for(int i = 0; i < adet; i++){
        beyin->noronlar[i].y = 0.0;
        beyin->noronlar[i].tau = 1.0;
        beyin->noronlar[i].w = 1.0;
    }
    return beyin;
}

void Lnn_step(Lnn_ag *beyin, float giris, float dt){
    for(int i = 0; i < beyin->noron_sayisi; i++){
        Lnn_Noron *noron = &beyin->noronlar[i];

        double degisim = (-noron->y / noron->tau) + (giris * noron->w);

        noron->y = noron->y + (degisim * dt);
    }
}

void Lnn_Yapilandir(Lnn_ag *beyin, int baslangic, int bitis, float yeni_tau, float yeni_w){
    if(baslangic < 0 || bitis > beyin->noron_sayisi) return;

    for(int i = baslangic; i < bitis; i++){
        beyin->noronlar[i].tau = yeni_tau;
        beyin->noronlar[i].w = yeni_w;
    }

}
float lnn_sigmoid(float x){
    if(x>10.0f) return 0.99999f;
    if(x< -10.0f) return 0.00001;
    return 1.0f / (1.0f + exp(-x));

}

//Not: Buradaki x sigmoidden gelen değer
float lnn_sigmoid_turevi(float x){
    return x * (1.0f - x);
}

void Lnn_Egit(Lnn_ag *ag, float beklenen_cikis, float ogrenme_hizi){
    //Loading
}

void Lnn_Noron_Guncelle(Lnn_Noron *noron, float hata_sinyali, float giris, float ogrenme_hizi){
    //Formül: Değişim = Hata * giriş * hız
    float degisim = hata_sinyali * giris * ogrenme_hizi;
    noron->w += degisim;
}

