from re import S
from tkinter import W
from turtle import circle
import numpy as np
import cv2
point=[]
def getBBox(event,x,y,flag,param):
    if event==cv2.EVENT_LBUTTONDOWN:
        point.append([x,y])
image=cv2.imread("photo1.png",0)
cv2.namedWindow("photo1.png", cv2.WINDOW_AUTOSIZE)
cv2.setMouseCallback("photo1",getBBox)
cv2.imshow('photo1.png', image)
point=np.array(point)
print(point)
x,y,h,W=cv2.boundingRect(point)
print(x,y,h,W)
cv2.polylines(image,[point],True,(0,0,255),2)
cv2.imshow("getBBox Window",image)
cv2.waitKey(0)
cropped=image[y:y+h,x:x+w]
cv2.imshow("cropped image",cropped)
cv2.waitkey(0)
cv2.destroyAllWindows()
edge=cv2.canny(cropped,100,200)
ret ,thresh = cv2.threshold(cropped, 127, 255, 0)
left_part=thresh[:, 0:thresh.shape[1]//3]
right_part= thresh[:,(thresh.shape[1]*2)//3:]
center_part= thresh[:,thresh.shape[1]//3:(thresh.shape[1]*2)//3]
cv2.imshow('left',left_part)
cv2.imshow('right',right_part)
cv2.imshow('center',center_part)
cv2.waitKey(0)
cv2.destroyAllWindows()
contours,hierarchy=cv2.findContours(left_part,2,1)
cnt1=contours[0]
contours,hierarchy=cv2.findContours(right_part,2,1)
cnt2=contours[0]
contours,hierarchy=cv2.findContours(center_part,2,1)
cnt3=contours[0]
ret1=cv2.matchShapes(cnt1,S,1,0,0)
if ret1<=.05:
    print("rectangle")
ret2=cv2.matchShapes(cnt2,W,1,0,0)
if ret2<=.05:
     print("star")
ret3=cv2.matchShapes(cnt3,circle,1,0,0)
if ret2<=.05:
    print("circle")
cv2.waitkey(0)
cv2.destroyAllWindows()