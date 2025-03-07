# **GK ESP32 Multi-Network Analyzer**

Zaawansowany system do analizy sieci **WiFi, Bluetooth, GSM** na ESP32. Umożliwia **skanowanie, analizę i wizualizację** wyników na wyświetlaczu OLED oraz interakcję poprzez menu.

## **Funkcjonalności**
- **Skanowanie sieci WiFi** – identyfikacja dostępnych sieci, siła sygnału, zabezpieczenia  
- **Analiza GSM (SIM800L)** – sprawdzanie BTS, poziomu sygnału, dostępnych operatorów  
- **Bluetooth Scanner** – skanowanie urządzeń BLE i BR/EDR, lista i szczegóły usług  
- **Obsługa menu OLED** – nawigacja po wynikach skanowania i szczegółowe widoki  

## **Sprzęt użyty w projekcie**
- **ESP32** – główny kontroler  
- **SIM800L** – moduł GSM  
- **OLED 128x64** – interfejs użytkownika  

## **Jak uruchomić?**
### **1. Klonuj repozytorium**
```bash
git clone https://github.com/twoj-user/twoje-repo.git
cd twoje-repo
```
### **2. Skonfiguruj i wgraj na ESP32**
Użyj **PlatformIO** lub **ESP-IDF** do skompilowania projektu.  
```bash
idf.py build flash monitor
```
### **3. Korzystaj z menu OLED**
Po uruchomieniu ESP32 wyświetlacz OLED umożliwia interakcję z systemem. Możesz przeglądać dostępne sieci, analizować sygnały i poruszać się po menu.

## **Licencja**
Projekt jest dostępny na licencji **MIT**, co oznacza, że możesz go dowolnie używać i modyfikować.

## **Współpraca**
Jeśli masz pomysły na rozwój projektu, zapraszam do **forkowania i tworzenia pull requestów**.

