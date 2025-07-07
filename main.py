import cv2
import numpy as np
from tensorflow.keras.models import load_model  # type: ignore
import requests
import time

ESP8266_LCD = "http://192.168.137.100"

# https://teachablemachine.withgoogle.com/models/TzPBK8zz2/
model = load_model('keras_model.h5', compile=False)
with open('labels.txt', 'r') as f:
    class_names = [line.strip().split(' ', 1)[1] for line in f.readlines()]

cap = cv2.VideoCapture(0)
last_sent_time = 0
send_interval = 1.0
confidence_threshold = 80.0

while True:
    current_time = time.time()
    ret, frame = cap.read()
    if not ret:
        continue

    img = cv2.resize(frame, (224, 224))
    img = np.asarray(img, dtype=np.float32).reshape(1, 224, 224, 3)
    img = (img / 127.5) - 1

    prediction = model.predict(img)
    class_index = np.argmax(prediction)
    class_name = class_names[class_index]
    confidence = float(prediction[0][class_index]) * 100

    display_text = f"{class_name} ({confidence:.2f}%)"
    cv2.putText(frame, display_text, (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv2.imshow("Webcam View", frame)

    if current_time - last_sent_time > send_interval:
        try:
            if confidence >= confidence_threshold:
                if class_name == "Circle":
                    requests.get(f"{ESP8266_LCD}/Circle", timeout=0.5)
                    print("Circle detected, LED A activated")
                elif class_name == "Rectangle":
                    requests.get(f"{ESP8266_LCD}/Rectangle", timeout=0.5)
                    print("Rectangle detected, LED B activated")
                elif class_name == "Triangle":
                    requests.get(f"{ESP8266_LCD}/Triangle", timeout=0.5)
                    print("Triangle detected, LED C activated")
                elif class_name == "Background":
                    requests.get(f"{ESP8266_LCD}/Background", timeout=0.5)
                    print("Background detected, LED D activated")
            else:
                requests.get(f"{ESP8266_LCD}/Background", timeout=0.5)
                print("Background detected, LED D activated")
            last_sent_time = current_time
        except Exception as e:
            print("Error sending request:", e)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
