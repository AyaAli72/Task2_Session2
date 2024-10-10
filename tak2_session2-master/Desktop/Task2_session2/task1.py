import numpy as np
import cv2
img= cv2.imread('th1.jpg')
img1=cv2.cvtColor (img,cv2.COLOR_BGR2GRAY)
cv2.imshow("img",img1)
cv2.waitKey(0)
cv2.destroyAllWindows()