import cv2
import numpy as np



def dark_channel(img, size = 15):
    r, g, b = cv2.split(img)
    #rgb图像分解成三个通道
    min_img = cv2.min(r, cv2.min(g, b))
    #求暗通道
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (size, size))
    #定义size*size的矩形（MORPH_RECT）
    dc_img = cv2.erode(min_img,kernel)
    #对输入图像用特定结构元素进行腐蚀操作，该结构元素确定腐蚀操作过程中的邻域的形状，各点像素值将被替换为对应邻域上的最小值
    return dc_img

#大气光的物理意义是对应图像中最明亮的区域，暗通道先验去雾求大气光值是对全局求
#取暗通道，然后选取最亮的前 0.1%个像素，将与其位置所对应的原图像素求平均
def get_atmo(img, percent = 0.001):
    mean_perpix = np.mean(img, axis = 2).reshape(-1)
    mean_perpix.sort()
    #倒序排序
    #将矩阵变成一行
    num=int(img.shape[0] * img.shape[1] * percent)
    #选取最亮的前 0.1%个像素
    mean_topper = mean_perpix[len(mean_perpix) - num - 1:]
    return np.mean(mean_topper)

#求取透射系数
def get_trans(img, atom, w = 0.95): #论文中为0.95
    x = img / atom
    t = 1 - w * dark_channel(x, 15)
    t0=0.1*np.ones(t.shape)
    t=np.maximum(t0,t)
    return t


"""
def guided_filter(p, i, r, e):
    :param p: input image
    :param i: guidance image
    :param r: radius
    :param e: regularization
    :return: filtering output q
    #1
    mean_I = cv2.boxFilter(i, cv2.CV_64F, (r, r))
    mean_p = cv2.boxFilter(p, cv2.CV_64F, (r, r))
    corr_I = cv2.boxFilter(i * i, cv2.CV_64F, (r, r))
    corr_Ip = cv2.boxFilter(i * p, cv2.CV_64F, (r, r))
    #2
    var_I = corr_I - mean_I * mean_I
    cov_Ip = corr_Ip - mean_I * mean_p
    #3
    a = cov_Ip / (var_I + e)
    b = mean_p - a * mean_I
    #4
    mean_a = cv2.boxFilter(a, cv2.CV_64F, (r, r))
    mean_b = cv2.boxFilter(b, cv2.CV_64F, (r, r))
    #5
    q = mean_a * i + mean_b
    return q
    """


def dehaze(im):
    #类型转换，转换成浮点型
    img = im.astype('float64') / 255
    img_gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY).astype('float64') / 255
    #转换成灰度图像
    atom = get_atmo(img)
    #获取大气光值
    trans = get_trans(img, atom)
    #获取投射率（假设每个窗口的透射率为常数）
    # trans_guided = guided_filter(trans, img_gray, 20, 0.0001)
    #trans_guided = cv2.max(trans_guided, 0.25)

    result = np.empty_like(img)
    #每次运行该程序时，都会得到不同的输出，它返回相同大小和形状的数组。
    for i in range(3):
        result[:, :, i] = (img[:, :, i] - atom) / trans + atom
    result=result*255
    result=result.astype('uint8')
    return result

    #如果图像数据类型是32F（32位浮点数）或64F（64位浮点数）
    #则imshow函数内部会自动将每个像素值乘以255并显示，即将原图像素值的范围由[0~1]映射到[0~255]
    #（注意：原图像素值必须要归一化）

"""cap = cv2.VideoCapture(1)
codec = cv2.VideoWriter_fourcc(*'MJPG')
fps = 20.0
frameSize = (1920, 1080)
out = cv2.VideoWriter('video.avi', codec, fps, frameSize)
print("按键Q-结束视频录制")
while(cap.isOpened()):
 ret, frame = cap.read()
 if ret==True:
  frame=dehaze(frame)
  out.write(frame)
  cv2.imshow('frame',frame)
  if cv2.waitKey(1) == ord('q'):
   break
 else:
  break
cap.release()
out.release()
origin=cv2.imread('4.jpg')
img=dehaze(origin)
cv2.imshow('img',img)
cv2.imwrite("4_dehazed.jpg",img)
cv2.waitKey(0)
cv2.destroyAllWindows()
"""
