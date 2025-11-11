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


