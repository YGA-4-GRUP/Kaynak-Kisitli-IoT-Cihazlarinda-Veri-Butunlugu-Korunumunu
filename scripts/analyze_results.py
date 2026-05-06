import json
from pathlib import Path

import pandas as pd


ROOT = Path(__file__).resolve().parent.parent
RESULTS = ROOT / "results"
RAW = RESULTS / "raw_results.csv"
SUMMARY = RESULTS / "summary_results.csv"
ATTACK = RESULTS / "attack_success.csv"
PERF = RESULTS / "performance_metrics.csv"
REPORT = RESULTS / "analysis_report.json"


def safe_mean(series):
    return float(series.mean()) if len(series) else 0.0


def to_records(df, n=1):
    if df.empty:
        return []
    return df.head(n).to_dict(orient="records")


def main():
    if not RAW.exists():
        raise FileNotFoundError(f"Missing raw results: {RAW}")

    df = pd.read_csv(RAW)
    group_cols = ["network", "algorithm"]
    grouped = df.groupby(group_cols, dropna=False)

    summary_rows = []
    attack_rows = []
    perf_rows = []

    for (network, algorithm), g in grouped:
        attacks = g[g["isAttack"] == 1]
        success_rate = safe_mean(attacks["attackSucceeded"]) if len(attacks) else 0.0
        first_success_series = g[g["attackSucceeded"] == 1]["simTime"]
        first_success = float(first_success_series.min()) if len(first_success_series) else -1.0

        summary_rows.append(
            {
                "network": network,
                "algorithm": algorithm,
                "accepted": int((g["accepted"] == 1).sum()),
                "rejected": int((g["accepted"] == 0).sum()),
                "mitmSuccessRate": success_rate,
                "avgE2eDelayMs": safe_mean(g["e2eDelayMs"]),
                "avgEncMs": safe_mean(g["encMs"]),
                "avgDecMs": safe_mean(g["decMs"]),
                "totalSensorEnergy": float(g["sensorEnergy"].sum()),
                "totalAcEnergy": float(g["acEnergy"].sum()),
                "avgOverhead": safe_mean(g["overhead"]),
            }
        )

        attack_rows.append(
            {
                "network": network,
                "algorithm": algorithm,
                "attackAttempts": int((g["isAttack"] == 1).sum()),
                "attackSuccess": int((g["attackSucceeded"] == 1).sum()),
                "mitmSuccessRate": success_rate,
                "firstSuccessTime": first_success,
                "replayRejected": int(((g["isReplay"] == 1) & (g["accepted"] == 0)).sum()),
                "spoofRejected": int(((g["isSpoof"] == 1) & (g["accepted"] == 0)).sum()),
                "tamperRejected": int(((g["isTampered"] == 1) & (g["accepted"] == 0)).sum()),
            }
        )

        perf_rows.append(
            {
                "network": network,
                "algorithm": algorithm,
                "avgE2eDelayMs": safe_mean(g["e2eDelayMs"]),
                "avgEncMs": safe_mean(g["encMs"]),
                "avgDecMs": safe_mean(g["decMs"]),
                "totalEnergy": float(g["sensorEnergy"].sum() + g["acEnergy"].sum()),
                "avgOverhead": safe_mean(g["overhead"]),
            }
        )

    summary_df = pd.DataFrame(summary_rows).sort_values(group_cols)
    attack_df = pd.DataFrame(attack_rows).sort_values(group_cols)
    perf_df = pd.DataFrame(perf_rows).sort_values(group_cols)

    summary_df.to_csv(SUMMARY, index=False)
    attack_df.to_csv(ATTACK, index=False)
    perf_df.to_csv(PERF, index=False)

    score_df = summary_df.merge(perf_df[["network", "algorithm", "totalEnergy"]], on=["network", "algorithm"], how="inner")
    for col in ["mitmSuccessRate", "avgE2eDelayMs", "totalEnergy", "avgOverhead"]:
        norm = (score_df[col] - score_df[col].min()) / (score_df[col].max() - score_df[col].min() + 1e-9)
        score_df[f"n_{col}"] = norm
    score_df["securityPerformanceScore"] = 1.0 - (
        0.4 * score_df["n_mitmSuccessRate"] +
        0.2 * score_df["n_avgE2eDelayMs"] +
        0.2 * score_df["n_totalEnergy"] +
        0.2 * score_df["n_avgOverhead"]
    )

    auth_cmp = df[df["isAttack"] == 1].groupby("authEnabled", dropna=False)["attackSucceeded"].mean()
    nonce_seq_off = df[
        (df["isAttack"] == 1) &
        (df["nonceCheckEnabled"] == 0) &
        (df["sequenceCheckEnabled"] == 0)
    ]["attackSucceeded"]
    nonce_seq_on = df[
        (df["isAttack"] == 1) &
        (df["nonceCheckEnabled"] == 1) &
        (df["sequenceCheckEnabled"] == 1)
    ]["attackSucceeded"]

    replay_reject = df[df["isReplay"] == 1].groupby(["nonceCheckEnabled", "sequenceCheckEnabled"], dropna=False)["accepted"].mean().reset_index()
    replay_reject["replayBlockedRate"] = 1.0 - replay_reject["accepted"]

    tamper_reject = df[df["isTampered"] == 1].groupby("algorithm", dropna=False)["accepted"].mean().reset_index()
    tamper_reject["tamperRejectRate"] = 1.0 - tamper_reject["accepted"]

    report = {
        "q1_wpan_ble_mitm_resilience": summary_df.groupby("network")["mitmSuccessRate"].mean().to_dict(),
        "q2_lowest_mitm_success_algorithm": to_records(attack_df.sort_values("mitmSuccessRate"), 1),
        "q3_lowest_latency_algorithm": to_records(perf_df.sort_values("avgE2eDelayMs"), 1),
        "q4_lowest_energy_algorithm": to_records(perf_df.sort_values("totalEnergy"), 1),
        "q5_best_security_performance_balance": to_records(score_df.sort_values("securityPerformanceScore", ascending=False), 1),
        "q6_replay_block_conditions": replay_reject[["nonceCheckEnabled", "sequenceCheckEnabled", "replayBlockedRate"]].to_dict(orient="records"),
        "q7_payload_manipulation_rejection_by_algorithm": tamper_reject[["algorithm", "tamperRejectRate"]].to_dict(orient="records"),
        "q8_auth_disabled_mitm_change": {
            "authOffSuccessRate": float(auth_cmp.get(0, 0.0)),
            "authOnSuccessRate": float(auth_cmp.get(1, 0.0)),
            "delta": float(auth_cmp.get(0, 0.0) - auth_cmp.get(1, 0.0)),
        },
        "q9_nonce_sequence_disabled_mitm_change": {
            "nonceSeqOffSuccessRate": safe_mean(nonce_seq_off),
            "nonceSeqOnSuccessRate": safe_mean(nonce_seq_on),
            "delta": safe_mean(nonce_seq_off) - safe_mean(nonce_seq_on),
        },
    }
    REPORT.write_text(json.dumps(report, indent=2), encoding="utf-8")


if __name__ == "__main__":
    main()
