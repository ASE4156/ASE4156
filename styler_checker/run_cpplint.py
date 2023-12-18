import os
import sys
import subprocess

def run_cpplint(directory, cpplint_path):
    current_dir = os.getcwd()  # Get current working directory
    reports_dir = os.path.join(current_dir, 'reports')  # Create reports directory path

    os.makedirs(reports_dir, exist_ok=True)  # Create the 'reports' directory if it doesn't exist

    for root, _, files in os.walk(directory):
        for filename in files:
            if filename.endswith(".cpp"):
                file_path = os.path.join(root, filename)
                output_file = os.path.join(reports_dir, 'style_checker.txt')  # Output file path
                
                # Open a subprocess to execute the command and redirect both stdout and stderr to the output file
                with open(output_file, 'a') as output:
                    subprocess.run(
                        [sys.executable, cpplint_path, file_path],
                        stdout=output,
                        stderr=subprocess.STDOUT,  # Redirect stderr to stdout
                        text=True  # Capture text output
                    )

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 run_cpplint.py <directory> <cpplint_path>")
        sys.exit(1)

    directory = sys.argv[1]
    cpplint_path = sys.argv[2]

    run_cpplint(directory, cpplint_path)
