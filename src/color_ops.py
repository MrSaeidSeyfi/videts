import cv2
import numpy as np

class ColorOps:
    @staticmethod
    def gray(frames):
        result = []
        for f in frames:
            g = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
            result.append(cv2.cvtColor(g, cv2.COLOR_GRAY2BGR))
        return result
    
    @staticmethod
    def invert(frames):
        return [cv2.bitwise_not(f) for f in frames]
    
    @staticmethod
    def brightness(frames, value):
        return [cv2.convertScaleAbs(f, alpha=1, beta=value) for f in frames]
    
    @staticmethod
    def contrast(frames, value):
        return [cv2.convertScaleAbs(f, alpha=value, beta=0) for f in frames]
    
    @staticmethod
    def hue_shift(frames, shift):
        result = []
        for f in frames:
            hsv = cv2.cvtColor(f, cv2.COLOR_BGR2HSV).astype(np.float32)
            hsv[:,:,0] = (hsv[:,:,0] + shift) % 180
            result.append(cv2.cvtColor(hsv.astype(np.uint8), cv2.COLOR_HSV2BGR))
        return result
    
    @staticmethod
    def saturation(frames, factor):
        result = []
        for f in frames:
            hsv = cv2.cvtColor(f, cv2.COLOR_BGR2HSV).astype(np.float32)
            hsv[:,:,1] = np.clip(hsv[:,:,1] * factor, 0, 255)
            result.append(cv2.cvtColor(hsv.astype(np.uint8), cv2.COLOR_HSV2BGR))
        return result
    
    @staticmethod
    def sepia(frames):
        result = []
        sepia_matrix = np.array([[0.272, 0.534, 0.131],
                                [0.349, 0.686, 0.168],
                                [0.393, 0.769, 0.189]])
        for f in frames:
            sepia_frame = cv2.transform(f, sepia_matrix)
            result.append(np.clip(sepia_frame, 0, 255).astype(np.uint8))
        return result
    
    @staticmethod
    def warm(frames, intensity):
        result = []
        for f in frames:
            b, g, r = cv2.split(f.astype(np.float32))
            r = np.clip(r + intensity, 0, 255)
            b = np.clip(b - intensity * 0.5, 0, 255)
            result.append(cv2.merge([b, g, r]).astype(np.uint8))
        return result
    
    @staticmethod
    def cool(frames, intensity):
        result = []
        for f in frames:
            b, g, r = cv2.split(f.astype(np.float32))
            b = np.clip(b + intensity, 0, 255)
            r = np.clip(r - intensity * 0.5, 0, 255)
            result.append(cv2.merge([b, g, r]).astype(np.uint8))
        return result
    
    @staticmethod
    def posterize(frames, levels):
        result = []
        step = 256 // levels
        for f in frames:
            quantized = (f // step) * step
            result.append(quantized.astype(np.uint8))
        return result



