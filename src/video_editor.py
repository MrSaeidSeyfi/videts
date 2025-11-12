from .video_io import VideoIO
from .geometric_ops import GeometricOps
from .temporal_ops import TemporalOps
from .color_ops import ColorOps
from .filter_ops import FilterOps
from .morph_ops import MorphOps
from .composite_ops import CompositeOps

class VideoEditor:
    def __init__(self, input_path):
        self.io = VideoIO(input_path)
        self.geom = GeometricOps()
        self.temp = TemporalOps()
        self.color = ColorOps()
        self.filt = FilterOps()
        self.morph = MorphOps()
        self.comp = CompositeOps()
    
    def execute(self, cmd, output, args):
        if cmd == 'crop':
            frames = self.io.read_all()
            frames = self.geom.crop(frames, int(args[0]), int(args[1]), int(args[2]), int(args[3]))
            self.io.write(output, frames)
        
        elif cmd == 'resize':
            frames = self.io.read_all()
            frames = self.geom.resize(frames, int(args[0]), int(args[1]))
            self.io.write(output, frames)
        
        elif cmd == 'rotate':
            frames = self.io.read_all()
            frames = self.geom.rotate(frames, float(args[0]), self.io.w, self.io.h)
            self.io.write(output, frames)
        
        elif cmd == 'flip':
            frames = self.io.read_all()
            d = int(args[0]) if args else 1
            frames = self.geom.flip(frames, d)
            self.io.write(output, frames)
        
        elif cmd == 'reverse':
            frames = self.io.read_all()
            frames = self.temp.reverse(frames)
            self.io.write(output, frames)
        
        elif cmd == 'speed':
            frames = self.io.read_all()
            s = float(args[0])
            if s >= 1:
                frames = self.temp.speed_up(frames, s)
                self.io.write(output, frames)
            else:
                self.io.write_fps(output, frames, int(self.io.fps * s))
        
        elif cmd == 'trim':
            frames = self.io.read_range(int(args[0]), int(args[1]))
            self.io.write(output, frames)
        
        elif cmd == 'gray':
            frames = self.io.read_all()
            frames = self.color.gray(frames)
            self.io.write(output, frames)
        
        elif cmd == 'invert':
            frames = self.io.read_all()
            frames = self.color.invert(frames)
            self.io.write(output, frames)
        
        elif cmd == 'bright':
            frames = self.io.read_all()
            v = int(args[0]) if args else 50
            frames = self.color.brightness(frames, v)
            self.io.write(output, frames)
        
        elif cmd == 'contrast':
            frames = self.io.read_all()
            v = float(args[0]) if args else 1.5
            frames = self.color.contrast(frames, v)
            self.io.write(output, frames)
        
        elif cmd == 'hue':
            frames = self.io.read_all()
            frames = self.color.hue_shift(frames, int(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'sat':
            frames = self.io.read_all()
            frames = self.color.saturation(frames, float(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'blur':
            frames = self.io.read_all()
            k = int(args[0]) if args else 15
            frames = self.filt.blur(frames, k)
            self.io.write(output, frames)
        
        elif cmd == 'sharpen':
            frames = self.io.read_all()
            frames = self.filt.sharpen(frames)
            self.io.write(output, frames)
        
        elif cmd == 'edge':
            frames = self.io.read_all()
            frames = self.filt.edge(frames)
            self.io.write(output, frames)
        
        elif cmd == 'denoise':
            frames = self.io.read_all()
            frames = self.filt.denoise(frames)
            self.io.write(output, frames)
        
        elif cmd == 'threshold':
            frames = self.io.read_all()
            t = int(args[0]) if args else 127
            frames = self.filt.threshold(frames, t)
            self.io.write(output, frames)
        
        elif cmd == 'dilate':
            frames = self.io.read_all()
            frames = self.morph.dilate(frames, int(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'erode':
            frames = self.io.read_all()
            frames = self.morph.erode(frames, int(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'open':
            frames = self.io.read_all()
            frames = self.morph.open(frames, int(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'close':
            frames = self.io.read_all()
            frames = self.morph.close(frames, int(args[0]))
            self.io.write(output, frames)
        
        elif cmd == 'stack':
            frames1 = self.io.read_all()
            io2 = VideoIO(args[0])
            frames2 = io2.read_all()
            frames = self.comp.stack_horizontal(frames1, frames2)
            self.io.write(output, frames)
        
        elif cmd == 'overlay':
            frames1 = self.io.read_all()
            io2 = VideoIO(args[0])
            frames2 = io2.read_all()
            x = int(args[1]) if len(args) > 1 else 0
            y = int(args[2]) if len(args) > 2 else 0
            frames = self.comp.overlay(frames1, frames2, x, y)
            self.io.write(output, frames)


