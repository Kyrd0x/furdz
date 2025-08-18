#!/usr/bin/env python3
"""
Ducky Script Encoder - Python Version
Converts Ducky Script to binary payload for USB Rubber Ducky

Based on the original HTML/JavaScript encoder
"""

import json
import sys
import argparse
import re
from typing import List, Dict, Tuple, Optional, Union

class DuckyEncoder:
    def __init__(self, keyboard_layout: Optional[Dict] = None, debug: bool = False):
        self.debug = debug
        self.debug_logs = []
        self.exit_code = 0
        self.current_instruction_index = 0
        self.last_useful_instruction_index = None
        self.output_buffer = []
        self.default_delay = 0
        self.delay_override = False
        self.instruction_map = []
        
        # Default Frnech keyboard layout (from the original)
        self.default_keyboard_layout = {
            "q": "00,00,04", "b": "00,00,05", "c": "00,00,06", "d": "00,00,07",
            "e": "00,00,08", "f": "00,00,09", "g": "00,00,0a", "h": "00,00,0b",
            "i": "00,00,0c", "j": "00,00,0d", "k": "00,00,0e", "l": "00,00,0f",
            ",": "00,00,10", "n": "00,00,11", "o": "00,00,12", "p": "00,00,13",
            "a": "00,00,14", "r": "00,00,15", "s": "00,00,16", "t": "00,00,17",
            "u": "00,00,18", "v": "00,00,19", "z": "00,00,1a", "x": "00,00,1b",
            "y": "00,00,1c", "w": "00,00,1d", "&": "00,00,1e", "\"": "00,00,20",
            "'": "00,00,21", "(": "00,00,22", "-": "00,00,23", "_": "00,00,25",
            "ENTER": "00,00,28", "ESC": "00,00,29", "ESCAPE": "00,00,29",
            "TAB": "00,00,2b", " ": "00,00,2c", "SPACE": "00,00,2c",
            ")": "00,00,2d", "=": "00,00,2e", "$": "00,00,30", "*": "00,00,31",
            "m": "00,00,33", ";": "00,00,36", ":": "00,00,37", "!": "00,00,38",
            "CAPSLOCK": "00,00,39", "F1": "00,00,3a", "F2": "00,00,3b",
            "F3": "00,00,3c", "F4": "00,00,3d", "F5": "00,00,3e", "F6": "00,00,3f",
            "F7": "00,00,40", "F8": "00,00,41", "F9": "00,00,42", "F10": "00,00,43",
            "F11": "00,00,44", "F12": "00,00,45", "PRINTSCREEN": "00,00,46",
            "SCROLLLOCK": "00,00,47", "BREAK": "00,00,48", "PAUSE": "00,00,48",
            "INSERT": "00,00,49", "HOME": "00,00,4a", "PAGEUP": "00,00,4b",
            "DEL": "00,00,4c", "DELETE": "00,00,4c", "END": "00,00,4d",
            "PAGEDOWN": "00,00,4e", "RIGHT": "00,00,4f", "RIGHTARROW": "00,00,4f",
            "LEFT": "00,00,50", "LEFTARROW": "00,00,50", "DOWN": "00,00,51",
            "DOWNARROW": "00,00,51", "UP": "00,00,52", "UPARROW": "00,00,52",
            "<": "00,00,64", "APP": "00,00,65", "MENU": "00,00,65",
            "ALT-TAB": "00,00,71", "CONTROL": "01,00,00", "CTRL": "01,00,00",
            "SHIFT": "02,00,00", "Q": "02,00,04", "B": "02,00,05", "C": "02,00,06",
            "D": "02,00,07", "E": "02,00,08", "F": "02,00,09", "G": "02,00,0a",
            "H": "02,00,0b", "I": "02,00,0c", "J": "02,00,0d", "K": "02,00,0e",
            "L": "02,00,0f", "?": "02,00,10", "N": "02,00,11", "O": "02,00,12",
            "P": "02,00,13", "A": "02,00,14", "R": "02,00,15", "S": "02,00,16",
            "T": "02,00,17", "U": "02,00,18", "V": "02,00,19", "Z": "02,00,1a",
            "X": "02,00,1b", "Y": "02,00,1c", "W": "02,00,1d", "1": "02,00,1e",
            "2": "02,00,1f", "3": "02,00,20", "4": "02,00,21", "5": "02,00,22",
            "6": "02,00,23", "7": "02,00,24", "8": "02,00,25", "9": "02,00,26",
            "0": "02,00,27", "+": "02,00,2e", "M": "02,00,33", "%": "02,00,34",
            ".": "02,00,36", "/": "02,00,37", ">": "02,00,64",
            "CTRL-SHIFT": "03,00,00", "ALT": "04,00,00", "CTRL-ALT": "05,00,00",
            "ALT-SHIFT": "06,00,00", "COMMAND": "08,00,00", "GUI": "08,00,00",
            "WINDOWS": "08,00,00", "COMMAND-OPTION": "12,00,00", "~": "40,00,1f",
            "#": "40,00,20", "{": "40,00,21", "[": "40,00,22", "|": "40,00,23",
            "`": "40,00,24", "\\": "40,00,25", "^": "40,00,26", "@": "40,00,27",
            "]": "40,00,2d", "}": "40,00,2e"
        }
        
        self.keyboard_layout = keyboard_layout or self.default_keyboard_layout
        
        # Instructions to ignore
        self.instructions_to_ignore = ['REM', '//', '\n', '', None]
        
        # Syntax mapping for special commands
        self.syntax_map = {
            'REPEAT': self._handle_repeat,
            'DEFAULTDELAY': self._handle_default_delay,
            'DEFAULT_DELAY': self._handle_default_delay,
            'STRING': self._handle_string,
            'DELAY': self._handle_delay,
            'STRING_DELAY': self._handle_string_delay,
            'CTRL': self._handle_modifier,
            'CONTROL': self._handle_modifier,
            'SHIFT': self._handle_modifier,
            'ALT': self._handle_modifier,
            'GUI': self._handle_modifier,
            'WINDOWS': self._handle_modifier,
            'CTRL-ALT': self._handle_modifier,
            'CTRL-SHIFT': self._handle_modifier,
            'ALT-SHIFT': self._handle_modifier,
            'COMMAND': self._handle_modifier,
            'COMMAND-CTRL': self._handle_modifier,
            'COMMAND-CTRL-SHIFT': self._handle_modifier,
            'COMMAND-OPTION': self._handle_modifier,
            'COMMAND-OPTION-SHIFT': self._handle_modifier
        }

    def _debug_log(self, message: str):
        """Add debug message to logs"""
        if self.debug:
            print(f"DEBUG: {message}")
            self.debug_logs.append(message)

    def _sanitize_script(self, script: str) -> str:
        """Sanitize the input script"""
        # Remove carriage returns and fix quotes
        script = script.replace('\r', '')
        script = script.replace('"', '"')
        return script

    def _split_script(self, script: str) -> List[str]:
        """Split script into lines"""
        return script.split('\n')

    def _split_line(self, line: str) -> List[str]:
        """Split line into command and arguments"""
        return line.split(' ')

    def _build_instruction_map(self, lines: List[str]) -> List[Tuple[str, Optional[str]]]:
        """Build instruction map from lines"""
        instruction_map = []
        
        for line in lines:
            line = line.strip()
            if not line:
                continue
                
            # Handle special case for single space
            if line == " ":
                line = "SPACE"
            
            parts = self._split_line(line)
            command = parts[0] if parts else ""
            args = " ".join(parts[1:]) if len(parts) > 1 else None
            
            instruction_map.append((command, args))
            self._debug_log(f"Instruction: {command} | Args: {args}")
        
        return instruction_map

    def _get_bytes_for_key(self, key: str) -> Optional[List[str]]:
        """Get byte codes for a key"""
        if key not in self.keyboard_layout:
            return None
        
        codes = self.keyboard_layout[key].split(',')
        
        # Character key
        if codes[2] != '00':
            key_codes = [codes[2], codes[0]]
            if key_codes[1] == '00':
                key_codes.pop()
        else:
            # Modifier key
            key_codes = [codes[0]]
        
        return key_codes

    def _append_hex_array(self, hex_array: List[str]):
        """Append hex array to output buffer"""
        self.output_buffer.extend(hex_array)

    def _build_delay_bytes(self, delay: int) -> List[str]:
        """Build delay byte array"""
        byte_array = []
        while delay > 0:
            byte_array.append('00')
            if delay > 255:
                byte_array.append('FF')
                delay -= 255
            else:
                byte_array.append(format(delay, '02x'))
                delay = 0
        return byte_array

    def _do_delay_check(self) -> Optional[List[str]]:
        """Check if default delay should be applied"""
        if not self.delay_override and self.default_delay > 0:
            return self._build_delay_bytes(self.default_delay)
        return None

    def _defaulting_hex_warning(self, key: str):
        """Handle warning for unknown key"""
        self._debug_log(f"WARN - defaulting 0x00 for char: {key}")
        self.exit_code = 1
        self._append_hex_array(['00', '00'])

    def _lookup_and_append(self, command: str):
        """Look up key and append to buffer"""
        self._debug_log(f"Key lookup on {command}")
        key_codes = self._get_bytes_for_key(command)
        
        if key_codes is None:
            if command == command[0]:  # Single character
                self._defaulting_hex_warning(command)
                return
            # Try first character
            self._lookup_and_append(command[0])
            return
        
        if len(key_codes) > 1:
            self._append_hex_array(key_codes)
        else:
            self._append_hex_array(key_codes + ['00'])

    # Command handlers
    def _handle_repeat(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle REPEAT command"""
        if not args or self.last_useful_instruction_index is None:
            return None
        
        loop_count = int(args.strip())
        instr_to_repeat = self.instruction_map[self.last_useful_instruction_index]
        cmd, arg = instr_to_repeat
        
        for _ in range(loop_count):
            if cmd not in self.instructions_to_ignore:
                self._delegate_command(cmd, arg)
        
        return None

    def _handle_delay(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle DELAY command"""
        if not args:
            return None
        
        self.delay_override = True
        delay = int(args)
        return self._build_delay_bytes(delay)

    def _handle_default_delay(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle DEFAULT_DELAY command"""
        if not args:
            return None
        
        self.default_delay = int(args)
        self.delay_override = True
        return None

    def _handle_string_delay(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle STRING_DELAY command"""
        if not args:
            return None
        
        parts = args.split(" ", 1)
        if len(parts) < 2:
            return None
        
        delay_arg = int(parts[0])
        inject_string = parts[1]
        
        result = []
        for char in inject_string:
            byte_array = self._get_bytes_for_key(char)
            if byte_array is None:
                self._defaulting_hex_warning(char)
            elif len(byte_array) > 1:
                result.extend(byte_array)
            else:
                result.extend(byte_array + ['00'])
            
            # Add delay
            counter = delay_arg
            while counter > 0:
                result.append('00')
                if counter > 0xFF:
                    result.append('FF')
                    counter -= 0xFF
                else:
                    result.append(format(counter, '02x'))
                    counter = 0
        
        return result

    def _handle_string(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle STRING command"""
        if not args:
            return None
        
        result = []
        for char in args:
            byte_array = self._get_bytes_for_key(char)
            if byte_array is None:
                self._defaulting_hex_warning(char)
            elif len(byte_array) > 1:
                result.extend(byte_array)
            else:
                result.extend(byte_array + ['00'])
        
        return result

    def _handle_modifier(self, command: str, args: Optional[str]) -> Optional[List[str]]:
        """Handle modifier commands (CTRL, ALT, etc.)"""
        if not args:
            key = ""
        else:
            key = args.split()[0] if args.split() else ""
        
        mod_hex = self._get_bytes_for_key(command)
        key_hex = self._get_bytes_for_key(key)
        
        if mod_hex is None:
            mod_hex = ["00"]
        if key_hex is None:
            key_hex = ["00"]
        
        return [key_hex[0], mod_hex[0]]

    def _delegate_command(self, command: str, args: Optional[str]):
        """Delegate command to appropriate handler"""
        if command in self.syntax_map:
            self._debug_log(f"Delegating syntax for {command}")
            result = self.syntax_map[command](command, args)
            if result is not None:
                self._append_hex_array(result)
        elif command not in self.instructions_to_ignore:
            # Try to look up as key
            self._lookup_and_append(command)
        else:
            self._debug_log(f"Key not found, syntax not understood: {command} {args}")

    def encode(self, script: str) -> Tuple[bytes, int]:
        """
        Encode Ducky Script to binary payload
        
        Args:
            script: The Ducky Script as string
            
        Returns:
            Tuple of (binary_payload, exit_code)
            exit_code: 0 = success, 1 = warnings, -1 = error
        """
        # Reset state
        self.output_buffer = []
        self.exit_code = 0
        self.current_instruction_index = 0
        self.last_useful_instruction_index = None
        self.default_delay = 0
        self.delay_override = False
        self.debug_logs = []
        
        if not script.strip():
            return b'', -1
        
        try:
            # Sanitize and split script
            sanitized = self._sanitize_script(script)
            lines = [line for line in self._split_script(sanitized) if line.strip()]
            
            # Convert single spaces to SPACE command
            lines = [line if line != " " else "SPACE" for line in lines]
            
            # Build instruction map
            self.instruction_map = self._build_instruction_map(lines)
            
            # Process each instruction
            for index, (command, args) in enumerate(self.instruction_map):
                self.current_instruction_index = index
                
                if command in self.instructions_to_ignore:
                    continue
                
                # Process command
                self._delegate_command(command, args)
                
                # Add default delay if needed
                delay_bytes = self._do_delay_check()
                if delay_bytes:
                    self._append_hex_array(delay_bytes)
                
                self.last_useful_instruction_index = index
                
        except Exception as e:
            self._debug_log(f"Error during encoding: {str(e)}")
            self.exit_code = -1
        
        # Convert hex strings to bytes
        try:
            hex_string = ''.join(self.output_buffer)
            if len(hex_string) % 2 != 0:
                hex_string += '0'  # Pad if odd length
            
            binary_data = bytes.fromhex(hex_string)
            return binary_data, self.exit_code
            
        except ValueError as e:
            self._debug_log(f"Error converting hex to binary: {str(e)}")
            return b'', -1

    def load_keyboard_layout(self, layout_file: str):
        """Load keyboard layout from JSON file"""
        try:
            with open(layout_file, 'r') as f:
                self.keyboard_layout = json.load(f)
        except Exception as e:
            print(f"Error loading keyboard layout: {str(e)}")
            raise

    def save_debug_log(self, filename: str):
        """Save debug log to file"""
        with open(filename, 'w') as f:
            f.write('\n'.join(self.debug_logs))


def main():
    parser = argparse.ArgumentParser(description='Ducky Script Encoder - Convert Ducky Script to binary payload')
    parser.add_argument('input_file', help='Input Ducky Script file (.txt)')
    parser.add_argument('-o', '--output', help='Output binary file (default: payload.bin)')
    parser.add_argument('-k', '--keyboard', help='Keyboard layout JSON file')
    parser.add_argument('-d', '--debug', action='store_true', help='Enable debug mode')
    parser.add_argument('--save-debug', help='Save debug log to file')
    
    args = parser.parse_args()
    
    # Set default output filename
    if not args.output:
        args.output = 'payload.bin'
    
    # Create encoder
    encoder = DuckyEncoder(debug=args.debug)
    
    # Load custom keyboard layout if provided
    if args.keyboard:
        try:
            encoder.load_keyboard_layout(args.keyboard)
            print(f"Loaded keyboard layout from {args.keyboard}")
        except Exception as e:
            print(f"Error loading keyboard layout: {e}")
            return 1
    
    # Read input script
    try:
        with open(args.input_file, 'r') as f:
            script = f.read()
    except Exception as e:
        print(f"Error reading input file: {e}")
        return 1
    
    # Encode script
    print("Encoding Ducky Script...")
    binary_payload, exit_code = encoder.encode(script)
    
    if exit_code == -1:
        print("ERROR: Encoding failed!")
        return 1
    elif exit_code == 1:
        print("WARNING: Some characters were not found in keyboard layout")
    else:
        print("SUCCESS: Encoding completed successfully")
    
    # Save binary payload
    try:
        with open(args.output, 'wb') as f:
            f.write(binary_payload)
        print(f"Binary payload saved to {args.output}")
    except Exception as e:
        print(f"Error saving output file: {e}")
        return 1
    
    # Save debug log if requested
    if args.save_debug:
        try:
            encoder.save_debug_log(args.save_debug)
            print(f"Debug log saved to {args.save_debug}")
        except Exception as e:
            print(f"Error saving debug log: {e}")
    
    return 0


if __name__ == '__main__':
    sys.exit(main())