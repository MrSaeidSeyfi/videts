import cv2
import numpy as np

class GeometricOps:
    @staticmethod
    def crop(frames, x, y, w, h):
        return [f[y:y+h, x:x+w] for f in frames]
    
    @staticmethod
    def resize(frames, w, h):
        return [cv2.resize(f, (w, h)) for f in frames]
    
    @staticmethod
    def rotate(frames, angle, w, h):
        m = cv2.getRotationMatrix2D((w//2, h//2), angle, 1)
        return [cv2.warpAffine(f, m, (w, h)) for f in frames]
    
    @staticmethod
    def flip(frames, direction):
        return [cv2.flip(f, direction) for f in frames]
    
    @staticmethod
    def zoom(frames, factor, center_x=None, center_y=None):
        result = []
        for f in frames:
            h, w = f.shape[:2]
            if center_x is None:
                center_x = w // 2
            if center_y is None:
                center_y = h // 2
            new_w = int(w / factor)
            new_h = int(h / factor)
            x1 = max(0, center_x - new_w // 2)
            y1 = max(0, center_y - new_h // 2)
            x2 = min(w, x1 + new_w)
            y2 = min(h, y1 + new_h)
            cropped = f[y1:y2, x1:x2]
            result.append(cv2.resize(cropped, (w, h)))
        return result
    
    @staticmethod
    def pan(frames, dx, dy):
        result = []
        for f in frames:
            h, w = f.shape[:2]
            M = np.float32([[1, 0, dx], [0, 1, dy]])
            result.append(cv2.warpAffine(f, M, (w, h)))
        return result



