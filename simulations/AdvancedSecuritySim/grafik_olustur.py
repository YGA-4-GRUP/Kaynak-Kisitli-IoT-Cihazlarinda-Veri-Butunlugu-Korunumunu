import matplotlib
matplotlib.use('Agg')  # WSL'de GUI yok, dosyaya kaydet
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

# ============================================================
# HAM VERİLER (ozet_tablo.csv'den alındı)
# ============================================================
senaryolar = ['Şifresiz\n(Zafiyet)', 'RSA', 'ECC', 'ML-KEM']
renkler = ['#e74c3c', '#f39c12', '#27ae60', '#2980b9']

mitm_basarim     = [3, 0, 0, 0]   # MitM_Manipulated
guvenlik_ihlali  = [2, 0, 0, 0]   # NumSecurityVulnerabilities
gonderilen       = [6, 1, 1, 2]   # NumSent
alınan           = [2, 0, 1, 2]   # NumReceived

# NumReceived / NumSent oranı (paket teslimat oranı %)
teslimat_orani = [round((a/g)*100, 1) if g > 0 else 0
                  for a, g in zip(alınan, gonderilen)]

fig, axs = plt.subplots(1, 3, figsize=(16, 6))
fig.suptitle('AdvancedSecuritySim — Şifreleme Senaryoları Karşılaştırması\n(IEEE 802.11n Wi-Fi WLAN simülasyonu)',
             fontsize=14, fontweight='bold', y=1.02)

# ── Grafik 1: MitM Başarım Sayısı ──────────────────────────
ax1 = axs[0]
bars1 = ax1.bar(senaryolar, mitm_basarim, color=renkler, edgecolor='white',
                linewidth=1.2, width=0.55)
ax1.set_title('MitM Saldırı Başarımı\n(Paket Ele Geçirme)', fontsize=12, fontweight='bold')
ax1.set_ylabel('Manipüle Edilen Paket Sayısı', fontsize=10)
ax1.set_ylim(0, max(mitm_basarim) + 1.5)
for bar, val in zip(bars1, mitm_basarim):
    ax1.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.05,
             str(int(val)), ha='center', va='bottom', fontsize=12, fontweight='bold')
ax1.axhline(y=0, color='grey', linewidth=0.5)
ax1.set_facecolor('#f8f9fa')
ax1.grid(axis='y', alpha=0.4)

# ── Grafik 2: Güvenlik İhlali (Actuator Kandırma) ──────────
ax2 = axs[1]
bars2 = ax2.bar(senaryolar, guvenlik_ihlali, color=renkler, edgecolor='white',
                linewidth=1.2, width=0.55)
ax2.set_title('Veri Bütünlüğü İhlali\n(Actuator Kandırma Sayısı)', fontsize=12, fontweight='bold')
ax2.set_ylabel('Güvenlik İhlali Sayısı', fontsize=10)
ax2.set_ylim(0, max(guvenlik_ihlali) + 1.5)
for bar, val in zip(bars2, guvenlik_ihlali):
    label = f'⚠ {int(val)}' if val > 0 else '✓ 0'
    color = '#c0392b' if val > 0 else '#27ae60'
    ax2.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.05,
             label, ha='center', va='bottom', fontsize=12,
             fontweight='bold', color=color)
ax2.set_facecolor('#f8f9fa')
ax2.grid(axis='y', alpha=0.4)

# ── Grafik 3: Paket Teslimat Oranı (%) ─────────────────────
ax3 = axs[2]
bars3 = ax3.bar(senaryolar, teslimat_orani, color=renkler, edgecolor='white',
                linewidth=1.2, width=0.55)
ax3.set_title('Paket Teslimat Başarı Oranı\n(%)', fontsize=12, fontweight='bold')
ax3.set_ylabel('Teslimat Oranı (%)', fontsize=10)
ax3.set_ylim(0, 115)
for bar, val in zip(bars3, teslimat_orani):
    ax3.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 1,
             f'%{val}', ha='center', va='bottom', fontsize=12, fontweight='bold')
ax3.axhline(y=100, color='grey', linestyle='--', linewidth=0.8, alpha=0.6)
ax3.set_facecolor('#f8f9fa')
ax3.grid(axis='y', alpha=0.4)

# ── Son Dokunuşlar ──────────────────────────────────────────
yorum = (
    "Şifresiz iletimde MitM saldırısı veri bütünlüğünü tamamen bozdu.\n"
    "RSA şifrelemesi güvenli ancak paket teslimatı gerçekleşmedi (CPU aşırı yük).\n"
    "ECC ve ML-KEM: He güvenli hem de iletim başarılı."
)
fig.text(0.5, -0.04, yorum, ha='center', fontsize=10, color='#555555',
         style='italic', wrap=True)

plt.tight_layout()
plt.savefig('simulasyon_grafikleri.png', dpi=180, bbox_inches='tight',
            facecolor='white')
print("Grafik başarıyla kaydedildi: simulasyon_grafikleri.png")
