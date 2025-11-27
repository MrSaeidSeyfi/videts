"""Top-level package exports for videts (package layout).

This file mirrors the previous top-level `src/__init__.py` but lives inside
the `videts` package folder so `import videts` works when installed or used from source.
"""
__version__ = "0.1.1"

from .video_editor import VideoEditor
from .video_io import VideoIO

# Expose operation helpers for convenience
from .color_ops import ColorOps
from .filter_ops import FilterOps
from .geometric_ops import GeometricOps
from .morph_ops import MorphOps
from .composite_ops import CompositeOps
from .temporal_ops import TemporalOps

__all__ = [
    '__version__',
    'VideoEditor', 'VideoIO',
    'ColorOps', 'FilterOps', 'GeometricOps', 'MorphOps', 'CompositeOps', 'TemporalOps',
]
