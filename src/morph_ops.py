import cv2
import numpy as np

class MorphOps:
    @staticmethod
    def dilate(frames, kernel_size):
        k = np.ones((kernel_size, kernel_size), np.uint8)
        return [cv2.dilate(f, k) for f in frames]
    
    @staticmethod
    def erode(frames, kernel_size):
        k = np.ones((kernel_size, kernel_size), np.uint8)
        return [cv2.erode(f, k) for f in frames]
    
    @staticmethod
    def open(frames, kernel_size):
        k = np.ones((kernel_size, kernel_size), np.uint8)
        return [cv2.morphologyEx(f, cv2.MORPH_OPEN, k) for f in frames]
    
    @staticmethod
    def close(frames, kernel_size):
        k = np.ones((kernel_size, kernel_size), np.uint8)
        return [cv2.morphologyEx(f, cv2.MORPH_CLOSE, k) for f in frames]


