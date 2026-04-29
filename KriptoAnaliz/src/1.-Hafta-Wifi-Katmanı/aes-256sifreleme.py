import time
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
from fontTools.misc.eexec import encrypt

def sifrele_aes256(veri_str):
    key=b'0123456789abcdef0123456789abcdef'
    
    cipher=Cipher(algorithms.AES(key),modes.ECB())
    encryptor=cipher.encryptor()
    
    padder=padding.PKCS7(128).padder()
    padded_data=padder.update(veri_str.encode())+padder.finalize()
    
    start_time=time.time()
    ciphertext=encryptor.update(padded_data)+encryptor.finalize()
    end_time=time.time()
    
    gecikme=end_time-start_time
    return len(ciphertext), gecikme

print ("AES-256 Şifreleme Motoru Yüklendi")

if __name__=="__main__":
        testVerisi = "A"*100
        boyut, sure=sifrele_aes256(testVerisi)
        print("\n--- ANALİZ SONUÇLARI ---")
        print(f"Orijinal: 100 Byte")
        print(f"Şifreli: {boyut} Byte")
        print(f"Gecikme: {sure:.10f} saniye")