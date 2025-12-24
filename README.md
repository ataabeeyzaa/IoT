# 🌱 IoT Destekli Akıllı Saksı Sistemi (ESP32 & Blynk)

Bu proje, bitkilerin nem ihtiyacını sensörler aracılığıyla takip eden, toprağın kuruluk oranına göre otomatik sulama yapan ve **Blynk** platformu üzerinden uzaktan kontrol edilebilen bir IoT (Nesnelerin İnterneti) sistemidir.

## 🚀 Projenin Amacı ve Özellikleri

Bitkilerin yanlış veya yetersiz sulanmasını önlemek amacıyla geliştirilen bu sistem, otonom çalışabilme yeteneğine sahiptir.

* **📊 Gerçek Zamanlı Takip:** Toprak nem oranı anlık olarak mobil uygulama üzerinden izlenebilir.
* **💧 Otomatik Sulama:** Toprak nemi belirlenen eşik değerin (%10) altına düştüğünde sistem otomatik olarak su motorunu çalıştırır.
* **📱 Uzaktan Kontrol:** Blynk uygulaması üzerinden sistemin otomasyon modu açılıp kapatılabilir.
* **🔄 Tam Senkronizasyon:** Cihaz üzerindeki fiziksel butona basıldığında, mobil uygulamadaki durum da anında güncellenir.
* **⚠️ Güvenlik:** Sistem pasif moda alındığında motor çalışması fiziksel olarak engellenir.

## 🛠 Kullanılan Donanım

| Bileşen | Açıklama |
| :--- | :--- |
| **Mikrodenetleyici** | ESP32 Development Board |
| **Sensör** | Kapasitif/Resistif Toprak Nem Sensörü |
| **Kontrolcü** | 1 Kanal 5V Röle Modülü (Su motoru kontrolü için) |
| **Giriş Birimi** | Push Button (Fiziksel kontrol) |
| **Güç Kaynağı** | 5V DC Adaptör / Powerbank |
| **Su Pompası** | 5V Dalgıç Mini Su Pompası |



## 🔌 Devre Bağlantı Şeması (Pinout)

Kod içerisindeki pin tanımlamaları aşağıdaki gibidir:

* **Toprak Nem Sensörü:** `GPIO 34` (Analog Giriş)
* **Su Motoru (Röle):** `GPIO 26`
* **Fiziksel Buton:** `GPIO 14` (Input Pullup)

## 💻 Yazılım ve Teknolojiler

* **Dil:** C++ (Arduino IDE)
* **Platform:** Blynk IoT Cloud
* **Kütüphaneler:** `WiFi.h`, `BlynkSimpleEsp32.h`

### Algoritma Mantığı
1.  Sistem her saniye sensörden veri okur.
2.  Okunan analog veri (0-4095 arası), kalibrasyon değerlerine (Kuru: 3000, Islak: 1200) göre 0-100 arası yüzdeliğe çevrilir.
3.  Eğer `Otomasyon Modu` aktifse ve `Nem < %10` ise röle tetiklenir ve sulama başlar.
4.  Butona basıldığında veya uygulamadan komut geldiğinde sistem durumu (Aktif/Pasif) değişir.

## ⚙️ Kurulum ve Kullanım

1.  Bu repodaki `.ino` uzantılı kodu Arduino IDE ile açın.
2.  Gerekli kütüphaneleri (`Blynk`) Library Manager üzerinden yükleyin.
3.  Kod içerisindeki şu alanları kendi bilgilerinizle doldurun:
    ```cpp
    #define BLYNK_TEMPLATE_ID "SENIN_TEMPLATE_ID"
    #define BLYNK_AUTH_TOKEN "SENIN_TOKEN"
    char ssid[] = "WIFI_ADINIZ";
    char pass[] = "WIFI_SIFRENIZ";
    ```
4.  Kodu ESP32 kartına yükleyin.
5.  Blynk mobil uygulamasından **V0 (Buton)** ve **V1 (Gösterge)** datastream'lerini tanımlayın.

---
*Geliştirici: [Beyza ATA/ataabeeyzaa]*
