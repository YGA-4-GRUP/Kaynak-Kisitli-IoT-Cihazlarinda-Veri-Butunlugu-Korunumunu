import time
from Crypto.Cipher import ChaCha20
from Crypto.Random import get_random_bytes

def sifrele_chacha20(veri_str):
    key = get_random_bytes(32) #random 32 byte
    nonce = get_random_bytes(12) # random 12 Byte
    
    data = veri_str.encode()
    cipher = ChaCha20.new(key=key, nonce=nonce)
    
    start_time = time.time()
    ciphertext = cipher.encrypt(data)
    end_time = time.time()
    
    gecikme = end_time - start_time
    return len(ciphertext), gecikme

print("ChaCha20 şifreleme motoru yüklendi")

if __name__=="__main__":
    testVerisi = "A" * 100
    boyut, sure = sifrele_chacha20(testVerisi)
    print("\n---ANALİZ SONUÇLARI---")
    print(f"Orijinal: 100 Byte")
    print(f"Şifreli: {boyut} Byte")
    print(f"Gecikme: {sure:.10f} saniye")
