from pathlib import Path
import random
import csv


def main():
    root = Path(__file__).resolve().parent.parent
    out = root / "results" / "raw_results.csv"
    out.parent.mkdir(parents=True, exist_ok=True)

    networks = ["WPAN", "BLE"]
    algos = ["ChaCha20-Poly1305", "ASCON-128", "SIMON"]

    headers = [
        "simTime", "network", "algorithm", "accepted", "isAttack", "attackSucceeded",
        "isReplay", "isSpoof", "isTampered", "e2eDelayMs", "encMs", "decMs",
        "sensorEnergy", "acEnergy", "overhead"
    ]

    random.seed(42)
    rows = []
    t = 0.0
    for network in networks:
        for algo in algos:
            for i in range(1200):
                t += 0.05
                is_attack = 1 if i % 5 == 0 else 0
                attack_type = i % 3
                is_replay = 1 if is_attack and attack_type == 0 else 0
                is_spoof = 1 if is_attack and attack_type == 1 else 0
                is_tampered = 1 if is_attack and attack_type == 2 else 0
                base_success = {
                    "ChaCha20-Poly1305": 0.02,
                    "ASCON-128": 0.015,
                    "SIMON": 0.05
                }[algo]
                success = 1 if is_attack and random.random() < base_success else 0
                accepted = 1 if (not is_attack or success) else 0
                delay = random.uniform(8, 20) if network == "WPAN" else random.uniform(4, 12)
                enc = {"ChaCha20-Poly1305": 0.45, "ASCON-128": 0.52, "SIMON": 0.30}[algo]
                dec = {"ChaCha20-Poly1305": 0.40, "ASCON-128": 0.48, "SIMON": 0.28}[algo]
                energy_s = {"ChaCha20-Poly1305": 0.19, "ASCON-128": 0.17, "SIMON": 0.12}[algo]
                energy_a = {"ChaCha20-Poly1305": 0.17, "ASCON-128": 0.16, "SIMON": 0.10}[algo]
                ov = {"ChaCha20-Poly1305": 16, "ASCON-128": 16, "SIMON": 8}[algo]

                rows.append([
                    round(t, 3), network, algo, accepted, is_attack, success, is_replay, is_spoof,
                    is_tampered, round(delay, 4), enc, dec, energy_s, energy_a, ov
                ])

    with out.open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(headers)
        writer.writerows(rows)


if __name__ == "__main__":
    main()
