import sys
import time

# OMNeT++'ın yerel kütüphaneleri görmesi için zorunlu yol
sys.path.append("/home/linux/.local/lib/python3.10/site-packages")

from cryptography.hazmat.primitives import padding
from simon import SimonCipher

def sifrele_simon_hafif(veri_str):
    # SIMON 64/128 konfigürasyonu
    key = 0x0123456789abcdef0123456789abcdef
    
    cipher = SimonCipher(key, key_size=128, block_size=64)
    padder = padding.PKCS7(64).padder()
    padded_data = padder.update(veri_str.encode()) + padder.finalize()
    
    ciphertext = bytearray()
    
    start_time = time.time()
    for i in range(0, len(padded_data), 8):
        block_int = int.from_bytes(padded_data[i:i+8], byteorder='big')
        encrypted_int = cipher.encrypt(block_int)
        ciphertext.extend(encrypted_int.to_bytes(8, byteorder='big'))
    end_time = time.time()
    
    gecikme = end_time - start_time
    return len(ciphertext), gecikme

print("SIMON-64/128 (Hafif Sıklet) Şifreleme Motoru Yüklendi")

if __name__ == "__main__":
    testVerisi = "A" * 100
    boyut, sure = sifrele_simon_hafif(testVerisi)
    print("\n--- BLE/6LoWPAN ANALİZ SONUÇLARI ---")
    print(f"Orijinal: 100 Byte")
    print(f"Şifreli: {boyut} Byte")
    print(f"Gecikme: {sure:.10f} saniye")