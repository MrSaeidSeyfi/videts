import sys
from src.video_editor import VideoEditor

if __name__ == '__main__':
    if len(sys.argv) < 4:
        print("Usage: python ve.py <input> <command> <output> [args...]")
        sys.exit(1)
    
    editor = VideoEditor(sys.argv[1])
    cmd = sys.argv[2]
    out = sys.argv[3]
    args = sys.argv[4:] if len(sys.argv) > 4 else []
    
    editor.execute(cmd, out, args)
    print(f"Done: {out}")
