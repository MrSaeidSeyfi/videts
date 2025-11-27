# videts — Video editing tools

Lightweight, easy-to-use Python tools for basic video editing (crop, resize, rotate, color effects, filters, composition and more).

## Features
- Read and write MP4 video files
- Common video operations: crop, resize, rotate, flip, pan, zoom
- Color operations: gray, invert, brightness, contrast, hue, saturation, sepia
- Filters: blur, sharpen, edge, denoise, vignette, motion blur and more
- Morphology: erode, dilate, open, close
- Composition: overlay, stack (vertical/horizontal), crossfade, fade in/out, text overlay
- Temporal ops: reverse, speed up, loop, freeze, duplicate

## Requirements
- Python 3.8+
- opencv-python
- numpy

Install dependencies with pip if needed:

```powershell
pip install opencv-python numpy
```

## Installation

Install from PyPI (recommended):

```powershell
pip install videts
```

Or install from source (developer):

```powershell
pip install .
```

## Quick start (CLI)

This project includes a small CLI helper `ve.py` (for direct execution) and a packaged command-line tool that is installed when you run `pip install videts`.

You will get a console command named `videts` and a short alias `ve` after installing the package. Both work the same and behave like `python ve.py` demonstrated below.

Usage (local script or installed command):

```powershell
python ve.py <input> <command> <output> [args...]

# examples
python ve.py input.mp4 crop output.mp4 10 10 300 200       # crop to rectangle x=10,y=10 width=300 height=200
python ve.py input.mp4 resize output.mp4 640 480           # resize to 640×480
python ve.py input.mp4 gray output.mp4                    # convert to grayscale
python ve.py input.mp4 overlay output.mp4 other.mp4       # overlay other.mp4 onto input.mp4
python ve.py input.mp4 extract frame_0001.png             # extract first frame as PNG
```

For a quick list of supported commands, inspect `src/video_editor.py` or run `videts --help` after installation.

### Getting help from the CLI

You can ask the CLI for help without loading heavy dependencies:

```powershell
python ve.py --help            # show short usage and list of supported commands
python ve.py help              # same as --help
python ve.py help crop         # show usage for the `crop` command
python ve.py input.mp4 crop --help  # show help for the 'crop' command
```

## Usage in Python (library)

Use the VideoEditor class for programmatic usage:

```python
from videts.video_editor import VideoEditor

editor = VideoEditor('input.mp4')
# run a single operation (command, output_path, [args])
editor.execute('crop', 'output_crop.mp4', ['10', '10', '300', '200'])

# you can also use VideoIO or operation classes directly (see src/video_io.py and src/*_ops.py)
```
 
## Notes

- This package relies on OpenCV (cv2). Depending on your platform, installing `opencv-python` is usually sufficient.
- Read and write functions allocate frames in memory; for very large videos, consider streaming or custom handling to avoid high memory use.

## License
MIT — see the `LICENSE` file in the project for details.

--- 
