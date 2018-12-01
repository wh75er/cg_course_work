import numpy as np
import cv2
import time

filename = "video.avi"
frames_per_seconds = 1.0

cap = cv2.VideoCapture(0)
out = cv2.VideoWriter(filename, cv2.VideoWriter_fourcc(*'XVID'), frames_per_seconds, (640, 480))

cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)
cap.set(3, 640)
cap.set(4, 480)

for i in range(20): 
    ret, frame = cap.read()

exposureChanger = False

while(True): 
    if exposureChanger:
        cap.set(cv2.CAP_PROP_EXPOSURE, 0.4)
        exposureChanger = False
    else:
        cap.set(cv2.CAP_PROP_EXPOSURE, 0.1)
        exposureChanger = True

    time.sleep(.400)
    ret, frame = cap.read()
    cv2.imshow('Video', frame)
    out.write(frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
out.release()
cv2.destroyAllWindows()



#0. CV_CAP_PROP_POS_MSEC Current position of the video file in milliseconds.
#1. CV_CAP_PROP_POS_FRAMES 0-based index of the frame to be decoded/captured next.
#2. CV_CAP_PROP_POS_AVI_RATIO Relative position of the video file
#3. CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
#4. CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
#5. CV_CAP_PROP_FPS Frame rate.
#6. CV_CAP_PROP_FOURCC 4-character code of codec.
#7. CV_CAP_PROP_FRAME_COUNT Number of frames in the video file.
#8. CV_CAP_PROP_FORMAT Format of the Mat objects returned by retrieve() .
#9. CV_CAP_PROP_MODE Backend-specific value indicating the current capture mode.
#10. CV_CAP_PROP_BRIGHTNESS Brightness of the image (only for cameras).
#11. CV_CAP_PROP_CONTRAST Contrast of the image (only for cameras).
#12. CV_CAP_PROP_SATURATION Saturation of the image (only for cameras).
#13. CV_CAP_PROP_HUE Hue of the image (only for cameras).
#14. CV_CAP_PROP_GAIN Gain of the image (only for cameras).
#15. CV_CAP_PROP_EXPOSURE Exposure (only for cameras).
#16. CV_CAP_PROP_CONVERT_RGB Boolean flags indicating whether images should be converted to RGB.
#17. CV_CAP_PROP_WHITE_BALANCE Currently unsupported
#18. CV_CAP_PROP_RECTIFICATION Rectification flag for stereo cameras (note: only supported by DC1394 v 2.x backend currently)
