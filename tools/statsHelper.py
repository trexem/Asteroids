import json
import sys
import argparse

# Use the same XOR_KEY as in your C++ code
XOR_KEY = 0xEF

def obfuscate(data):
    """Obfuscate data using the same XOR algorithm as C++ code"""
    return bytes([c ^ XOR_KEY for c in data.encode('utf-8')])

def deobfuscate(data):
    """Deobfuscate data using the same XOR algorithm as C++ code"""
    return bytes([c ^ XOR_KEY for c in data]).decode('utf-8')

def process_file(input_file, output_file, action):
    """Process the file with either obfuscation or deobfuscation"""
    try:
        with open(input_file, 'rb') as f:
            data = f.read()
        
        if action == 'deobfuscate':
            # Deobfuscate and pretty-print JSON
            json_data = json.loads(deobfuscate(data))
            result = json.dumps(json_data, indent=4)
            output_ext = '.json'
        else:
            # Obfuscate (read from JSON file)
            json_data = json.loads(data.decode('utf-8'))
            result = obfuscate(json.dumps(json_data))
            output_ext = '.dat'
        
        # Determine output filename if none specified
        if not output_file:
            if action == 'deobfuscate':
                output_file = input_file.replace('.dat', '.json')
            else:
                output_file = input_file.replace('.json', '.dat')
        
        with open(output_file, 'wb' if action == 'obfuscate' else 'w') as f:
            f.write(result)
        
        print(f"Success! {action}d file saved to {output_file}")
    
    except Exception as e:
        print(f"Error: {str(e)}")
        sys.exit(1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Obfuscate/deobfuscate game stats files',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''Examples:
  Deobfuscate: python game_stats_helper.py -i stats.dat -o readable.json -d
  Obfuscate:   python game_stats_helper.py -i edited.json -o stats.dat -o
''')
    parser.add_argument('-i', '--input', required=True, help='Input file')
    parser.add_argument('-o', '--output', help='Output file (optional)')
    parser.add_argument('-d', '--deobfuscate', action='store_true', help='Deobfuscate file')
    parser.add_argument('-b', '--obfuscate', action='store_true', help='Obfuscate file')
    
    args = parser.parse_args()
    
    if not (args.deobfuscate ^ args.obfuscate):
        print("Error: Please specify either --deobfuscate or --obfuscate")
        sys.exit(1)
    
    action = 'deobfuscate' if args.deobfuscate else 'obfuscate'
    process_file(args.input, args.output, action)
    