import cv2

class VideoIO:
    def __init__(self, path):
        self.cap = cv2.VideoCapture(path)
        self.w = int(self.cap.get(3))
        self.h = int(self.cap.get(4))
        self.fps = int(self.cap.get(5))
        self.fc = int(self.cap.get(7))
        
    def read_all(self):
        frames = []
        while self.cap.isOpened():
            ret, frame = self.cap.read()
            if not ret: break
            frames.append(frame)
        return frames
    
    def read_range(self, start, end):
        self.cap.set(1, start)
        frames = []
        for _ in range(end - start):
            ret, frame = self.cap.read()
            if not ret: break
            frames.append(frame)
        return frames
    
    def write(self, path, frames):
        if not frames: return
        h, w = frames[0].shape[:2]
        writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*'mp4v'), self.fps, (w, h))
        for frame in frames:
            writer.write(frame)
        writer.release()
    
    def write_fps(self, path, frames, fps):
        if not frames: return
        h, w = frames[0].shape[:2]
        writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*'mp4v'), fps, (w, h))
        for frame in frames:
            writer.write(frame)
        writer.release()
    
    def extract_frame(self, frame_idx, output_path):
        self.cap.set(1, frame_idx)
        ret, frame = self.cap.read()
        if ret:
            cv2.imwrite(output_path, frame)
        return ret
    
    def get_frame_count(self):
        return self.fc
    
    def __del__(self):
        self.cap.release()



