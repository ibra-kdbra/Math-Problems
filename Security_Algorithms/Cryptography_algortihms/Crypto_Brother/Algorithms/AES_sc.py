from Crypto.Cipher import AES
from random import randbytes as rng
def encrypt(key, data):
    cipher = AES.new(key, AES.MODE_EAX)
    ciphertext, tag = cipher.encrypt_and_digest(data)

    return cipher.nonce + tag + ciphertext

def decrypt(key, data):
    nonce = data[:AES.block_size]
    tag = data[AES.block_size:AES.block_size * 2]
    ciphertext = data[AES.block_size * 2:]
    cipher = AES.new(key, AES.MODE_EAX, nonce)

    return cipher.decrypt_and_verify(ciphertext, tag)

def generate_key(size):
    return rng(int(size))