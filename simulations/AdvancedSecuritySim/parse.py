import pandas as pd

# Load the large CSV
df = pd.read_csv('analiz_sonuclari.csv', low_memory=False)

# Filter only the scalars we manually recorded
scalars = ['NumSent', 'NumReceived', 'NumDropped', 'NumActuated', 'NumSecurityVulnerabilities', 'MitM_Received', 'MitM_Manipulated']
df_filtered = df[(df['type'] == 'scalar') & (df['name'].isin(scalars))]

# Extract the base Run name (e.g., "RSA-0...", "ECC-0...")
# The run name usually starts with the Config name: "Sifresiz_Zafiyet_Testi", "RSA", "ECC", "ML-KEM"
df_filtered['Config'] = df_filtered['run'].apply(lambda x: x.split('-')[0])

# Since values are in string format but actually numbers, convert them
df_filtered['value'] = pd.to_numeric(df_filtered['value'], errors='coerce')

# Group by Config, Module, and Statistic Name
summary = df_filtered.groupby(['Config', 'name'])['value'].sum().unstack().fillna(0)

# Save to a clean CSV
summary.to_csv('ozet_tablo.csv')
print("Sadeleştirilmiş özet tablo başarıyla oluşturuldu: ozet_tablo.csv")
