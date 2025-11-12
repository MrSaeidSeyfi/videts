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
        
        elif cmd == 'zoom':
            frames = self.io.read_all()
            factor = float(args[0]) if args else 1.5
            cx = int(args[1]) if len(args) > 1 else None
            cy = int(args[2]) if len(args) > 2 else None
            frames = self.geom.zoom(frames, factor, cx, cy)
            self.io.write(output, frames)
        
        elif cmd == 'pan':
            frames = self.io.read_all()
            dx = int(args[0]) if args else 0
            dy = int(args[1]) if len(args) > 1 else 0
            frames = self.geom.pan(frames, dx, dy)
            self.io.write(output, frames)
        
        elif cmd == 'freeze':
            frames = self.io.read_all()
            frame_idx = int(args[0]) if args else 0
            duration = int(args[1]) if len(args) > 1 else 30
            frames = self.temp.freeze(frames, frame_idx, duration)
            self.io.write(output, frames)
        
        elif cmd == 'loop':
            frames = self.io.read_all()
            count = int(args[0]) if args else 2
            frames = self.temp.loop(frames, count)
            self.io.write(output, frames)
        
        elif cmd == 'duplicate':
            frames = self.io.read_all()
            factor = int(args[0]) if args else 2
            frames = self.temp.duplicate(frames, factor)
            self.io.write(output, frames)
        
        elif cmd == 'sepia':
            frames = self.io.read_all()
            frames = self.color.sepia(frames)
            self.io.write(output, frames)
        
        elif cmd == 'warm':
            frames = self.io.read_all()
            intensity = int(args[0]) if args else 20
            frames = self.color.warm(frames, intensity)
            self.io.write(output, frames)
        
        elif cmd == 'cool':
            frames = self.io.read_all()
            intensity = int(args[0]) if args else 20
            frames = self.color.cool(frames, intensity)
            self.io.write(output, frames)
        
        elif cmd == 'posterize':
            frames = self.io.read_all()
            levels = int(args[0]) if args else 4
            frames = self.color.posterize(frames, levels)
            self.io.write(output, frames)
        
        elif cmd == 'emboss':
            frames = self.io.read_all()
            frames = self.filt.emboss(frames)
            self.io.write(output, frames)
        
        elif cmd == 'cartoon':
            frames = self.io.read_all()
            frames = self.filt.cartoon(frames)
            self.io.write(output, frames)
        
        elif cmd == 'vignette':
            frames = self.io.read_all()
            intensity = float(args[0]) if args else 0.3
            frames = self.filt.vignette(frames, intensity)
            self.io.write(output, frames)
        
        elif cmd == 'motion_blur':
            frames = self.io.read_all()
            size = int(args[0]) if args else 15
            angle = float(args[1]) if len(args) > 1 else 0
            frames = self.filt.motion_blur(frames, size, angle)
            self.io.write(output, frames)
        
        elif cmd == 'fade_in':
            frames = self.io.read_all()
            duration = int(args[0]) if args else 30
            frames = self.comp.fade_in(frames, duration)
            self.io.write(output, frames)
        
        elif cmd == 'fade_out':
            frames = self.io.read_all()
            duration = int(args[0]) if args else 30
            frames = self.comp.fade_out(frames, duration)
            self.io.write(output, frames)
        
        elif cmd == 'crossfade':
            frames1 = self.io.read_all()
            io2 = VideoIO(args[0])
            frames2 = io2.read_all()
            duration = int(args[1]) if len(args) > 1 else 30
            frames = self.comp.crossfade(frames1, frames2, duration)
            self.io.write(output, frames)
        
        elif cmd == 'stack_v':
            frames1 = self.io.read_all()
            io2 = VideoIO(args[0])
            frames2 = io2.read_all()
            frames = self.comp.stack_vertical(frames1, frames2)
            self.io.write(output, frames)
        
        elif cmd == 'text':
            frames = self.io.read_all()
            text = args[0] if args else "Text"
            x = int(args[1]) if len(args) > 1 else 10
            y = int(args[2]) if len(args) > 2 else 30
            scale = float(args[3]) if len(args) > 3 else 1.0
            color = (int(args[4]), int(args[5]), int(args[6])) if len(args) > 6 else (255, 255, 255)
            thickness = int(args[7]) if len(args) > 7 else 2
            frames = self.comp.text_overlay(frames, text, x, y, scale, color, thickness)
            self.io.write(output, frames)
        
        elif cmd == 'extract':
            frame_idx = int(args[0]) if args else 0
            self.io.extract_frame(frame_idx, output)
            print(f"Done: {output}")



