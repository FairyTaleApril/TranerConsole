#!/usr/bin/env python
# -*- coding: utf-8 -*-


from flask import Flask, render_template, request, Response
from Camera import VideoCamera, HazedeVideoCamera, DefogVideoCamera, RecognitionVideoCamera

app = Flask(__name__)


@app.route('/')
def index():
	return render_template('TranserConsole.html')


def gen(camera):
	while True:
		frame = camera.get_frame()  # jpeg格式
		yield (b'--frame\r\n'
			   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')   # 返回frame


@app.route('/video_feed0')
def video_feed0():
    return Response(gen(VideoCamera()), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/video_feed1')
def video_feed1():
    return Response(gen(HazedeVideoCamera()), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/video_feed2')
def video_feed2():
    return Response(gen(DefogVideoCamera()), mimetype='multipart/x-mixed-replace; boundary=frame')
    
@app.route('/video_feed3')
def video_feed3():
    return Response(gen(RecognitionVideoCamera()), mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
	app.run(host = '0.0.0.0', port = 8000)
    