from serial import Serial

portName = '/dev/ttyACM1'
baudrate = 115200

ser = Serial(portName, baudrate, timeout=0.1)

val = []

while True:
    try:
        line = ser.readline().strip().decode()  # Satýrý oku, boþluklarý temizle ve deþifre et
        if line.startswith('lat'):  # Satýr 'lat' ile baþlýyorsa
            lat = float(line.split('=')[1])  # Deðerini al ve float'a çevir
            val.append(lat)
            print("Latitude:", lat)
        elif line.startswith('lng'):  # Satýr 'lng' ile baþlýyorsa
            lng = float(line.split('=')[1])  # Deðerini al ve float'a çevir
            print("Longitude:", lng)
        elif line.startswith('z'):  # Satýr 'z' ile baþlýyorsa
            z = line.split('=')[1].strip()  # Deðerini al
            print("Z:", z)
    except KeyboardInterrupt:
        break
    except Exception as e:
        print("Hata:", e)
        break

ser.close()
