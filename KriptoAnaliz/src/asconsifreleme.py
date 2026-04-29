import ascon
import time

def sifrele_ascon(veri_str):
    key = b'0123456789abcdef'
    nonce = b'0123456789abcdef'
    ad = b'header'
    
    start_time = time.time()
    ciphertext = ascon.encrypt(key, nonce, ad, veri_str.encode(), "Ascon-128")
    end_time = time.time()
    
    gecikme = end_time - start_time
    return len(ciphertext), gecikme

print("ASCON Şifreleme Motoru Yukklendi.")

if __name__=="__main__":
        testVerisi = "A"*100
        boyut, sure=sifrele_ascon(testVerisi)
        print("\n--- ANALİZ SONUÇLARI ---")
        print(f"Orijinal: 100 Byte")
        print(f"Şifreli: {boyut} Byte")
        print(f"Gecikme: {sure:.10f} saniye")