import serial
import subprocess
from datetime import datetime

# Serielle Verbindung zum Arduino herstellen
SERIAL_PORT = '/dev/ttyACM0'  # Passe den Port an dein System an
BAUD_RATE = 9600
arduino = serial.Serial(SERIAL_PORT, BAUD_RATE)

# Datei zur Speicherung von Dezibelwerten
DECIBEL_FILE = "/home/maciej/LF7/decibel_values.txt"

# Skript-Pfad
GRAPH_SCRIPT = "/home/maciej/LF7/graph.py"

# Prozess-Variable
collecting_data = False

try:
    print("Warte auf Button-Drücke...")
    while True:
        if arduino.in_waiting > 0:
            raw_data = arduino.readline().decode('utf-8').strip()
            print(f"Empfangenes Signal: {raw_data}")  # Debugging

            if raw_data == "START_SCRIPT":
                if not collecting_data:
                    print("Beginne mit der Datensammlung...")
                    collecting_data = True

            elif raw_data == "STOP_SCRIPT":
                print("Stop-Button erkannt!")  # Debugging
                if collecting_data:
                    print("Beende die Datensammlung und öffne den Graphen...")
                    collecting_data = False
                    
                    # Graph-Skript aufrufen
                    try:
                        process = subprocess.Popen(
                            ["python3", GRAPH_SCRIPT],
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE
                        )
                        stdout, stderr = process.communicate()
                        print("Graph-Skript-Ausgabe:")
                        print(stdout.decode())
                        print(stderr.decode())
                    except Exception as e:
                        print(f"Fehler beim Öffnen des Graphen: {e}")

            elif raw_data.startswith("DECIBEL:") and collecting_data:
                decibel_value = raw_data.split(":")[1].strip()
                with open(DECIBEL_FILE, "a") as f:
                    f.write(f"{decibel_value},{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")

except KeyboardInterrupt:
    print("Programm beendet.")
finally:
    arduino.close()
