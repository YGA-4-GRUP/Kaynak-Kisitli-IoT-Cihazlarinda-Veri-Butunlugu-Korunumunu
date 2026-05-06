import time
import ascon

def sifrele_ascon(veri_str):
    key = b"1234567890abcdef"
    nonce = b"1234567890abcdef"
    
    data = veri_str.encode()
    start_time = time.time()
    ciphertext = ascon.encrypt(key, nonce, b"", data, variant="Ascon-128")
    end_time = time.time()
    
    gecikme = end_time - start_time
    return len(ciphertext), gecikme

print("ASCON şifreleme motoru yüklendi")

if __name__=="__main__":
    testVerisi="A"*100
    boyut,sure=sifrele_ascon(testVerisi)
    print("\n---ANALİZ SONUÇLARI---")
    print(f"Orijinal: 100 Byte")
    print(f"Şifreli: {boyut} Byte")
    print(f"Gecikme: {sure:.10f} saniye")