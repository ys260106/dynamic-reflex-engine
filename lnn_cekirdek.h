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

#ifndef LNN_CEKIRDEK_H
#define LNN_CEKIRDEK_H

typedef struct{
    float y;
    float tau;
    float w;
} Lnn_Noron;

typedef struct{
    int noron_sayisi;
    Lnn_Noron *noronlar;
} Lnn_ag;

Lnn_ag* katman_olustur(int adet);

void Lnn_step(Lnn_ag *beyin, float giris, float dt);

void Lnn_Yapilandir(Lnn_ag *beyin, int baslangic, int bitis, float yeni_tau, float yeni_w);

float lnn_sigmoid(float x);

float lnn_sigmoid_turevi(float x);

//Egitim fonksiyonu
void Lnn_Egit(Lnn_ag *ag, float beklenen_cikis, float ogrenme_hizi);

void Lnn_Noron_Guncelle(Lnn_Noron *noron, float hata_sinyali, float giris, float ogrenme_hizi);


#endif
