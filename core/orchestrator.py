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

def build_exe(exe_name, prioritize_size=False, verbose=False):
    if verbose:
        print(f"[i] Compiling EXE '{exe_name}'...")
    subprocess.run(["bash", "scripts/compile_exe.sh", "-o", exe_name, "--prioritize-size", str(prioritize_size).lower()], check=True)

def clean_build_env():
    subprocess.run(["rm", "-rf", "bin", "build", ".build"], check=True)