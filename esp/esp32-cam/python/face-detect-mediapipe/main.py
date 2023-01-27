import cv2
import mediapipe as mp
import numpy as np



mp_facedetector = mp.solutions.face_detection
mp_draw = mp.solutions.drawing_utils

url ="http://192.168.1.248:81/stream"
cap = cv2.VideoCapture(url)

with mp_facedetector.FaceDetection(min_detection_confidence=0.7) as face_detection:
    while cap.isOpened():    
        success, img = cap.read()

        if not success:
            print("unable to read stream")
            break

        #img = cv2.cvtColor(img, cv2.COLOR_GRAY2RGB)

        results = face_detection.process(img)
        #img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

        if results.detections:
            for id, detection in enumerate(results.detections):
                mp_draw.draw_detection(img, detection)
                print(id, detection)

                bBox = detection.location_data.relative_bounding_box

                h, w, c = img.shape

                boundBox = int(bBox.xmin * w), int(bBox.ymin * h), int(bBox.width * w), int(bBox.height * h)

                cv2.putText(img, f'{int(detection.score[0]*100)}%', (boundBox[0], boundBox[1] - 20), cv2.FONT_HERSHEY_SIMPLEX, 2, (0,255,0), 2)
        
        
            cv2.imshow('Face Detection', img)
              # 27 is escape key
            if cv2.waitKey(5) & 0xFF == 27:
                break

cap.release()

