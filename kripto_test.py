import json
import time
from pathlib import Path

from cryptography.hazmat.primitives import hashes, padding
from cryptography.hazmat.primitives.asymmetric import ec, rsa
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

PAYLOAD = b"A" * 100
OUT_FILE = Path("results/crypto_profiles.json")


def _rounded(ms_value):
    return round(ms_value, 6)


def _build_profile(name, elapsed_ms, message_length, source):
    return name, {
        "proc_delay_ms": _rounded(elapsed_ms),
        "message_length_B": int(message_length),
        "source": source,
    }


def _mock_lightweight_block_cipher(expansion_bytes, speed_factor):
    key = b"0123456789abcdef"
    iv = b"abcdef9876543210"
    padder = padding.PKCS7(128).padder()
    padded = padder.update(PAYLOAD) + padder.finalize()
    cipher = Cipher(algorithms.AES(key), modes.CBC(iv)).encryptor()

    start = time.perf_counter()
    _ = cipher.update(padded) + cipher.finalize()
    elapsed_ms = (time.perf_counter() - start) * 1000

    adjusted_ms = max(elapsed_ms * speed_factor, 0.01)
    return adjusted_ms, len(PAYLOAD) + expansion_bytes


def rsa_benchmark():
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    public_key = private_key.public_key()
    start = time.perf_counter()
    encrypted = public_key.encrypt(
        PAYLOAD,
        padding.OAEP(
            mgf=padding.MGF1(algorithm=hashes.SHA256()),
            algorithm=hashes.SHA256(),
            label=None,
        ),
    )
    elapsed_ms = (time.perf_counter() - start) * 1000
    return elapsed_ms, len(encrypted)


def ecc_benchmark():
    private_key = ec.generate_private_key(ec.SECP256R1())
    start = time.perf_counter()
    signature = private_key.sign(PAYLOAD, ec.ECDSA(hashes.SHA256()))
    elapsed_ms = (time.perf_counter() - start) * 1000
    return elapsed_ms, len(PAYLOAD) + len(signature)


def chacha20_benchmark():
    key = b"0123456789abcdef0123456789abcdef"
    nonce = b"1234567890abcdef"
    cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None).encryptor()
    start = time.perf_counter()
    encrypted = cipher.update(PAYLOAD)
    elapsed_ms = (time.perf_counter() - start) * 1000
    return elapsed_ms, len(encrypted)


def sha256_benchmark():
    digest = hashes.Hash(hashes.SHA256())
    start = time.perf_counter()
    digest.update(PAYLOAD)
    hashed = digest.finalize()
    elapsed_ms = (time.perf_counter() - start) * 1000
    return elapsed_ms, len(PAYLOAD) + len(hashed)


def speck_benchmark():
    return _mock_lightweight_block_cipher(expansion_bytes=8, speed_factor=2.0)


def present_benchmark():
    return _mock_lightweight_block_cipher(expansion_bytes=8, speed_factor=2.2)


def clefia_benchmark():
    return _mock_lightweight_block_cipher(expansion_bytes=16, speed_factor=2.8)


def gift_benchmark():
    return _mock_lightweight_block_cipher(expansion_bytes=8, speed_factor=1.8)


def spongent_benchmark():
    return _mock_lightweight_block_cipher(expansion_bytes=20, speed_factor=1.4)


def ml_kem_reference():
    return 0.500, 1188


def ml_dsa_reference():
    return 0.850, 2420


def ble_reference():
    # BLE burada sifreleme algoritmasi yerine ag katmani profilini temsil eder.
    return 0.120, 112


def run_all_benchmarks():
    results = {}
    tests = [
        ("PRESENT", present_benchmark, "mock"),
        ("SPECK", speck_benchmark, "mock"),
        ("CLEFIA", clefia_benchmark, "mock"),
        ("BLE", ble_reference, "reference"),
        ("ChaCha20", chacha20_benchmark, "live"),
        ("GIFT", gift_benchmark, "mock"),
        ("SPONGENT", spongent_benchmark, "mock"),
        ("ML-KEM", ml_kem_reference, "reference"),
        ("ML-DSA", ml_dsa_reference, "reference"),
        ("RSA", rsa_benchmark, "live"),
        ("ECC", ecc_benchmark, "live"),
        ("SHA-256", sha256_benchmark, "live"),
    ]

    print("--- Kaynak Kisıtlı IoT Kriptografi Testleri Basliyor ---\n")
    for index, (name, fn, source) in enumerate(tests, start=1):
        elapsed_ms, msg_len = fn()
        key, profile = _build_profile(name, elapsed_ms, msg_len, source)
        results[key] = profile
        print(f"{index}. {name} testi")
        print(f"-> Islem suresi: {profile['proc_delay_ms']} ms")
        print(f"-> Mesaj boyutu: {profile['message_length_B']} Byte")
        print(f"-> Kaynak: {profile['source']}\n")
    return results


if __name__ == "__main__":
    OUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    profile_data = run_all_benchmarks()
    OUT_FILE.write_text(json.dumps(profile_data, indent=2), encoding="utf-8")
    print(f"Profil dosyasi olusturuldu: {OUT_FILE}")
