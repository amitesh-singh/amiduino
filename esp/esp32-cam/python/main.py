import cv2
import urllib.request
import numpy as np

faceCascade= cv2.CascadeClassifier("./res/haarcascade_frontalface_default.xml")
eyeCascade=cv2.CascadeClassifier('./res/haarcascade_eye.xml')

url ="http://192.168.1.248:81/stream"
cap = cv2.VideoCapture(url)

while True:
    success, img = cap.read()
    if not success:
        print("unable to get stream")
        break
    
    faces = faceCascade.detectMultiScale(img,1.1,4)
    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),2)
        roi_gray = img[y: y +h, x: x+w]
        roi_color = img[y: y+h, x: x+ w]
        eyes = eyeCascade.detectMultiScale(roi_gray)
        for (ex, ey, ew, eh) in eyes:
            cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (255, 0, 0), 1)

    cv2.imshow("stream", img)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
cap.release()
