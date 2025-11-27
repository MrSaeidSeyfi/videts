"""Shared command help for the videts package (inside package).

This mirrors the top-level helper so both the installed command and local script
share the same help text.
"""
COMMAND_HELP = {
    'crop': ('crop', 'Crop to rectangle: x y width height'),
    'resize': ('resize', 'Resize to width height'),
    'rotate': ('rotate', 'Rotate by angle in degrees'),
    'flip': ('flip', 'Flip frames direction (0 vertical, 1 horizontal)'),
    'reverse': ('reverse', 'Reverse all frames'),
    'speed': ('speed', 'Speed up (>1) or write reduced fps (<1): value'),
    'trim': ('trim', 'Trim from start_frame to end_frame (indices)'),
    'gray': ('gray', 'Convert to grayscale'),
    'invert': ('invert', 'Invert colors'),
    'bright': ('bright', 'Change brightness by value (integer)'),
    'contrast': ('contrast', 'Change contrast by multiplier (float)'),
    'hue': ('hue', 'Shift hue by degrees (0-179)'),
    'sat': ('sat', 'Adjust saturation (float multiplier)'),
    'blur': ('blur', 'Gaussian blur with kernel size (odd int)'),
    'sharpen': ('sharpen', 'Sharpen frames'),
    'edge': ('edge', 'Edge detection'),
    'denoise': ('denoise', 'Denoise frames'),
    'threshold': ('threshold', 'Binary threshold (0-255)'),
    'dilate': ('dilate', 'Morphology dilation (kernel size)'),
    'erode': ('erode', 'Morphology erosion (kernel size)'),
    'open': ('open', 'Morphology open (kernel size)'),
    'close': ('close', 'Morphology close (kernel size)'),
    'stack': ('stack', 'Stack two videos horizontally — second file path required'),
    'overlay': ('overlay', 'Overlay second video at x y — second file path required'),
    'zoom': ('zoom', 'Zoom by factor (center optional: cx cy)'),
    'pan': ('pan', 'Pan frames by dx dy'),
    'freeze': ('freeze', 'Freeze a frame index for duration frames'),
    'loop': ('loop', 'Repeat video count times'),
    'duplicate': ('duplicate', 'Duplicate every frame N times'),
    'sepia': ('sepia', 'Apply sepia effect'),
    'warm': ('warm', 'Make image warmer (intensity)'),
    'cool': ('cool', 'Make image cooler (intensity)'),
    'posterize': ('posterize', 'Reduce color levels (int)'),
    'emboss': ('emboss', 'Emboss filter'),
    'cartoon': ('cartoon', 'Cartoon effect'),
    'vignette': ('vignette', 'Vignette effect (intensity float)'),
    'motion_blur': ('motion_blur', 'Motion blur size angle'),
    'fade_in': ('fade_in', 'Fade in duration frames'),
    'fade_out': ('fade_out', 'Fade out duration frames'),
    'crossfade': ('crossfade', 'Crossfade with another video — second file path and duration required'),
    'stack_v': ('stack_v', 'Stack two videos vertically — second file path required'),
    'text': ('text', 'Put text on video: text x y scale r g b thickness'),
    'extract': ('extract', 'Extract single frame by index to an image path'),
}


def print_help(command=None, prog_name='videts'):
    """Print general or per-command help to the console.

    Args:
        command: optional command name to show detailed help for.
        prog_name: name to show in usage text (defaults to 'videts').
    """
    if command:
        c = command.strip().lower()
        if c in COMMAND_HELP:
            sig, desc = COMMAND_HELP[c]
            print(f"Command: {sig}\n  {desc}")
        else:
            print(f"Unknown command: {command}\nUse: {prog_name} --help  or {prog_name} help to list commands")
        return

    # general help
    print(f"Usage: {prog_name} <input> <command> <output> [args...]")
    print()
    print("Common commands (examples):")
    for key in sorted(COMMAND_HELP.keys()):
        sig, desc = COMMAND_HELP[key]
        print(f"  {sig:12} - {desc}")
    print()
    print("Examples:")
    print(f"  {prog_name} input.mp4 crop output.mp4 10 10 300 200")
    print(f"  {prog_name} input.mp4 resize output.mp4 640 480")
    print(f"  {prog_name} input.mp4 gray output.mp4")
    print(f"  {prog_name} help crop")
    print(f"  {prog_name} input.mp4 crop --help")
