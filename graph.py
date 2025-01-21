import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from datetime import datetime

# Datei mit Dezibelwerten
DECIBEL_FILE = "/home/maciej/LF7/decibel_values.txt"

# Daten lesen
timestamps = []
decibel_levels = []

try:
    with open(DECIBEL_FILE, "r") as f:
        for line in f:
            if line.strip():
                value, timestamp = line.strip().split(",")
                decibel_levels.append(float(value))
                timestamps.append(datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S"))
except FileNotFoundError:
    print("Keine Dezibel-Daten gefunden.")
    exit()

# Graph erstellen
if timestamps and decibel_levels:
    plt.figure(figsize=(10, 6))
    plt.plot(timestamps, decibel_levels, label="Dezibel-Werte", marker='o')

    # Formatierung der Zeitachse
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))  # Format: Stunden:Minuten:Sekunden
    plt.gca().xaxis.set_major_locator(mdates.AutoDateLocator())  # Automatische Lokatoren

    # Achsenbeschriftungen und Titel
    plt.title("Dezibel-Werte über Zeit")
    plt.xlabel("Zeit (HH:MM:SS)")
    plt.ylabel("Dezibel")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    # Graph anzeigen
    plt.show()
else:
    print("Keine Daten zum Anzeigen.")
