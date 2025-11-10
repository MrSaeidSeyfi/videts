import cv2

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

