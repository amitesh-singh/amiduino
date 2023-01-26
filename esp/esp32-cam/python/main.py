import cv2
import urllib.request
import numpy as np

url ="http://192.168.1.248:81/stream"
cap = cv2.VideoCapture(url)

while True:
    success, img = cap.read()
    if not success:
        print("unable to get stream")
        break

    cv2.imshow("stream", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
cap.release()
