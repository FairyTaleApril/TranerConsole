#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv2
from dehaze import dehaze
from defog import histequal1
from meanshift import getroi_hist, tracking


cap = cv2.VideoCapture(0)


class VideoCamera(object):
    def __init__(self):
        self.video = cap
    
    def __del__(self):
        self.video.release()
    
    def get_frame(self):
        success, image = self.video.read()
        ret, jpeg = cv2.imencode('.jpg', image)
        return jpeg.tobytes()


class HazedeVideoCamera(object):
    def __init__(self):
        self.video = cap
    
    def __del__(self):
        self.video.release()
    
    def get_frame(self):
        success, image = self.video.read()
        image = dehaze(image)
        ret, jpg = cv2.imencode('.jpg', image)
        return jpg.tobytes()
    

class DefogVideoCamera(object):
    def __init__(self):
        self.video = cap
    
    def __del__(self):
        self.video.release()
    
    def get_frame(self):
        success, image = self.video.read()
        image = histequal1(image)
        ret, jpg = cv2.imencode('.jpg', image)
        return jpg.tobytes()
    

class RecognitionVideoCamera(object):
    def __init__(self):
        self.video = cap
    
    def __del__(self):
        self.video.release()
    
    def get_frame(self):
        success, image = self.video.read()
        global track_window
        global roi_hist
        
        with open("../pos.txt", "r") as f:
            data = f.read()
        if data != "0":
            factor = data.split('#')
            roi_hist, track_window = getroi_hist(image, float(factor[1]), float(factor[2]))
            rewrite_pos()
        
        image, track_window = tracking(image, roi_hist, track_window)
        ret, jpg = cv2.imencode('.jpg', image)
        return jpg.tobytes()


def rewrite_pos():
    with open('../pos.txt', "w", encoding = "utf-8") as f:
        f.truncate(0)
    with open('../pos.txt', "w", encoding = "utf-8") as f:
        f.write("0")