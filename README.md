# Kaynak Kısıtlı IoT Cihazlarında Veri Bütünlüğü Korunumu

Bu depo, **YGA Grup 4** tarafından yürütülen "Kaynak Kısıtlı IoT Cihazlarında Veri Bütünlüğü Korunumu" projesine ait araştırma, simülasyon ve analiz dokümanlarını barındırmaktadır.

## Proje Hakkında
Nesnelerin İnterneti (IoT) ağlarında yer alan uç cihazların sınırlı donanım (işlemci, bellek ve enerji) kapasitelerine karşın güvenliğin sağlanması ve veri bütünlüğünün korunması kritik bir gereksinimdir. Bu proje, söz konusu kısıtlı cihazlar üzerinde kullanılabilecek, performans ve güvenlik dengesini en iyi şekilde sağlayan kriptografik (lightweight) algoritmaları ve çeşitli ağ mimarilerini araştırmayı hedefler.

## Odaklanılan Konular ve Dosya İçerikleri

Proje kapsamında araştırılan ve dökümante edilen başlıca çalışma başlıkları şunlardır:

- **Ağ Mimarileri:** IoT sistemlerinde verinin uç noktadan merkeze veya buluta taşınmasındaki güvenli ağ katmanları ve mimarileri.
- **Kriptografik Algoritmalar:** Kaynak donanımı zayıf olan cihazlara entegre edilebilecek kriptografik algoritmaların altyapısı.
- **Simüle Edilecek Algoritmalar:** Belirlenen şifreleme ve veri doğrulama algoritmalarının simüle edilerek performans testlerinin yapılması.
- **Bildiriler ve Sunumlar:** Proje çalışmasının sonuçlarını, ilerlemeyi ve analizleri gösteren makale taslakları, sunumlar ve proje dokümanları.

## Ekip
**YGA GRUP 4**

## Bu Hafta Hazirlanan Simulasyon Konfigleri

`omnetpp.ini` dosyasina bu algoritmalar icin ayri `Config` bloklari eklendi:

- WPAN: `WPAN_PRESENT`, `WPAN_SPECK`, `WPAN_CLEFIA`
- BLE / 6LoWPAN: `BLE_6LOWPAN_SPECK`, `BLE_6LOWPAN_ECC`, `BLE_6LOWPAN_CHACHA20`
- LPWAN: `LPWAN_SPECK`, `LPWAN_GIFT`, `LPWAN_SPONGENT`
- Yuksek performansli: `HP_ML_KEM`, `HP_ML_DSA`, `HP_RSA`, `HP_ECC`, `HP_SHA256`

## Senin Test Akisin

1. Once kripto profillerini uret:
   - `python kripto_test.py`
   - Bu komut `results/crypto_profiles.json` dosyasini uretir.
2. OMNeT++ icinde `omnetpp.ini` dosyasini ac.
3. Calistirmak istedigin `Config` secimini yap (ornek: `WPAN_SPECK`).
4. Simulasyonu calistir ve `results/` klasorune yazilan `.sca` ve `.vec` ciktilarini incele.
5. Tum configler icin ayni adimi tekrarlayip algoritma karsilastirma tablosu olustur.

## Not

- `kripto_test.py` dosyasinda RSA, ECC, ChaCha20 ve SHA-256 canli olculur.
- PRESENT, SPECK, CLEFIA, GIFT, SPONGENT, ML-KEM, ML-DSA icin baslangic profil degerleri referans olarak girilmistir; istersen bu degerleri kendi olcumlerinle dogrudan `omnetpp.ini` icinde guncelleyebilirsin.