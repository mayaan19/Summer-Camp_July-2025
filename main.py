import cv2
import numpy as np
from tensorflow.keras.models import load_model  # type: ignore
import requests
import time

ESP8266_LCD = "http://192.168.137.120"
ESP8266_LED = "http://192.168.137.182"
ESP8266_IPS = [ESP8266_LCD, ESP8266_LED]

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
        path = class_name if confidence >= confidence_threshold else "Background"
        for ip in ESP8266_IPS:
            try:
                requests.get(f"{ip}/{path}", timeout=0.5)
                print(f"Sent '{path}' to {ip}")
            except Exception as e:
                print(f"Error sending to {ip}:", e)
        last_sent_time = current_time

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
