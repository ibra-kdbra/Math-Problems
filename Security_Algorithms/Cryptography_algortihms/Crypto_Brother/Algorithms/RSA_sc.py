from Crypto.Signature import pkcs1_15
from Crypto.PublicKey import RSA
from Crypto_Brother.Hash import *
from Crypto_Brother.Tools import switch
from Crypto_Brother.Settings import *

import os
import re
import configparser
from random import randint

def generate_priv_key(bits, mode, *args):
    if bits > 1023 and (bits % 256) == 0:
        exp=randint(65537, 99999)
        if (exp%2)==0: exp+=1
        key= RSA.generate(bits, randfunc=None,e=exp)
        if mode == 'file':
            if len(args) == 0: return 255
            name=MD5_sc(bytes(os.path.basename(args[0]), 'utf-8'))
            if os.path.dirname(args[0])=='': path=keysett.path+"/"
            else : path=os.path.dirname(args[0])+'/'
            configs=configparser.ConfigParser()
            configs.read(os.path.dirname(os.path.dirname(__file__))+'/Settings/'+'settings.ini')
            if len(args) > 2 and args[1]=='DER': tag=configs['RSA_sc']['BIN_TYPE']
            else: tag=configs['RSA_sc']['TEXT_TYPE']
            with switch(len(args)) as s:
                if s.case(1): open(path+name+tag,'+wb').write(key.export_key('PEM'));return path+name+tag
                if s.case(2): open(path+name+tag,'+wb').write(key.export_key(format=args[1]));return path+name+tag
                if s.case(3): open(path+name+tag,'+wb').write(key.export_key(format=args[1], passphrase=args[2]));return path+name+tag
                if s.case(4): open(path+name+tag,'+wb').write(key.export_key(format=args[1], passphrase=args[2], pkcs=args[3]));return path+name+tag
                if s.case(6): open(path+name+tag,'+wb').write(key.export_key(format=args[1], passphrase=args[2], pkcs=args[3], protection=args[4]));return path+name+tag
                if s.case(6): open(path+name+tag,'+wb').write(key.export_key(format=args[1], passphrase=args[2], pkcs=args[3], protection=args[4], randfunc=[5]));return path+name+tag
        if mode == 'var':
            return key

def import_key(file, struct, password):
    name=os.path.basename(file)
    if os.path.dirname(file)=='': path=keysett.path
    else : path=os.path.dirname(file)+'/'
    print(path+'/'+name)
    if struct=='text':
        if password=='':password=None
        return RSA.import_key(open(path+'/'+name, 'rt').read(), passphrase=password)
    elif struct=='bin':
        if password=='':password=None
        return RSA.import_key(open(path+'/'+name, 'rb').read(), passphrase=password)
    else:
        if password=='':password=None
        configs=configparser.ConfigParser()
        configs.read(os.path.dirname(os.path.dirname(__file__))+'/Settings/'+'settings.ini')
        if re.search(configs['RSA_sc']['TEXT_TYPE'], name):
            return RSA.import_key(open(path+'/'+name, 'rt').read(), passphrase=password)
        elif re.search(configs['RSA_sc']['BIN_TYPE'], name):
            return RSA.import_key(open(path+'/'+name, 'rb').read(), passphrase=password)
        else: return 255

def generate_pub_key(priv_key, mode, *args):
    if mode=='file':
        if len(args) == 0: return 255
        name=MD5_sc(bytes(os.path.basename(args[0]), 'utf-8'))
        if os.path.dirname(args[0])=='': path=filesett.path+'/'
        else : path=os.path.dirname(args[0])+'/'
        configs=configparser.ConfigParser()
        configs.read(os.path.dirname(os.path.dirname(__file__))+'/Settings/'+'settings.ini')
        if len(args) > 2 and args[1]=='DER': tag=configs['RSA_sc']['BIN_TYPE']
        else: tag=configs['RSA_sc']['TEXT_TYPE']
        with switch(len(args)) as s:
            if s.case(1): open(path+name+tag,'+wb').write(priv_key.publickey().export_key('PEM'));return path+name+tag
            if s.case(2): open(path+name+tag,'+wb').write(priv_key.publickey().export_key(format=args[1]));return path+name+tag
            if s.case(3): open(path+name+tag,'+wb').write(priv_key.publickey().export_key(format=args[1], passphrase=args[2]));return path+name+tag
            if s.case(4): open(path+name+tag,'+wb').write(priv_key.publickey().export_key(format=args[1], passphrase=args[2], pkcs=args[3]));return path+name+tag
            if s.case(6): open(path+name+tag,'+wb').write(priv_key.publickey().export_key(format=args[1], passphrase=args[2], pkcs=args[3], protection=args[4]));return path+name+tag
            if s.case(6): open(path+name+tag,'+wb').write(priv_key.publickey().export_key(format=args[1], passphrase=args[2], pkcs=args[3], protection=args[4], randfunc=[5]));return path+name+tag
        return path+name+tag
    if mode=='var':
        return priv_key.publickey()

def sign_keys(key, struct, mode, password):
    test=b'message to sign'
    if struct=='file':
        keys=import_key(key, mode, password)
    if struct=='var':
        keys=key
    signature=pkcs1_15.new(keys).sign(SHA256_pcd(test))
    return signature

def verify_signature(key, signature):
    test=b'message to sign'
    try:
        pkcs1_15.new(key).verify(SHA256_pcd(test), signature)
        return 1
    except (ValueError, TypeError):
        return 0