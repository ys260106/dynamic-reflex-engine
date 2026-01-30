# Gergino: ESP32 ile Sıvı Sinir Ağı (LNN) Denemesi 

Merhaba, ben Yusuf. Bilgisayar Mühendisliği öğrencisiyim. Bu proje, MIT'nin geliştirdiği **"Liquid Neural Networks" (Sıvı Sinir Ağları)** yapısının mantığını kavramak ve bir mikrodenetleyici (ESP32) üzerinde çalışıp çalışmayacağını görmek için yaptığım bir deneydir.

Robotun (Gergino) çok basit bir görevi var: **Panik yapmak.** Ama bunu klasik `if-else` kodlarıyla değil, zamanla değişen diferansiyel denklemlerle yapıyor.

---

##  Amaç Ne?
Normalde bir sensörden "engel yok" verisi gelince robotun anında durması gerekir. Ama canlılar böyle çalışmaz. Korku veya heyecan hemen sönmez, yavaş yavaş azalır.

Bu projede **"Leaky Integrate-and-Fire" (Sızdıran Kova)** nöron modelini kodlayarak, robotun olaylara anlık değil, zamana yayılmış tepkiler vermesini sağlamaya çalıştım.

---

##  Nasıl Çalışıyor? (Teknik Kısım)
Projenin kalbinde, nöronun "iç huzursuzluğunu" hesaplayan şu Euler formülü var:

$$Durum_{yeni} = Durum_{eski} + \Delta t \cdot \left( -\frac{Durum}{\tau} + Girdi \right)$$

* **Girdi:** Mesafe sensöründen gelen veri.
* **Tau ($\tau$):** Unutma hızı. (Bunu değiştirdiğimde robotun karakteri değişiyor; bazen hemen sakinleşiyor, bazen korkusu uzun sürüyor.)

Bu sayede sensör kablosunu anlık olarak çeksem bile, robot hemen "güvendeyim" demiyor, hafızasındaki korku yavaş yavaş sönüyor.

---

##  Kullandığım Malzemeler
* **Beyin:** ESP32 (İşlem gücü yüksek olduğu için seçtim)
* **Göz:** HC-SR04 Ultrasonik Sensör
* **Tepki:** 3 Adet LED (Yeşil, Sarı, Kırmızı)
* **Dil:** C++ / Arduino IDE

---

##  Mevcut Durum (V1) ve Eksikler
Bu proje henüz bitmiş bir ürün değil, bir prototiptir.

* **Çalışan Kısımlar:** Matematiksel nöron motoru çalışıyor. Mesafe verisi işlenip "panik seviyesi"ne dönüştürülüyor ve LED'lerle gösteriliyor.
* **Eksiklerim:** Henüz "Öğrenme" (Backpropagation) yok. Ağırlıkları (Weights) deneme-yanılma yoluyla elle girdim.
* **Hedef:** İleride bu nöronların hatasından ders çıkarıp kendi parametrelerini güncellemesini sağlamak istiyorum.

---

##  İletişim
Bu proje benim öğrenme sürecimin bir parçasıdır. Kodlarda acemice yerler olabilir, önerilerinize açığım!
**Yusuf Şahin**
