import time
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from pathlib import Path


class PPMChangeHandler(FileSystemEventHandler):
    def __init__(self, file_to_watch, command_to_run):
        self.file_to_watch = Path(file_to_watch).resolve()
        self.command_to_run = command_to_run

    def on_modified(self, event):
        if Path(event.src_path).resolve() == self.file_to_watch:
            print(f"{self.file_to_watch.name} updated. Running viewer...")
            subprocess.run(self.command_to_run)


if __name__ == "__main__":
    ppm_file = "image.ppm"
    cmd = ["python", "viewer.py"]

    event_handler = PPMChangeHandler(ppm_file, cmd)
    observer = Observer()
    observer.schedule(event_handler, path=".", recursive=False)

    print(f"Watching {ppm_file} for changes...")
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()