import os
import time
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

PPM_FILE = "image.ppm"

class PPMHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith(PPM_FILE):
            print(f"{PPM_FILE} updated. Waiting for stability...")
            if self.wait_for_stable_file(PPM_FILE):
                print("Running viewer...")
                subprocess.run(["python", "viewer.py"])
            else:
                print("File not stable. Skipping this run.")

    def wait_for_stable_file(self, path, timeout=2, interval=0.2):
        """Wait until file size is stable for a given duration"""
        start_time = time.time()
        last_size = -1
        while time.time() - start_time < timeout:
            try:
                current_size = os.path.getsize(path)
                if current_size == last_size:
                    return True
                last_size = current_size
                time.sleep(interval)
            except FileNotFoundError:
                return False
        return False

if __name__ == "__main__":
    path = "."
    event_handler = PPMHandler()
    observer = Observer()
    observer.schedule(event_handler, path, recursive=False)
    observer.start()
    print(f"Watching for changes to {PPM_FILE}...")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
