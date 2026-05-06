import time
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305
import os

def sifrele_ChaCha20_Poly1305(veri_str):
    key=ChaCha20Poly1305.generate_key() #şifreleme anahtarı oluşturma
    chacha=ChaCha20Poly1305(key)
    
    nonce=os.urandom(12)
    
    data=veri_str.encode()
    
    start_time=time.time()
    ciphertext=chacha.encrypt(nonce, data, None)
    end_time=time.time()
    
    gecikme=end_time-start_time
    return len(ciphertext), gecikme

print ("ChaCha20-Poly1305 Şifreleme Motoru Yüklendi")

if __name__=="__main__":
        testVerisi = "A"*100
        boyut, sure=sifrele_ChaCha20_Poly1305(testVerisi)
        print("\n--- ANALİZ SONUÇLARI ---")
        print(f"Orijinal: 100 Byte")
        print(f"Şifreli: {boyut} Byte")
        print(f"Gecikme: {sure:.10f} saniye")