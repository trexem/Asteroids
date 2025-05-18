from dotenv import load_dotenv
import zipfile
import os
import io

load_dotenv()

HEADER = os.getenv("HEADER").encode()          # Custom 4-byte header
KEY = int(os.getenv("KEY"), 0)                 # XOR key
print(KEY)
print(HEADER)
INPUT_FOLDER = 'data'
OUTPUT_FILE = 'data.bin'

def zip_data(folder):
    zip_bytes = io.BytesIO()
    with zipfile.ZipFile(zip_bytes, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(folder):
            for file in files:
                path = os.path.join(root, file)
                arcname = os.path.relpath(path, folder)
                zipf.write(path, arcname)
    return zip_bytes.getvalue()

def xor_obfuscate(data, key):
    return bytes([b ^ key for b in data])

def build_bin():
    zip_data_raw = zip_data(INPUT_FOLDER)
    obfuscated = xor_obfuscate(zip_data_raw, KEY)
    with open(OUTPUT_FILE, 'wb') as f:
        f.write(HEADER)
        f.write(obfuscated)
    print(f"[OK] Obfuscated {INPUT_FOLDER}/ into {OUTPUT_FILE}")

if __name__ == '__main__':
    build_bin()
    