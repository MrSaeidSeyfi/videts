"""Command-line entry point for the videts package.

This module is purposely lightweight and imports the heavy VideoEditor
only when the user actually runs a command. Running `--help` or `help`
will display help text without importing OpenCV.
"""
import sys
from .help import print_help


def main(argv=None):
    """Main entry point for console_scripts.

    argv is a list (like sys.argv) or None (defaults to sys.argv).
    This function prints help on -h/--help or 'help' and runs video operations otherwise.
    """
    if argv is None:
        argv = sys.argv

    # keep behavior same as root ve.py: support help and lazy import
    if len(argv) == 1:
        # print general help
        print_help()
        return 0

    # detect help flags
    if argv[1] in ('-h', '--help'):
        if len(argv) > 2:
            print_help(argv[2])
        else:
            print_help()
        return 0

    if argv[1] == 'help':
        if len(argv) > 2:
            print_help(argv[2])
        else:
            print_help()
        return 0

    if '-h' in argv[1:] or '--help' in argv[1:]:
        cmd = argv[2] if len(argv) >= 3 else None
        print_help(cmd)
        return 0

    # Need input, command, output
    if len(argv) < 4:
        print('Usage: videts <input> <command> <output> [args...]')
        return 1

    # now import VideoEditor lazily
    from .video_editor import VideoEditor

    input_path = argv[1]
    cmd = argv[2]
    out = argv[3]
    args = argv[4:] if len(argv) > 4 else []

    editor = VideoEditor(input_path)
    editor.execute(cmd, out, args)
    print(f'Done: {out}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
