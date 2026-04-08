#!/bin/bash
# Endustriyel Guvenlik Simulasyonu Icin Derleme ve Calistirma Scripti

echo "Mesaj dosyalari hazirlaniyor..."
opp_msgc *.msg 2>/dev/null || echo "Hata yok."

echo "Makefile olusturuluyor..."
# INET Framework ile baglanti kuruluyo
opp_makemake -f --deep -O out -KINET_PROJ=/home/uzai/yga/omnet/inet -DINET_IMPORT -I\$\(INET_PROJ\)/src -L\$\(INET_PROJ\)/src -lINET

echo "Proje derleniyor..."
make -j$(nproc)

echo "Derleme basarili. Sifresiz_Zafiyet_Testi simulasyonu baslatiliyor..."
./AdvancedSecuritySim -u Cmdenv -c Sifresiz_Zafiyet_Testi
echo "-----------------------------------"

echo "RSA Sifreleme simulasyonu baslatiliyor..."
./AdvancedSecuritySim -u Cmdenv -c RSA
echo "-----------------------------------"

echo "ECC Sifreleme simulasyonu baslatiliyor..."
./AdvancedSecuritySim -u Cmdenv -c ECC
echo "-----------------------------------"

echo "ML-KEM Post-Kuantum simulasyonu baslatiliyor..."
./AdvancedSecuritySim -u Cmdenv -c ML-KEM
echo "-----------------------------------"

echo "Tum simulasyonlar Tamamlandi. Sonuclar 'results' dizininde."
