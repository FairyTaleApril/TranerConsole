import cv2

def histequal1(frame):
    B, G, R = cv2.split(frame)
    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))
    clahe_B = clahe.apply(B)
    clahe_G = clahe.apply(G)
    clahe_R = clahe.apply(R)
    frame = cv2.merge((clahe_B, clahe_G, clahe_R))
    return frame
def histequal2(frame):
    B, G, R = cv2.split(frame)
    B = cv2.equalizeHist(B)
    G = cv2.equalizeHist(G)
    R = cv2.equalizeHist(R)
    frame = cv2.merge((B, G, R))
    return frame