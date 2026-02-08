import struct

MAGIC = 0x4D4D
MODULE_ID = 0

def crc16(data):
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc <<= 1
                crc &= 0xFFFF
    return crc


with open("module.bin", "rb") as f:
    code = f.read()

code_size = len(code)
crc = crc16(code)
func_count = 4   # change per module

header = struct.pack(
    "<HHHBBI",   # little endian
    MAGIC,
    code_size,
    crc,
    func_count,
    MODULE_ID,
    0
)

with open("module_image.bin", "wb") as f:
    f.write(header)
    f.write(code)

print("Done.")
print("Size:", code_size)
print("CRC :", hex(crc))
