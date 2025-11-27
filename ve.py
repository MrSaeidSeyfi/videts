import sys
from videts.help import print_help

if __name__ == '__main__':
    # CLI parsing with help handling. Keep imports lazy so help can show without OpenCV available.
    argv = sys.argv
    if len(argv) == 1:
        print_help(prog_name='python ve.py')
        sys.exit(0)

    # top-level help flags
    if argv[1] in ('-h', '--help'):
        # optional command: python ve.py --help crop
        if len(argv) > 2:
            print_help(argv[2], prog_name='python ve.py')
        else:
            print_help(prog_name='python ve.py')
        sys.exit(0)

    # python ve.py help [command]
    if argv[1] == 'help':
        if len(argv) > 2:
            print_help(argv[2], prog_name='python ve.py')
        else:
            print_help(prog_name='python ve.py')
        sys.exit(0)

    # any later -h/--help means show help for the command
    if '-h' in argv[1:] or '--help' in argv[1:]:
        # find command position: expected form input command output
        cmd = argv[2] if len(argv) >= 3 else None
        print_help(cmd, prog_name='python ve.py')
        sys.exit(0)

    # normal run: need at least input, command and output
    if len(argv) < 4:
        print("Usage: python ve.py <input> <command> <output> [args...]")
        sys.exit(1)

    # import VideoEditor lazily to avoid importing OpenCV on help calls
    from videts.video_editor import VideoEditor

    input_path = argv[1]
    cmd = argv[2]
    out = argv[3]
    args = argv[4:] if len(argv) > 4 else []

    editor = VideoEditor(input_path)
    editor.execute(cmd, out, args)
    print(f"Done: {out}")
