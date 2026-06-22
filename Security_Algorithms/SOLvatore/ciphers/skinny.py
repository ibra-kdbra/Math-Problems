from cipher_description import CipherDescription


def generate_skinny_version(wordsize, rounds):
    # State
    # 0  1   2  3
    # 4  5   6  7
    # 8  9  10 11
    # 12 13 14 15
    if wordsize == 4:
        skinny_sbox = [
            0xC,
            0x6,
            0x9,
            0x0,
            0x1,
            0xA,
            0x2,
            0xB,
            0x3,
            0x8,
            0x5,
            0xD,
            0x4,
            0xE,
            0x7,
            0xF,
        ]
    elif wordsize == 8:
        skinny_sbox = [
            0x65,
            0x4C,
            0x6A,
            0x42,
            0x4B,
            0x63,
            0x43,
            0x6B,
            0x55,
            0x75,
            0x5A,
            0x7A,
            0x53,
            0x73,
            0x5B,
            0x7B,
            0x35,
            0x8C,
            0x3A,
            0x81,
            0x89,
            0x33,
            0x80,
            0x3B,
            0x95,
            0x25,
            0x98,
            0x2A,
            0x90,
            0x23,
            0x99,
            0x2B,
            0xE5,
            0xCC,
            0xE8,
            0xC1,
            0xC9,
            0xE0,
            0xC0,
            0xE9,
            0xD5,
            0xF5,
            0xD8,
            0xF8,
            0xD0,
            0xF0,
            0xD9,
            0xF9,
            0xA5,
            0x1C,
            0xA8,
            0x12,
            0x1B,
            0xA0,
            0x13,
            0xA9,
            0x05,
            0xB5,
            0x0A,
            0xB8,
            0x03,
            0xB0,
            0x0B,
            0xB9,
            0x32,
            0x88,
            0x3C,
            0x85,
            0x8D,
            0x34,
            0x84,
            0x3D,
            0x91,
            0x22,
            0x9C,
            0x2C,
            0x94,
            0x24,
            0x9D,
            0x2D,
            0x62,
            0x4A,
            0x6C,
            0x45,
            0x4D,
            0x64,
            0x44,
            0x6D,
            0x52,
            0x72,
            0x5C,
            0x7C,
            0x54,
            0x74,
            0x5D,
            0x7D,
            0xA1,
            0x1A,
            0xAC,
            0x15,
            0x1D,
            0xA4,
            0x14,
            0xAD,
            0x02,
            0xB1,
            0x0C,
            0xBC,
            0x04,
            0xB4,
            0x0D,
            0xBD,
            0xE1,
            0xC8,
            0xEC,
            0xC5,
            0xCD,
            0xE4,
            0xC4,
            0xED,
            0xD1,
            0xF1,
            0xDC,
            0xFC,
            0xD4,
            0xF4,
            0xDD,
            0xFD,
            0x36,
            0x8E,
            0x38,
            0x82,
            0x8B,
            0x30,
            0x83,
            0x39,
            0x96,
            0x26,
            0x9A,
            0x28,
            0x93,
            0x20,
            0x9B,
            0x29,
            0x66,
            0x4E,
            0x68,
            0x41,
            0x49,
            0x60,
            0x40,
            0x69,
            0x56,
            0x76,
            0x58,
            0x78,
            0x50,
            0x70,
            0x59,
            0x79,
            0xA6,
            0x1E,
            0xAA,
            0x11,
            0x19,
            0xA3,
            0x10,
            0xAB,
            0x06,
            0xB6,
            0x08,
            0xBA,
            0x00,
            0xB3,
            0x09,
            0xBB,
            0xE6,
            0xCE,
            0xEA,
            0xC2,
            0xCB,
            0xE3,
            0xC3,
            0xEB,
            0xD6,
            0xF6,
            0xDA,
            0xFA,
            0xD3,
            0xF3,
            0xDB,
            0xFB,
            0x31,
            0x8A,
            0x3E,
            0x86,
            0x8F,
            0x37,
            0x87,
            0x3F,
            0x92,
            0x21,
            0x9E,
            0x2E,
            0x97,
            0x27,
            0x9F,
            0x2F,
            0x61,
            0x48,
            0x6E,
            0x46,
            0x4F,
            0x67,
            0x47,
            0x6F,
            0x51,
            0x71,
            0x5E,
            0x7E,
            0x57,
            0x77,
            0x5F,
            0x7F,
            0xA2,
            0x18,
            0xAE,
            0x16,
            0x1F,
            0xA7,
            0x17,
            0xAF,
            0x01,
            0xB2,
            0x0E,
            0xBE,
            0x07,
            0xB7,
            0x0F,
            0xBF,
            0xE2,
            0xCA,
            0xEE,
            0xC6,
            0xCF,
            0xE7,
            0xC7,
            0xEF,
            0xD2,
            0xF2,
            0xDE,
            0xFE,
            0xD7,
            0xF7,
            0xDF,
            0xFF,
        ]

    # ShiftRows
    shiftrows = []
    for bit in range(wordsize):
        # Second Row
        shiftrows.append(
            [
                "s{}".format(wordsize * 4 + bit),
                "s{}".format(wordsize * 5 + bit),
                "s{}".format(wordsize * 6 + bit),
                "s{}".format(wordsize * 7 + bit),
            ]
        )
        # Third Row
        shiftrows.append(
            ["s{}".format(wordsize * 8 + bit), "s{}".format(wordsize * 10 + bit)]
        )
        shiftrows.append(
            ["s{}".format(wordsize * 9 + bit), "s{}".format(wordsize * 11 + bit)]
        )

        # Fourth Row
        shiftrows.append(
            [
                "s{}".format(wordsize * 12 + bit),
                "s{}".format(wordsize * 15 + bit),
                "s{}".format(wordsize * 14 + bit),
                "s{}".format(wordsize * 13 + bit),
            ]
        )

    skinny = CipherDescription(wordsize * 16)
    skinny.add_sbox("S-box", skinny_sbox)

    # SubCells
    for word in range(16):
        bits = ["s{}".format(wordsize * word + i) for i in range(wordsize)]
        skinny.apply_sbox("S-box", bits, bits)

    # ShiftRows
    for shift in shiftrows:
        skinny.apply_permutation(shift)

    # MixColumns
    for col in range(4):
        for bit in range(wordsize):
            x0 = "s{}".format(bit + col * wordsize)
            x1 = "s{}".format(bit + 4 * wordsize + col * wordsize)
            x2 = "s{}".format(bit + 4 * 2 * wordsize + col * wordsize)
            x3 = "s{}".format(bit + 4 * 3 * wordsize + col * wordsize)

            skinny.apply_xor(x1, x2, x1)
            skinny.apply_xor(x2, x0, x2)
            skinny.apply_xor(x2, x3, x3)

            skinny.apply_permutation([x0, x1, x2, x3])

    return skinny


skinny64 = generate_skinny_version(4, 32)
skinny128 = generate_skinny_version(8, 40)
