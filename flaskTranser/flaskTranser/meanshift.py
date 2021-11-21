import numpy as np
import cv2


def getroi_hist(frame, x_rate = 0.5, y_rate = 0.5):
# set up the ROI for tracking
    height= frame.shape[0]
    width= frame.shape[1]
    #print(width,height)
    x0= int(width*x_rate)
    y0= int(height*y_rate)
    x, y, w, h = x0-40, y0-40, 80, 80 # simply hardcoded the values
    track_window = (x, y, w, h)
    roi = frame[y:y+h, x:x+w]
    #cv2.imwrite('roi.jpg', roi)
    hsv_roi = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
    #cv2.imwrite('hsv_roi.jpg', hsv_roi)
    mask = cv2.inRange(hsv_roi, np.array((0., 60.,32.)), np.array((180.,255.,255.)))
    #cv2.imwrite('mask.jpg',mask)
    #H色调:  0 — 180 S饱和度:  0 — 255 V亮度:  0 — 255
    roi_hist = cv2.calcHist([hsv_roi],[0],mask,[180],[0,180])
    #print(roi_hist)
    cv2.normalize(roi_hist,roi_hist,0,255,cv2.NORM_MINMAX)
    return roi_hist, track_window

def tracking(frame, roi_hist, track_window):
    # 归一化：原始图像，结果图像，映射到结果图像中的最小值，最大值，归一化类型
    # cv2.NORM_MINMAX对数组的所有值进行转化，使它们线性映射到最小值和最大值之间
    # 归一化后的图像便于显示，归一化后到0,255之间了
    # Setup the termination criteria, either 10 iteration or move by atleast 1 pt
    term_crit = ( cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1 )
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    dst = cv2.calcBackProject([hsv], [0], roi_hist, [0, 180], 1)
    #cv2.imwrite('dst.jpg', dst)
    # apply meanshift to get the new location
    ret, track_window = cv2.meanShift(dst, track_window, term_crit)
    # cv2.imshow("Backproject",dst)
    # Draw it on image
    x, y, w, h = track_window
    img2 = cv2.rectangle(frame, (x, y), (x + w, y + h), 255, 2)
    #cv2.imshow('img2', img2)
    return img2, track_window

'''
if __name__=='__main__':
    cap = cv2.VideoCapture(0)
    ret, frame = cap.read()
    roi_hist,track_window=getroi_hist(frame,0.5,0.5)
    while(1):
        ret, frame = cap.read()
        if ret==True:
            frame = tracking(frame,roi_hist,track_window)
            cv2.imshow('frame', frame)
            cv2.waitKey(30)
'''