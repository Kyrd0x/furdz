import subprocess
import os

def prepare_build_env():
    subprocess.run(["rm", "-rf", "build"], check=True)
    subprocess.run(["mkdir", "-p", "build/bin", "bin"], check=True)
    subprocess.run(["cp", "-r", "src", "include", "build/"], check=True)

def build_dll(dll_name, verbose=False):
    if verbose:
        print(f"[i] Compiling DLL '{dll_name}'...")
    subprocess.run(["bash", "scripts/compile_dll.sh", dll_name], check=True)

def build_exe(output_file="executable.exe", prioritize_size=False, verbose=False):
    if verbose:
        print(f"[i] Compiling EXE '{output_file}'...")
    subprocess.run(["bash", "scripts/compile_exe.sh", output_file, str(prioritize_size).lower(), str(verbose).lower()], check=True)

def clean_build_env():
    subprocess.run(["rm", "-rf", "bin", "build", ".build"], check=True)