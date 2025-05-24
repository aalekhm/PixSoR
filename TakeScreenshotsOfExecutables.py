import os
import subprocess
import time
import pyautogui
import pygetwindow as gw
import argparse

# Setup argument parser
parser = argparse.ArgumentParser(description="Run executables, capture window screenshots, and save stdout output.")
parser.add_argument("exe_directory", type=str, help="Directory containing executable files")
parser.add_argument("screenshot_directory", type=str, help="Directory to save screenshots")
parser.add_argument("output_directory", type=str, help="Directory to save stdout outputs")
parser.add_argument("save_fullscreen_screenshot", type=str, help="Save full-screen screenshot if window not found? Yes/No")

# Parse arguments
args = parser.parse_args()

exe_directory = args.exe_directory
screenshot_directory = args.screenshot_directory
output_directory = args.output_directory
save_fullscreen_screenshot = True if args.save_fullscreen_screenshot == "Yes" else False

# Ensure output directories exist
for directory in [screenshot_directory, output_directory]:
    if not os.path.exists(directory):
        os.makedirs(directory)

# Change to the input directory
os.chdir(exe_directory)

# Iterate through all files in the directory
for filename in os.listdir(exe_directory):
    if filename.endswith(".exe"):  # Process only executable files
        exe_path = os.path.join(exe_directory, filename)

        print(f"Launching: {exe_path}")

        # Open executable, capturing stdout
        stdout_path = os.path.join(output_directory, f"{os.path.splitext(filename)[0]}_output.txt")
        with open(stdout_path, "w") as stdout_file:
            process = subprocess.Popen(exe_path, stdout=stdout_file, stderr=subprocess.STDOUT, text=True)
        
        time.sleep(5)  # Wait for the application to load

        # Remove ".exe" extension from filename for window title matching
        window_title = os.path.splitext(filename)[0]

        # Find the application's window
        window = None
        for w in gw.getWindowsWithTitle(window_title):
            if window_title.lower() in w.title.lower():  # Match window title
                window = w
                break
                
        time.sleep(1)   # Some delay before taking a screenshot
        
        if window:
            # Capture only the window area
            left, top, width, height = window.left, window.top, window.width, window.height
            screenshot = pyautogui.screenshot(region=(left, top, width, height))
            screenshot = screenshot.convert("RGB")  # Ensures compatibility with JPEG

            screenshot_path = os.path.join(screenshot_directory, f"{window_title}_screenshot.jpg")
            screenshot.save(screenshot_path, "JPEG")

            print(f"Screenshot saved: {screenshot_path}")
        else:
            if save_fullscreen_screenshot:
                print(f"Could not find a window for {filename}. Saving full-screen screenshot instead.")
                screenshot = pyautogui.screenshot().convert("RGB")  # Convert to RGB for JPEG format
                screenshot_path = os.path.join(screenshot_directory, f"{window_title}_screenshot.jpg")
                screenshot.save(screenshot_path, "JPEG")

        # Close the application (optional)
        process.terminate()
        time.sleep(2)  # Allow time for application to close

        print(f"Stdout output saved to: {stdout_path}")