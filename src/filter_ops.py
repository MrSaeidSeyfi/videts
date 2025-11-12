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
    
    @staticmethod
    def emboss(frames):
        k = np.array([[-2, -1, 0], [-1, 1, 1], [0, 1, 2]])
        result = []
        for f in frames:
            gray = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
            embossed = cv2.filter2D(gray, -1, k)
            result.append(cv2.cvtColor(embossed, cv2.COLOR_GRAY2BGR))
        return result
    
    @staticmethod
    def cartoon(frames):
        result = []
        for f in frames:
            gray = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
            gray = cv2.medianBlur(gray, 5)
            edges = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 9, 9)
            color = cv2.bilateralFilter(f, 9, 300, 300)
            cartoon = cv2.bitwise_and(color, color, mask=edges)
            result.append(cartoon)
        return result
    
    @staticmethod
    def vignette(frames, intensity):
        result = []
        for f in frames:
            h, w = f.shape[:2]
            kernel_x = cv2.getGaussianKernel(w, w * intensity)
            kernel_y = cv2.getGaussianKernel(h, h * intensity)
            kernel = kernel_y * kernel_x.T
            mask = kernel / kernel.max()
            mask = cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR)
            result.append((f * mask).astype(np.uint8))
        return result
    
    @staticmethod
    def motion_blur(frames, size, angle):
        k = np.zeros((size, size))
        k[int((size-1)/2), :] = np.ones(size)
        k = cv2.warpAffine(k, cv2.getRotationMatrix2D((size/2-0.5, size/2-0.5), angle, 1.0), (size, size))
        k = k * (1.0 / np.sum(k))
        return [cv2.filter2D(f, -1, k) for f in frames]



