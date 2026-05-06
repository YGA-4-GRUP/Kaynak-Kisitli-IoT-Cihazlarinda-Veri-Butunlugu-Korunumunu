from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


ROOT = Path(__file__).resolve().parent.parent
RESULTS = ROOT / "results"
FIGURES = RESULTS / "figures"
FIGURES.mkdir(parents=True, exist_ok=True)

summary = pd.read_csv(RESULTS / "summary_results.csv")
attack = pd.read_csv(RESULTS / "attack_success.csv")
perf = pd.read_csv(RESULTS / "performance_metrics.csv")
raw = pd.read_csv(RESULTS / "raw_results.csv")


def save(fig_name):
    plt.tight_layout()
    plt.savefig(FIGURES / fig_name, dpi=150)
    plt.close()


plt.figure(figsize=(8, 4))
for net in sorted(summary["network"].unique()):
    subset = summary[summary["network"] == net]
    plt.bar([f"{net}-{a}" for a in subset["algorithm"]], subset["mitmSuccessRate"], label=net, alpha=0.7)
plt.ylabel("MITM Basari Orani")
plt.xticks(rotation=30, ha="right")
plt.legend()
save("mitm_success_rate.png")

plt.figure(figsize=(8, 4))
attack["combo"] = attack["network"] + "-" + attack["algorithm"]
plt.bar(attack["combo"], attack["firstSuccessTime"])
plt.ylabel("Ilk Basari Zamani (s)")
plt.xticks(rotation=30, ha="right")
save("first_success_time.png")

plt.figure(figsize=(8, 4))
for net in sorted(perf["network"].unique()):
    subset = perf[perf["network"] == net]
    plt.plot(subset["algorithm"], subset["avgE2eDelayMs"], marker="o", label=net)
plt.ylabel("Ortalama Gecikme (ms)")
plt.legend()
save("avg_latency.png")

plt.figure(figsize=(8, 4))
perf["combo"] = perf["network"] + "-" + perf["algorithm"]
plt.bar(perf["combo"], perf["totalEnergy"])
plt.ylabel("Toplam Enerji")
plt.xticks(rotation=30, ha="right")
save("energy_consumption.png")

plt.figure(figsize=(8, 4))
plt.bar(summary["algorithm"] + "-" + summary["network"], summary["avgOverhead"])
plt.ylabel("Paket Overhead (byte)")
plt.xticks(rotation=30, ha="right")
save("packet_overhead.png")

plt.figure(figsize=(8, 4))
attack_types = {
    "Replay": ((raw["isReplay"] == 1) & (raw["attackSucceeded"] == 1)).sum(),
    "Spoofing": ((raw["isSpoof"] == 1) & (raw["attackSucceeded"] == 1)).sum(),
    "PayloadManipulation": ((raw["isTampered"] == 1) & (raw["attackSucceeded"] == 1)).sum(),
}
plt.bar(list(attack_types.keys()), list(attack_types.values()))
plt.ylabel("Basarili Saldiri Sayisi")
save("attack_type_success.png")

score_df = summary.merge(
    perf[["network", "algorithm", "totalEnergy"]],
    on=["network", "algorithm"],
    how="inner"
)
for col in ["mitmSuccessRate", "avgE2eDelayMs", "totalEnergy", "avgOverhead"]:
    norm = (score_df[col] - score_df[col].min()) / (score_df[col].max() - score_df[col].min() + 1e-9)
    score_df[f"n_{col}"] = norm
score_df["securityPerformanceScore"] = 1.0 - (
    0.4 * score_df["n_mitmSuccessRate"] +
    0.2 * score_df["n_avgE2eDelayMs"] +
    0.2 * score_df["n_totalEnergy"] +
    0.2 * score_df["n_avgOverhead"]
)

plt.figure(figsize=(8, 4))
score_df["combo"] = score_df["network"] + "-" + score_df["algorithm"]
plt.bar(score_df["combo"], score_df["securityPerformanceScore"])
plt.ylabel("Genel Guvenlik-Performans Skoru")
plt.xticks(rotation=30, ha="right")
save("security_performance_score.png")
