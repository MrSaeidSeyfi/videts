import cv2
import numpy as np

class CompositeOps:
    @staticmethod
    def stack_horizontal(frames1, frames2):
        result = []
        for f1, f2 in zip(frames1, frames2):
            f2_resized = cv2.resize(f2, (f1.shape[1], f1.shape[0]))
            result.append(np.hstack((f1, f2_resized)))
        return result
    
    @staticmethod
    def overlay(frames1, frames2, x, y):
        result = []
        for i, f1 in enumerate(frames1):
            f = f1.copy()
            if i < len(frames2):
                f2 = frames2[i]
                h, w = f2.shape[:2]
                f[y:y+h, x:x+w] = f2
            result.append(f)
        return result

