import pandas as pd

df = pd.read_csv("tum_sonuclar.csv")

# Config değerleri
cfg = df[df["type"] == "config"][["run", "attrname", "attrvalue"]].copy()
cfg = cfg[cfg["attrname"].isin(["*.sensor.udp.procDelay", "*.sensor.app[0].messageLength"])]
cfg_w = cfg.pivot_table(index="run", columns="attrname", values="attrvalue", aggfunc="first").reset_index()

# configname (runattr içinden)
runattr = df[(df["type"] == "runattr") & (df["attrname"] == "configname")][["run", "attrvalue"]].rename(
    columns={"attrvalue": "Algoritma"}
)

# Alıcıdaki gerçek sonuçlar
sc = df[
    (df["type"] == "scalar")
    & (df["module"] == "KriptoAgi.gateway.app[0]")
    & (df["name"].isin(["packetReceived:count", "packetReceived:sum(packetBytes)"]))
][["run", "name", "value"]].copy()

sc["value"] = pd.to_numeric(sc["value"], errors="coerce")
sc_w = sc.pivot_table(index="run", columns="name", values="value", aggfunc="first").reset_index()

out = runattr.merge(cfg_w, on="run", how="left").merge(sc_w, on="run", how="left")

out = out.rename(columns={
    "*.sensor.udp.procDelay": "procDelay_ms",
    "*.sensor.app[0].messageLength": "messageLength_B",
    "packetReceived:count": "received_packets",
    "packetReceived:sum(packetBytes)": "received_bytes",
})

out = out[["Algoritma", "procDelay_ms", "messageLength_B", "received_packets", "received_bytes"]]
out.to_csv("ozet_tablo.csv", index=False, encoding="utf-8-sig")
print("Hazir: ozet_tablo.csv")