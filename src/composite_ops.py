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
    
    @staticmethod
    def fade_in(frames, duration):
        result = []
        fade_frames = min(duration, len(frames))
        for i, f in enumerate(frames):
            alpha = min(1.0, i / fade_frames) if fade_frames > 0 else 1.0
            result.append((f * alpha).astype(np.uint8))
        return result
    
    @staticmethod
    def fade_out(frames, duration):
        result = []
        fade_frames = min(duration, len(frames))
        start_idx = len(frames) - fade_frames
        for i, f in enumerate(frames):
            if i >= start_idx:
                alpha = 1.0 - (i - start_idx) / fade_frames if fade_frames > 0 else 0.0
            else:
                alpha = 1.0
            result.append((f * alpha).astype(np.uint8))
        return result
    
    @staticmethod
    def crossfade(frames1, frames2, duration):
        result = list(frames1[:-duration])
        fade_len = min(duration, len(frames1), len(frames2))
        for i in range(fade_len):
            idx1 = len(frames1) - fade_len + i
            idx2 = i
            if idx1 < len(frames1) and idx2 < len(frames2):
                alpha = i / fade_len
                f1 = frames1[idx1].astype(np.float32)
                f2 = frames2[idx2].astype(np.float32)
                if f1.shape != f2.shape:
                    f2 = cv2.resize(f2, (f1.shape[1], f1.shape[0]))
                blended = (f1 * (1 - alpha) + f2 * alpha).astype(np.uint8)
                result.append(blended)
        result.extend(frames2[fade_len:])
        return result
    
    @staticmethod
    def stack_vertical(frames1, frames2):
        result = []
        for f1, f2 in zip(frames1, frames2):
            f2_resized = cv2.resize(f2, (f1.shape[1], f1.shape[0]))
            result.append(np.vstack((f1, f2_resized)))
        return result
    
    @staticmethod
    def text_overlay(frames, text, x, y, scale, color, thickness):
        result = []
        for f in frames:
            frame = f.copy()
            cv2.putText(frame, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, scale, color, thickness)
            result.append(frame)
        return result



