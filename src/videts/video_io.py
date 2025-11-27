import cv2


class VideoIO:
    """Simple abstraction to read/write video files or operate on an in-memory list of frames.

    Args:
        source: path to a video file (str) or a list of frames (list-like). If None, you
            can set frames later.
        fps: when providing an in-memory frames list you may pass an integer fps (default 30).
    """
    def __init__(self, source=None, fps=30):
        self._frames = None
        self.cap = None
        self.fps = fps
        self.w = None
        self.h = None
        self.fc = 0

        if source is None:
            return

        if isinstance(source, (list, tuple)):
            # initialize from an in-memory list of frames
            self._frames = list(source)
            if self._frames:
                self.h, self.w = self._frames[0].shape[:2]
                self.fc = len(self._frames)
        elif isinstance(source, str):
            # initialize from a video file path
            self.cap = cv2.VideoCapture(source)
            self.w = int(self.cap.get(3))
            self.h = int(self.cap.get(4))
            self.fps = int(self.cap.get(5))
            self.fc = int(self.cap.get(7))
        else:
            raise TypeError('source must be a file path or a list of frames')
        
    def read_all(self):
        """Return list of frames. If VideoIO was created with an in-memory list, return it.

        Otherwise read all frames from the bound VideoCapture.
        """
        if self._frames is not None:
            return list(self._frames)

        frames = []
        if self.cap is None:
            return frames

        while self.cap.isOpened():
            ret, frame = self.cap.read()
            if not ret:
                break
            frames.append(frame)
        return frames
    
    def read_range(self, start, end):
        """Return frames in [start, end) from either the in-memory frames or the capture."""
        if self._frames is not None:
            return list(self._frames[start:end])

        if self.cap is None:
            return []

        self.cap.set(1, start)
        frames = []
        for _ in range(max(0, end - start)):
            ret, frame = self.cap.read()
            if not ret:
                break
            frames.append(frame)
        return frames
    
    def write(self, path, frames):
        """Write a list of frames to `path` as an mp4 file. If frames is empty or None, do nothing."""
        if not frames:
            return
        h, w = frames[0].shape[:2]
        writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*'mp4v'), self.fps, (w, h))
        for frame in frames:
            writer.write(frame)
        writer.release()
    
    def write_fps(self, path, frames, fps):
        """Write frames to file using provided fps."""
        if not frames:
            return
        h, w = frames[0].shape[:2]
        writer = cv2.VideoWriter(path, cv2.VideoWriter_fourcc(*'mp4v'), fps, (w, h))
        for frame in frames:
            writer.write(frame)
        writer.release()
    
    def extract_frame(self, frame_idx, output_path):
        """Extract a single frame by index to an image file and return True on success."""
        if self._frames is not None:
            if 0 <= frame_idx < len(self._frames):
                cv2.imwrite(output_path, self._frames[frame_idx])
                return True
            return False

        if self.cap is None:
            return False

        self.cap.set(1, frame_idx)
        ret, frame = self.cap.read()
        if ret:
            cv2.imwrite(output_path, frame)
        return ret
    
    def get_frame_count(self):
        return self.fc
    
    def __del__(self):
        if self.cap is not None:
            self.cap.release()
