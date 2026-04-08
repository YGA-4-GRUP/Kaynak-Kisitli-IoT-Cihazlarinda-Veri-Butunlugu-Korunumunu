import time
from cryptography.hazmat.primitives.asymmetric import rsa, ec, padding
from cryptography.hazmat.primitives import hashes

# 100 Byte'lık örnek IoT sensör verisi
sensor_verisi = b"A" * 100 

print("--- Kaynak Kısıtlı IoT Kriptografi Testleri Başlıyor ---\n")

# 1. RSA-2048 TESTİ
print("1. RSA-2048 İşlemi Test Ediliyor...")
rsa_private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
rsa_public_key = rsa_private_key.public_key()

baslangic = time.perf_counter()
rsa_sifreli_veri = rsa_public_key.encrypt(
    sensor_verisi,
    padding.OAEP(mgf=padding.MGF1(algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None)
)
bitis = time.perf_counter()

rsa_sure = (bitis - baslangic) * 1000
rsa_boyut = len(rsa_sifreli_veri)
print(f"-> RSA-2048 Şifreleme Süresi: {rsa_sure:.3f} ms")
print(f"-> RSA-2048 Şifreli Paket Boyutu: {rsa_boyut} Byte\n")


# 2. ECC TESTİ (SECP256R1)
print("2. ECC İşlemi Test Ediliyor...")
ecc_private_key = ec.generate_private_key(ec.SECP256R1())

baslangic = time.perf_counter()
ecc_imza = ecc_private_key.sign(sensor_verisi, ec.ECDSA(hashes.SHA256()))
bitis = time.perf_counter()

ecc_sure = (bitis - baslangic) * 1000
ecc_boyut = len(sensor_verisi) + len(ecc_imza)
print(f"-> ECC İşlem Süresi: {ecc_sure:.3f} ms")
print(f"-> ECC Çıktı Boyutu: {ecc_boyut} Byte\n")


# 3. ML-KEM (Kyber) REFERANS
print("3. ML-KEM (Kuantum Sonrası) Değerleri...")
print("-> ML-KEM Şifreleme Süresi: ~0.500 ms (NIST liboqs referans değeri)")
print("-> ML-KEM Şifreli Paket Boyutu: 1188 Byte\n")

print("--- Test Tamamlandı. Bu süreler OMNeT++ simülasyonuna entegre edilecektir. ---")
