
#define BLYNK_TEMPLATE_ID "TMPL61pSAxk_O"
#define BLYNK_TEMPLATE_NAME "AkilliSaksi"
#define BLYNK_AUTH_TOKEN "BURAYA_KENDI_TOKENINIZI_YAZIN" 
// (Gerçek token'ı sildim, yerine uyarı yazdım)

// --- KÜTÜPHANELER ---
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// --- WIFI AYARLARI ---
char ssid[] = "WIFI_ADINIZ";       // Kullanıcı kendi adını yazsın
char pass[] = "WIFI_SIFRENIZ";     // Kullanıcı kendi şifresini yazsın



// --- PIN TANIMLAMALARI ---
const int toprakNemPin = 34;       // Sensör
const int suMotoruPin = 26;        // Motor
const int fizikselButonPin = 14;   // Fiziksel Buton (D14)

// --- DEĞİŞKENLER ---
int otomasyonAktif = 1;      // 1: Açık, 0: Kapalı
const int kuruDeger = 3000;  // Havada (kuru) okunan
const int islakDeger = 1200; // Suda (ıslak) okunan

// --- AYAR: SULAMA EŞİĞİ ---
const int sulamaSiniri = 10; // Nem %10'un altına düşerse sula

BlynkTimer timer;

// --- 1. TELEFONDAN KOMUT GELİRSE (V0) ---
BLYNK_WRITE(V0) {
  otomasyonAktif = param.asInt(); 
  durumGuncellemesi();           
}

// --- 2. DURUM BİLGİLENDİRME ---
void durumGuncellemesi() {
  if (otomasyonAktif == 1) {
    Serial.println(">> DURUM: Sistem AKTIF (Otomatik Mod Devrede)");
  } else {
    Serial.println(">> DURUM: Sistem PASIF (Motor Durduruldu)");
    digitalWrite(suMotoruPin, HIGH); // Motoru hemen kapat
  }
}

// --- 3. FİZİKSEL BUTONU DİNLE ---
void fizikselButonuDinle() {
  if (digitalRead(fizikselButonPin) == LOW) {
    delay(50); // Titreşim engelleme
    if (digitalRead(fizikselButonPin) == LOW) {
      Serial.println(">> FIZIKSEL TUSA BASILDI!");
      
      otomasyonAktif = !otomasyonAktif; // Durumu tersine çevir
      
      // Telefondaki tuşu güncelle (Senkronizasyon)
      Blynk.virtualWrite(V0, otomasyonAktif);
      
      durumGuncellemesi();
      
      // Elini çekene kadar bekle
      while(digitalRead(fizikselButonPin) == LOW) {
        delay(10);
      }
    }
  }
}

// --- 4. SENSÖR OKUMA VE KARAR ---
void sensoruOkuVeGonder() {
  int hamDeger = analogRead(toprakNemPin);
  int nemYuzdesi = map(hamDeger, kuruDeger, islakDeger, 0, 100);
  nemYuzdesi = constrain(nemYuzdesi, 0, 100);

  // Nemi Telefona Gönder
  Blynk.virtualWrite(V1, nemYuzdesi); 
  Serial.print("Nem: %"); Serial.println(nemYuzdesi);

  // --- KARAR MEKANİZMASI (Sınır %10) ---
  if (otomasyonAktif == 1) {
    // BURASI DEĞİŞTİ: Nem %10'un altındaysa çalışır
    if (nemYuzdesi < sulamaSiniri) {
      digitalWrite(suMotoruPin, LOW); // Motor AÇIK
      Serial.println("!!! Toprak COK Kuru (%10 alti) -> Sulama Yapiliyor !!!");
    } else {
      digitalWrite(suMotoruPin, HIGH); // Motor KAPALI
    }
  } 
  else {
    digitalWrite(suMotoruPin, HIGH); // Sistem pasifse motoru kapalı tut
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(suMotoruPin, OUTPUT);
  digitalWrite(suMotoruPin, HIGH); 

  pinMode(fizikselButonPin, INPUT_PULLUP); 

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  Blynk.virtualWrite(V0, 1); // Başlangıçta aktif olsun
  
  timer.setInterval(1000L, sensoruOkuVeGonder);
}

void loop() {
  Blynk.run();
  timer.run();
  fizikselButonuDinle(); 
}