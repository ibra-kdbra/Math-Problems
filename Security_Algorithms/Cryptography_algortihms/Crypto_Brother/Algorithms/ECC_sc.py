from ecies.utils import generate_eth_key, generate_key
from ecies import encrypt, decrypt
import time
#HEXADECIMAL MODE
eth_k = generate_eth_key()
sk_hex = eth_k.to_hex()  # hex string
pk_hex = eth_k.public_key.to_hex()  # hex string
#ENCRYPTION
inicio_de_encrypt = time.time()
org_file=open("test_file.rar","rb")
new_file=open("Topocrypt.rar","+wb")
data = org_file.read()
encrypted_data=encrypt(pk_hex, data)
new_file.write(encrypted_data)
org_file.close
new_file.close
final_de_encrypt=time.time()
print('tiempo de encriptado: ', final_de_encrypt - inicio_de_encrypt)

#DECRYPTION
inicio_unencrypt=time.time()
org_file=open("Topocrypt.rar","rb")
new_file=open("Topodecrypt.rar","+wb")
data = org_file.read()
decrypted_data=decrypt(sk_hex, data)
new_file.write(decrypted_data)
org_file.close
new_file.close
final_unencrypt=time.time()
print('tiempo de desencriptado: ', final_unencrypt - inicio_unencrypt)

#decrypt(sk_hex, encrypt(pk_hex, data))

#BYTES MODE
#secp_k = generate_key()
#sk_bytes = secp_k.secret  # bytes
#pk_bytes = secp_k.public_key.format(True)  # bytes
#decrypt(sk_bytes, encrypt(pk_bytes, data))