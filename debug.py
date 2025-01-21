import serial

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600

arduino = serial.Serial(SERIAL_PORT, BAUD_RATE)

try:
    while True:
        if arduino.in_waiting > 0:
            raw_data = arduino.readline().decode('utf-8').strip()
            print(f"Empfangen: {raw_data}")
except KeyboardInterrupt:
    print("Programm beendet.")
finally:
    arduino.close()
