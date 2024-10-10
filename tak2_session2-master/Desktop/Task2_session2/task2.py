import cv2
import numpy as np
def nothing(x): 
    pass 
cv2.namedWindow('windows')
cv2.createTrackbar('LOwRED','windows',0,155,nothing)
cv2.createTrackbar('HIGHRED','windows',155,255,nothing)
cv2.createTrackbar('LOwGREEN','windows',0,155,nothing)
cv2.createTrackbar('HIGHGREEN','windows',155,255,nothing)
cv2.createTrackbar('LOwBLUE','windows',0,155,nothing)
cv2.createTrackbar('HIGHBLUE','windows',155,255,nothing) 

cap= cv2.VideoCapture(0)
while True:
    low_red =cv2.getTrackbarPos('LOwRED','windows')
    high_red =cv2.getTrackbarPos('HIGHRED','windows')
    low_green =cv2.getTrackbarPos('LOwGREEN','windows')  
    high_green =cv2.getTrackbarPos('HIGHGREEN','windows')  
    low_blue =cv2.getTrackbarPos('LOwBLUE','windows')
    high_blue =cv2.getTrackbarPos('HIGHBLUE','windows')
    ret,Fram= cap.read()
    mask=cv2.inRange(Fram,(low_red,low_green,low_blue),(high_red ,high_green,high_blue))   
    cv2.imshow('mask',mask)
    cv2.imshow('windows',Fram)
    if cv2.waitKey(1) == 27:
      break  
cv2.destroyAllWindows()
cap.release() 