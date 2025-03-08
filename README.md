# **GK ESP32 Multi-Network Analyzer**

Zaawansowany system do analizy sieci **WiFi, Bluetooth, GSM** na ESP32. Umożliwia **skanowanie, analizę i wizualizację** wyników na wyświetlaczu OLED oraz interakcję poprzez menu.

## **Schemat projektu**
![obraz](https://github.com/user-attachments/assets/0481ad97-a6c6-403d-b4a4-c44463156d97)

## **Prosta dokumentacja**
![obraz](https://github.com/user-attachments/assets/210eeadd-8400-4fd6-be7f-457120a29374)

## **Zdjęcie Menu**
![obraz](https://github.com/user-attachments/assets/9a5aa7e9-0114-4ac9-b0bb-d30b6867dba4)

## **Zdjęcie przykładowej funkcji - szczegółów sieci wifi**
![obraz](https://github.com/user-attachments/assets/923cb680-407f-42a7-97b5-48644e4b79eb)

**Reszta zdjęć i opisów w dokumentacji**

## **Funkcjonalności**
- **Skanowanie sieci WiFi** – identyfikacja dostępnych sieci, siła sygnału, zabezpieczenia. System pobiera listę dostępnych sieci WiFi i wyświetla je w menu OLED, umożliwiając użytkownikowi wybór oraz podgląd szczegółów, takich jak siła sygnału, typ zabezpieczeń i kanał.
- **Analiza GSM (SIM800L)** – system nawiązuje komunikację z modułem SIM800L i odczytuje informacje o dostępnych stacjach BTS, sile sygnału oraz operatorze. Użytkownik może przejrzeć dostępne sieci oraz sprawdzić parametry połączenia komórkowego.
- **Bluetooth Scanner** – skanowanie urządzeń BLE i BR/EDR. Użytkownik może przeglądać listę znalezionych urządzeń, sprawdzać ich adresy MAC, nazwy oraz dostępne usługi.
- **Obsługa menu OLED** – interaktywne menu pozwala na łatwe przełączanie się pomiędzy poszczególnymi funkcjami systemu. Wyniki są prezentowane w sposób czytelny, a dłuższe nazwy są przewijane automatycznie.

## **Sprzęt użyty w projekcie**
- **ESP32** – główny kontroler zarządzający wszystkimi modułami.
- **SIM800L** – moduł GSM, odpowiedzialny za analizę sieci komórkowych.
- **OLED 128x64** – wyświetlacz prezentujący wyniki skanowania oraz nawigację w menu.

## **Protokoły używane w projekcie**
- **WiFi (802.11 b/g/n)** – do skanowania dostępnych sieci oraz ich parametrów.
- **Bluetooth (BLE, BR/EDR)** – do wykrywania i analizy urządzeń Bluetooth.
- **GSM/AT Commands** – do komunikacji z modułem SIM800L i analizy sygnału sieci komórkowej.
- **I2C** – do komunikacji z wyświetlaczem OLED.
- **UART** – do sterowania modułem SIM800L i odbierania wyników analizy.

## **Opis działania**
1. **Interfejs użytkownika** – system uruchamia ekran startowy, po czym przechodzi do menu głównego.
2. **Skanowanie sieci** – użytkownik może wybrać tryb skanowania (WiFi, Bluetooth, GSM). System wykonuje zapytania do odpowiednich modułów, zbiera dane i wyświetla je na OLED.
3. **Przeglądanie wyników** – po zakończeniu skanowania użytkownik może przejrzeć listę wyników i uzyskać szczegółowe informacje na temat wybranej sieci lub urządzenia.
4. **Opcje dodatkowe** – system umożliwia testowanie połączenia GSM, przeglądanie operatorów i siły sygnału.

## **Jak uruchomić?**
### **1. Klonuj repozytorium**
```bash
git clone https://github.com/GrzegorzK121/GK-ESP32-Multi-Network-Analyzer.git
cd GK-ESP32-Multi-Network-Analyzer
```
### **2. Wyczyść poprzednią kompilację**
```bash
idf.py clean
```
### **3. Skonfiguruj i zbuduj projekt**
```bash
idf.py build
```
### **4. Wgraj na ESP32**
```bash
idf.py flash monitor
```
### **5. Korzystaj z menu OLED**
Po uruchomieniu ESP32 wyświetlacz OLED umożliwia interakcję z systemem. Możesz przeglądać dostępne sieci, analizować sygnały i poruszać się po menu.

## **Licencja**
Projekt jest dostępny na licencji **MIT**, co oznacza, że możesz go dowolnie używać i modyfikować.

## **Współpraca**
Jeśli masz pomysły na rozwój projektu, zapraszam do **forkowania i tworzenia pull requestów**.

