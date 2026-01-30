/* Robot Beyni V2 -ESP32 Uyarlaması
 * Kod tamamen fizik ile çalışır simülasyon yoktur */

#include <Arduino.h>
#include <lnn_cekirdek.h>

//Donanım ayarları
#define TRIG_PIN 13 //ses gönderen pin
#define ECHO_PIN 12 //ses dinleyen pin
#define LED_PIN 2 // Tepki gösteren pin
#define PIN_KIRMIZI 26  // Beyaz Kablo
#define PIN_SARI    27  // Mavi Kablo
#define PIN_YESIL   14  // Mor Kablo
//Beyin ayarları
#define MAX_MESAFE 200.0f
#define MAX_HIZ 100.0f
#define DT 0.1f

//Hafıza
Lnn_ag *mesafe_noron;
Lnn_ag *hiz_noron;
float onceki_mesafe = 0.0f;
bool ilk_acilis = true;  //İlk turda hız hesabı saçmalamasın diye


void setup (){
    Serial.begin(115200); //PC ile haberleşme hızı

    pinMode(PIN_KIRMIZI, OUTPUT);
    pinMode(PIN_SARI, OUTPUT);
    pinMode(PIN_YESIL, OUTPUT);
    //Pinlerin ne iş yapacağını işlemciye söylüyoruz
    pinMode(TRIG_PIN, OUTPUT); //Bu pin bağıracak
    pinMode(ECHO_PIN, INPUT); //Bu pin dinleyecek
    pinMode(LED_PIN, OUTPUT); //Bu pin ışık vericek


    //Nöronları inşa ediyoruz
    mesafe_noron = katman_olustur(1);
    Lnn_Yapilandir(mesafe_noron,0 ,1 ,0.3f ,20.0f );

    hiz_noron = katman_olustur(1);
    Lnn_Yapilandir(hiz_noron,0 ,1 ,0.5f ,35.0f );

    Serial.println("Sistem Hazır. Sensör Dinleniyor...");

    delay(1000); // 1 saniye bekliyoruz

}

void loop(){
    //sensörü oku
    digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);


    //pulseIn kronometre dir paramatreleri sırayla hangi pini dinleyeyim ve elektrik varkenmi yoksa yokkenmi dinleyeyim yani benım kullanıdgım HIGH elektrık varken süreyi ölç
    long sure = pulseIn(ECHO_PIN, HIGH); //sesin gidip gelme süresi

    float gercek_mesafe = sure * 0.0343 / 2.0f;

    if(gercek_mesafe > 400 || gercek_mesafe == 0 ) gercek_mesafe = 400;

    //bu kısımdada ilk açılışta saçmalamaması için hata düzeltme eklıyoruz
    if(ilk_acilis) {
        onceki_mesafe = gercek_mesafe;
        ilk_acilis = false;
    }

    //hesaplama kısmı (fizik)
    float anlik_hiz = (onceki_mesafe - gercek_mesafe) / DT;
    if(anlik_hiz < 0) anlik_hiz = 0;

    float ham_mesafe = 1.0f - (gercek_mesafe / MAX_MESAFE);
    if(ham_mesafe < 0) ham_mesafe = 0.0f;
    float input_mesafe = ham_mesafe * ham_mesafe;

    float ham_hiz = anlik_hiz / MAX_HIZ;
    if(ham_hiz > 1.0f) ham_hiz = 1.0f;
    float input_hiz = ham_hiz * ham_hiz;

    Lnn_step(mesafe_noron, input_mesafe, DT);
    Lnn_step(hiz_noron, input_hiz, DT);

    float out_mesafe = lnn_sigmoid(mesafe_noron->noronlar[0].y);
    float out_hiz    = lnn_sigmoid(hiz_noron->noronlar[0].y);

    float guc_mesafe = (out_mesafe - 0.5f) * 2.0f;
    if(guc_mesafe < 0) guc_mesafe = 0; if(guc_mesafe > 1) guc_mesafe = 1.0f;

    float guc_hiz = (out_hiz - 0.5f) * 2.0f;
    if(guc_hiz < 0) guc_hiz = 0; if(guc_hiz > 1) guc_hiz = 1.0f;

    float panik_toplam = (guc_mesafe * 0.7f) + (guc_hiz * 0.3f);
    if(panik_toplam > 1.0f) panik_toplam = 1.0f;

    float panik_yuzde = panik_toplam * 100.0f;

    // Önce hepsini söndür (Temiz sayfa)
    digitalWrite(PIN_YESIL, LOW);
    digitalWrite(PIN_SARI, LOW);
    digitalWrite(PIN_KIRMIZI, LOW);

    // Panik durumuna göre yak
    if (panik_yuzde < 30) {
        // Durum Sakin -> YEŞİL
        digitalWrite(PIN_YESIL, HIGH);
    }
    else if (panik_yuzde >= 30 && panik_yuzde < 70) {
        // Durum Tedirgin -> SARI
        digitalWrite(PIN_SARI, HIGH);
    }
    else {
        // Durum Panik -> KIRMIZI
        digitalWrite(PIN_KIRMIZI, HIGH);
    }

    Serial.print("\nMesafe: "); Serial.print(gercek_mesafe);
    Serial.print(" Hız: "); Serial.print(anlik_hiz);
    Serial.print(" Panik: "); Serial.print(panik_toplam * 100);

    onceki_mesafe = gercek_mesafe;

    delay(100); //dt süresi kadar bekle
}
