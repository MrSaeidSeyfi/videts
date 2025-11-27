class TemporalOps:
    @staticmethod
    def reverse(frames):
        return frames[::-1]
    
    @staticmethod
    def speed_up(frames, factor):
        step = int(factor)
        return frames[::step]
    
    @staticmethod
    def slow_down(frames):
        return frames
    
    @staticmethod
    def freeze(frames, frame_idx, duration):
        if not frames or frame_idx >= len(frames):
            return frames
        freeze_frame = frames[frame_idx]
        result = frames[:frame_idx]
        for _ in range(duration):
            result.append(freeze_frame.copy())
        result.extend(frames[frame_idx:])
        return result
    
    @staticmethod
    def loop(frames, count):
        return frames * count
    
    @staticmethod
    def duplicate(frames, factor):
        result = []
        for f in frames:
            for _ in range(factor):
                result.append(f.copy())
        return result
