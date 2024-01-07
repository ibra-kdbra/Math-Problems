import hashlib
import time
from Crypto.Hash import SHA256, MD5
from time import ctime, time
import os
def MD5_sc(data):
    log_start_time=ctime(time())
    size=len(data)
    log="starting MD5 encode of %d bytes at %s\n" % (size, log_start_time)
    open(os.path.dirname(os.path.dirname(__file__))+'/'+'synacrypto_systems.log', 'at').write(log)
    return hashlib.md5(data).hexdigest()

def SHA256_sc(data):
    log_start_time=ctime(time())
    size=len(data)
    log="starting sha256 encode of %d bytes at %s\n" % (size, log_start_time)
    open(os.path.dirname(os.path.dirname(__file__))+'/'+'synacrypto_systems.log', 'at').write(log)
    return hashlib.sha256(data).hexdigest()

def SHA256_pcd(data):
    log_start_time=ctime(time())
    size=len(data)
    log="starting MD5 Pycryptodome encode of %d bytes at %s\n" % (size, log_start_time)
    open(os.path.dirname(os.path.dirname(__file__))+'/'+'synacrypto_systems.log', 'at').write(log)
    return SHA256.new(data)

def MD5_pcd(data):
    log_start_time=ctime(time())
    size=len(data)
    log="starting MD5 encode of %d bytes at %s\n" % (size, log_start_time)
    open(os.path.dirname(os.path.dirname(__file__))+'/keys/'+'synacrypto_systems.log', 'at').write(log)
    return MD5.new(data)