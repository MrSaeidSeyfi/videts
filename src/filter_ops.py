import cv2
import numpy as np

class FilterOps:
    @staticmethod
    def blur(frames, kernel):
        return [cv2.GaussianBlur(f, (kernel, kernel), 0) for f in frames]
    
    @staticmethod
    def sharpen(frames):
        k = np.array([[-1,-1,-1],[-1,9,-1],[-1,-1,-1]])
        return [cv2.filter2D(f, -1, k) for f in frames]
    
    @staticmethod
    def edge(frames):
        result = []
        for f in frames:
            e = cv2.Canny(f, 100, 200)
            result.append(cv2.cvtColor(e, cv2.COLOR_GRAY2BGR))
        return result
    
    @staticmethod
    def denoise(frames):
        return [cv2.fastNlMeansDenoisingColored(f, None, 10, 10, 7, 21) for f in frames]
    
    @staticmethod
    def threshold(frames, thresh):
        result = []
        for f in frames:
            g = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
            _, th = cv2.threshold(g, thresh, 255, cv2.THRESH_BINARY)
            result.append(cv2.cvtColor(th, cv2.COLOR_GRAY2BGR))
        return result

