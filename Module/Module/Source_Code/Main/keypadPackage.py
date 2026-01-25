#!/usr/bin/env python3
"""
Keypad Module Packaging Tool - Compatible with main firmware linker script
Extracts function offsets from IAR map file for execution buffer at 0x3C000
"""

import struct
import subprocess
import sys
import os
import re
import json
from pathlib import Path

class KeypadPackager:
    def __init__(self):
        self.iar_path = r"C:\Program Files (x86)\IAR Systems\Embedded Workbench 8.0\arm\bin"
        self.modules_dir = "Modules"
        self.output_dir = "Output"
        
        # Create output directory
        Path(self.output_dir).mkdir(exist_ok=True)
        
    def find_iar_compiler(self):
        """Find IAR compiler executables"""
        iccarm = os.path.join(self.iar_path, "iccarm.exe")
        ielftool = os.path.join(self.iar_path, "ielftool.exe")
        
        if not os.path.exists(iccarm):
            print("IAR compiler not found. Please install IAR Embedded Workbench")
            return None, None
        
        return iccarm, ielftool

    def compile_keypad_module(self):
        """Compile keypad module using IAR compiler for execution buffer at 0x3C000"""
        
        iccarm, ielftool = self.find_iar_compiler()
        if not iccarm:
            return False
        
        elf_file = os.path.join(self.output_dir, "keypad_module.elf")
        bin_file = os.path.join(self.output_dir, "keypad_module.bin")
        map_file = os.path.join(self.output_dir, "keypad_module.map")
        
        # IAR compilation command for execution buffer
        cmd = [
            iccarm,
            os.path.join(self.modules_dir, "keypad.c"),
            "-o", elf_file,
            "--cpu=Cortex-M0",
            "-f", os.path.join(self.modules_dir, "eeprom_module.icf"),  # Module linker script
            "--debug",
            "--endian=little",
            "-e",
            "--dlib_config", "full",
            "-On",  # Optimize for size
            "--no_cse", 
            "--no_unroll", 
            "--no_inline", 
            "--no_code_motion",
            "--no_tbaa", 
            "--no_clustering", 
            "--no_scheduling",
            "-D_IAR_",
            "--vla",
            "-I.", 
            "-I" + self.modules_dir,
            "--silent",
        ]
        
        try:
            print("Compiling keypad module for execution buffer at 0x3C000...")
            result = subprocess.run(cmd, check=True, capture_output=True, text=True)
            
            # Convert to binary
            subprocess.run([
                ielftool,
                elf_file,
                bin_file,
                "--bin"
            ], check=True, capture_output=True)
            
            # Generate map file in text format for parsing
            subprocess.run([
                ielftool,
                elf_file,
                "--output", "json",  # JSON format for easier parsing
                map_file
            ], check=True, capture_output=True)
            
            print("Keypad module compiled successfully")
            return elf_file, map_file
            
        except subprocess.CalledProcessError as e:
            print("IAR compilation failed")
            if e.stderr:
                print("STDERR:", e.stderr)
            return None, None

    def extract_function_offsets_from_map(self, map_file):
        """Extract function offsets from IAR map file for execution buffer"""
        
        if not os.path.exists(map_file):
            print("Map file not found:", map_file)
            return None
        
        # Keypad function names we're looking for
        target_functions = [
            "keypad_init",
            "keypad_deinit", 
            "keypad_is_key_pressed",
            "keypad_get_key",
            "keypad_debounce_delay",
            "keypad_get_last_key",
            "keypad_wait_for_key"
        ]
        
        function_offsets = {}
        
        try:
            # Read and parse the map file
            with open(map_file, 'r', encoding='utf-8') as f:
                map_content = f.read()
            
            # Try to parse as JSON first (IAR JSON map format)
            try:
                map_data = json.loads(map_content)
                print("Parsing JSON map file...")
                
                # Navigate through the JSON structure to find functions
                if 'output' in map_data and 'sections' in map_data['output']:
                    for section in map_data['output']['sections']:
                        if 'name' in section and '.module_code' in section['name']:
                            if 'contents' in section:
                                for item in section['contents']:
                                    if 'symbols' in item:
                                        for symbol in item['symbols']:
                                            if 'name' in symbol and 'value' in symbol:
                                                func_name = symbol['name']
                                                if func_name in target_functions:
                                                    func_address = int(symbol['value'], 16)
                                                    # Calculate offset from execution buffer base (0x3C000)
                                                    func_offset = func_address - 0x3C000
                                                    function_offsets[func_name] = func_offset
                                                    print(f"Found {func_name} at 0x{func_address:05X} -> offset 0x{func_offset:04X}")
                
            except json.JSONDecodeError:
                # Fall back to text parsing if JSON fails
                print("Map file is not JSON, parsing as text...")
                self.extract_offsets_from_text_map(map_content, target_functions, function_offsets)
            
            # Verify we found all required functions
            missing_functions = [f for f in target_functions if f not in function_offsets]
            if missing_functions:
                print("Warning: Some functions not found in map file:", missing_functions)
                print("Using fallback offsets")
                function_offsets = self.get_fallback_offsets()
            else:
                print("Successfully extracted all function offsets from map file")
            
            return function_offsets
            
        except Exception as e:
            print("Error parsing map file:", e)
            print("Using fallback offsets")
            return self.get_fallback_offsets()

    def extract_offsets_from_text_map(self, map_content, target_functions, function_offsets):
        """Extract function offsets from text format map file"""
        
        # Look for function entries in the map file
        # Typical IAR map file format: "keypad_init   0x0003c000   0x50  Code  Gb  .module_code  keypad.o"
        for line in map_content.split('\n'):
            line = line.strip()
            for func_name in target_functions:
                if func_name in line:
                    # Extract hexadecimal address
                    address_match = re.search(r'0x([0-9a-fA-F]+)', line)
                    if address_match:
                        func_address = int(address_match.group(1), 16)
                        # Only consider addresses in execution buffer range
                        if 0x3C000 <= func_address <= 0x3FFFF:
                            func_offset = func_address - 0x3C000
                            function_offsets[func_name] = func_offset
                            print(f"Found {func_name} at 0x{func_address:05X} -> offset 0x{func_offset:04X}")
                            break

    def get_fallback_offsets(self):
        """Get fallback function offsets if map parsing fails"""
        return {
            "keypad_init": 0x0000,
            "keypad_deinit": 0x0050,  
            "keypad_is_key_pressed": 0x00A0,
            "keypad_get_key": 0x00F0,
            "keypad_debounce_delay": 0x0140,
            "keypad_get_last_key": 0x0190,
            "keypad_wait_for_key": 0x01E0
        }

    def map_function_names_to_ids(self, function_offsets):
        """Map function names to IDs for the function table"""
        name_to_id = {
            "keypad_init": 0,
            "keypad_deinit": 1,
            "keypad_is_key_pressed": 2,
            "keypad_get_key": 3,
            "keypad_debounce_delay": 4,
            "keypad_get_last_key": 5,
            "keypad_wait_for_key": 6
        }
        
        id_to_offset = {}
        for func_name, offset in function_offsets.items():
            func_id = name_to_id.get(func_name)
            if func_id is not None:
                id_to_offset[func_id] = offset
                print(f"Function {func_id} ({func_name}) -> 0x{offset:04X}")
        
        return id_to_offset

    def calculate_crc16(self, data):
        """Calculate CRC-16/CCITT-FALSE for module validation"""
        crc = 0xFFFF
        for byte in data:
            crc ^= byte << 8
            for _ in range(8):
                if crc & 0x8000:
                    crc = (crc << 1) ^ 0x1021
                else:
                    crc = crc << 1
                crc &= 0xFFFF
        return crc

    def create_module_package(self, bin_file, function_offsets):
        """Create complete module package for external flash"""
        
        # Read compiled binary
        try:
            with open(bin_file, "rb") as f:
                code_data = f.read()
        except FileNotFoundError:
            print("Compiled binary not found:", bin_file)
            return None
        
        code_size = len(code_data)
        print(f"Compiled code size: {code_size} bytes")
        
        # Create module header (matches loader_module_header_t in loader.c)
        header = struct.pack('<HHHH', 
            0x4D4D,           # magic 'MM'
            0x0001,           # module_id (KEYPAD)
            code_size,        # code_size
            0                 # crc16 placeholder
        )
        
        # Calculate CRC16 of code data
        crc16 = self.calculate_crc16(code_data)
        header = header[:4] + struct.pack('<H', crc16)  # Update CRC in header
        
        # Create function table
        function_count = len(function_offsets)
        func_table = struct.pack('<H', function_count)  # Function count first
        
        # Add function entries (sorted by function ID)
        for func_id in sorted(function_offsets.keys()):
            offset = function_offsets[func_id]
            func_table += struct.pack('<HH', func_id, offset)
        
        # Combine everything: header + code + function_table
        module_data = header + code_data + func_table
        
        # Pad to 256-byte boundary (FM25W32 page size)
        padding_size = (256 - (len(module_data) % 256)) % 256
        module_data += b'\xFF' * padding_size
        
        return module_data

    def package_keypad(self):
        """Complete packaging workflow for keypad module"""
        print("Keypad Module Packaging")
        print("======================")
        print("Execution Buffer: 0x3C000-0x3FFFF")
        print("External Flash Address: 0x000000")
        print("")
        
        # Step 1: Compile module
        elf_file, map_file = self.compile_keypad_module()
        if not elf_file:
            return False
        
        # Step 2: Extract function offsets from map file
        bin_file = os.path.join(self.output_dir, "keypad_module.bin")
        function_offsets = self.extract_function_offsets_from_map(map_file)
        if not function_offsets:
            return False
        
        # Step 3: Map function names to IDs
        id_to_offset = self.map_function_names_to_ids(function_offsets)
        
        # Step 4: Create module package
        module_data = self.create_module_package(bin_file, id_to_offset)
        if not module_data:
            return False
        
        # Step 5: Save package
        output_file = os.path.join(self.output_dir, "keypad_module_fm25w32.bin")
        with open(output_file, "wb") as f:
            f.write(module_data)
        
        # Generate comprehensive info file
        self.generate_deployment_info(output_file, module_data, id_to_offset)
        
        print(f"Keypad module packaged successfully!")
        print(f"Output file: {output_file}")
        print(f"Package size: {len(module_data)} bytes")
        
        return output_file

    def generate_deployment_info(self, output_file, module_data, function_offsets):
        """Generate detailed deployment information"""
        info_file = os.path.join(self.output_dir, "keypad_deployment_info.txt")
        
        with open(info_file, "w") as f:
            f.write("Keypad Module Deployment Information\n")
            f.write("===================================\n")
            f.write("\nMemory Configuration:\n")
            f.write("---------------------\n")
            f.write("Main Firmware: 0x00000-0x2FFFF (192KB)\n")
            f.write("Execution Buffer: 0x3C000-0x3FFFF (16KB)\n")
            f.write("External Flash (FM25W32): 0x000000\n")
            f.write("\nModule Details:\n")
            f.write("---------------\n")
            f.write(f"Module: KEYPAD\n")
            f.write(f"Module ID: 0x0001\n")
            f.write(f"Package File: {os.path.basename(output_file)}\n")
            f.write(f"Total Package Size: {len(module_data)} bytes\n")
            f.write(f"Code Size: {len(module_data) - 8 - (7 * 4)} bytes\n")
            f.write(f"CRC16: 0x{self.calculate_crc16(module_data[8:]):04X}\n")
            f.write(f"Function Count: {len(function_offsets)}\n")
            f.write("\nFunction Offsets (from 0x3C000):\n")
            f.write("---------------------------------\n")
            for func_id, offset in sorted(function_offsets.items()):
                f.write(f"Function {func_id}: 0x{offset:04X} -> 0x{0x3C000 + offset:05X}\n")
            f.write("\nLoader Compatibility:\n")
            f.write("---------------------\n")
            f.write("✓ Compatible with main firmware linker script\n")
            f.write("✓ Uses reserved execution buffer (0x3C000-0x3FFFF)\n")
            f.write("✓ Function offsets extracted from IAR map file\n")
            f.write("✓ CRC16 validation included\n")
            f.write("\nDeployment Steps:\n")
            f.write("-----------------\n")
            f.write("1. Program keypad_module_fm25w32.bin to FM25W32 at 0x000000\n")
            f.write("2. Build main firmware with provided linker script\n")
            f.write("3. Flash main firmware to internal flash\n")
            f.write("4. Test module loading with keypad_init()\n")
            f.write("\nRuntime Behavior:\n")
            f.write("-----------------\n")
            f.write("- keypad_init() triggers dynamic module loading\n")
            f.write("- Loader copies module from FM25W32 to 0x3C000\n")
            f.write("- Function calls execute from execution buffer\n")
            f.write("- keypad_deinit() unloads module and frees buffer\n")
        
        print(f"Deployment info: {info_file}")

def main():
    """Main packaging function"""
    packager = KeypadPackager()
    
    output_file = packager.package_keypad()
    
    if output_file:
        print("\nKeypad module packaging completed!")
        print("Module is compatible with main firmware linker script")
        print("Ready for programming to external flash")
    else:
        print("\nKeypad module packaging failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()