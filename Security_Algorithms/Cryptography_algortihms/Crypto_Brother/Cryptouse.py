from Crypto_Brother.Algorithms import AES_sc
from Crypto_Brother.Algorithms import RSA_sc
#from Sinacrypto.Algorithms import ECC_sc as ECC
from Crypto_Brother.Hash.Hash_sc import *
from Crypto_Brother.Tools import *
from Crypto_Brother.Settings import *
from Crypto.Cipher import PKCS1_OAEP

import os
import re
import configparser
from time import *
from random import randbytes as rng, randint as dice

def RSA_AL_key_alg(bits, priv_name, pub_name, only_pub):
    randnum = dice(0, 4)
    files=[]
    for false_keys in range(0, 5):
        priv_key=RSA_sc.generate_priv_key(bits,'var',priv_name+str(false_keys))
        files.append(RSA_sc.generate_pub_key(priv_key, 'file', pub_name+str(false_keys)))
        if false_keys == randnum and only_pub==1:
            priv_key=RSA_sc.import_key(priv_name, 'text', '')
            files.append(RSA_sc.generate_pub_key(priv_key, 'file', pub_name+'appended'+str(false_keys)))
        if false_keys == randnum and only_pub==0:
            open(keysett.path+'/'+MD5_sc(bytes(os.path.basename(priv_name), 'utf-8'))+'.pem.priv', '+wb').write(priv_key.export_key('PEM'))
    if only_pub:
        signature=RSA_sc.sign_keys(priv_name, 'file', 'text', '')
    else:
        signature=RSA_sc.sign_keys(keysett.path+'/'+MD5_sc(bytes(os.path.basename(priv_name), 'utf-8'))+'.pem.priv', 'file', 'text', '')
    print(files)
    for key_file in files:
        key=RSA_sc.import_key(key_file, 'text', '')
        if RSA_sc.verify_signature(key, signature):
            print("pub key founded")
            open(keysett.path+'/'+MD5_sc(bytes(pub_name, 'utf-8'))+'.pub', '+wb').write(open(key_file, 'rb').read())
            os.remove(key_file)
        else:
            os.remove(key_file)

def PKCS1_OAEP_encrypt(msg,msg_type,key_file):
    configs=configparser.ConfigParser()
    configs.read(os.path.dirname(__file__)+'/Settings/'+'settings.ini')
    key=RSA_sc.import_key(key_file, 'text', '')
    print(key)
    if msg_type == 'text':
        tag=configs['RSA_sc']['MES_TYPE']
        message=bytes(msg, 'utf8')
        path=filesett.path
        name=MD5_sc(message)
        size=len(message)
        crypt_file=(path+name+tag, '+wb')
    if msg_type == 'file':
        tag=configs['RSA_sc']['FILE_TYPE']
        path=os.path.dirname(msg)+"/"
        if path=='':filesett.path+"/"
        name=os.path.basename(msg)
        size=file_size(path, name)
        message=open(path+'/'+name, 'rb')
        crypt_file=open(path+'/'+MD5_sc(bytes(name, 'utf-8'))+tag, '+wb')
    loop_cnty=(int(size/80)+1)
    encrypt_content=PKCS1_OAEP.new(key)
    print(path, name)
    for loop in range(0, loop_cnty):
        print('in loop ', loop, ' of ', loop_cnty)
        if size > 79:
            if msg_type == 'file':
                crypt_file.write(encrypt_content.encrypt(message.read(80)))
            if msg_type == 'text':
                concatenated_chars=''
                for char_src in range((loop*80),((80*(loop+1))-1)):
                    concatenated_chars+=bytes(chr(message[char_src]), 'utf-8')
                crypt_file.write(encrypt_content.encrypt(concatenated_chars))
            size=size-80
        else:
            if msg_type == 'file':
                crypt_file.write(encrypt_content.encrypt(message.read()))
            if msg_type == 'text':
                concatenated_chars=''
                for char_src in range((loop*80),((loop*80)+size)):
                    concatenated_chars+=bytes(chr(message[char_src]), 'utf-8')
                crypt_file.write(encrypt_content.encrypt(concatenated_chars))

def PKCS1_OAEP_decrypt(encrypted_file, decrypt_type, key):
    if key == '':
        for file in os.listdir(keysett.path):
            if re.search(".priv", file):
                key = RSA_sc.import_key(file, 'text' , '')
                break
    if decrypt_type == '':
        configs=configparser.ConfigParser()
        configs.read(os.path.dirname(__file__)+'/Settings/'+'settings.ini')
        if re.search(configs['RSA_sc']['FILE_TYPE'], encrypted_file):
            decrypt_type='file'
        elif re.search(configs['RSA_sc']['MES_TYPE'], encrypted_file):
            decrypt_type='text'
        else: return 4

    if os.path.dirname(encrypted_file) ==  '': path=filesett.path+'/'
    else: path=os.path.dirname(encrypted_file)+'/'
    name=os.path.basename(encrypted_file)
    size=file_size(path, name)
    loopcnty=(int(size/1024)+1)

    for loop in range(0, loopcnty):
        if decrypt_type == 'text':
            decrypt=''
        if decrypt_type == 'file':
            return 0
    return decrypt

#SYMETRIC ALGORITHMS, you can add here your function
def AES_log_mode_encrypt(msg, msg_type, mode):
    #Configs
    configs=configparser.ConfigParser()
    configs.read(os.path.dirname(__file__)+'/settings/'+'settings.ini')
    start_time=time()                                                       #Start time
    if mode == 256 or mode == 128 or mode == 64:
        key=AES_sc.generate_key(mode/8)
    else: return 1                                                          #Fail reason invalid mode or none one!
    tag=bytes(configs['AES_log_mode']['TAG'], 'utf8')
    if msg_type == "file":
        name=os.path.basename(msg)
        if os.path.dirname(msg) == '': path=filesett.path
        else: path=os.path.dirname(msg)
        size=file_size(path+'/', name)
        extension=configs['AES_log_mode']['FILE_TYPE']
        content=open(path+'/'+name, 'rb').read()
    elif msg_type == "text":
        name=MD5_sc(rng(32))
        path=filesett.path
        extension=configs['AES_log_mode']['MES_TYPE']
        content=bytes(msg, 'utf-8')
        size=len(content)
    else :  return 2                                                        #Fail reason, invalid message type or non one

    #Log section
    log_start_time=ctime(time())
    log="starting AES %d encrypt of %d bytes at %s\n" % (mode, size, log_start_time)
    open(logsett.path+'/'+logsett.name, 'at').write(log)

    #Encrypt section
    for line in open(logsett.path+'/'+logsett.name, 'rt').readlines():
        if re.search(log, line):
            log=line
            break
    print("encrypting...")
    encrypt=AES_sc.encrypt(key,content)
    open(path+'/'+MD5_sc(bytes(str(log), 'utf-8'))+extension, '+wb').write(bytes(name, 'utf-8'))
    open(path+'/'+MD5_sc(bytes(str(log), 'utf-8'))+extension, 'ab').write(tag)
    open(path+'/'+MD5_sc(bytes(str(log), 'utf-8'))+extension, 'ab').write(encrypt)

    #Export section
    log_end_time=ctime(time())
    if log_end_time == log_start_time: sleep(1); log_end_time=ctime(time())
    log="ending AES %d encrypt of %d bytes at %s\n" % (mode, size, log_end_time)
    if msg_type == 'file': os.remove(path+'/'+name)
    print("exporting key...")
    open(keysett.path+'/'+MD5_sc(bytes(log, 'utf-8'))+'.key', '+wb').write(key)
    #End section
    open(logsett.path+'/'+logsett.name, 'at').write(log)
    return (path+MD5_sc(bytes(str(log), 'utf-8'))+extension, os.path.dirname(__file__))

def AES_log_mode_decrypt(msg, msg_type, raw_log):
    configs=configparser.ConfigParser()
    configs.read(os.path.dirname(__file__)+'/settings/'+'settings.ini')
    tag=bytes(configs['AES_log_mode']['TAG'], 'utf-8')
    old_name=os.path.basename(msg)
    if os.path.dirname(msg) == '': path=filesett.path
    else: path=os.path.dirname(msg)
    if msg_type == '':
        if re.search(configs['AES_log_mode']['FILE_TYPE'], msg):
            msg_type='file'
        if re.search(configs['AES_log_mode']['MES_TYPE'], msg):
            msg_type='text'
    raw_content=open(path+'/'+old_name, 'rb').read()
    new_size=file_size(path+'/',old_name)
    name=''
    print(new_size)
    for i in range(0, new_size):
        s_tag=''
        if chr(raw_content[i]) == chr(tag[0]):
            for j in range(0, len(tag)):
                s_tag=s_tag+chr(raw_content[i+j])
        if s_tag==tag.decode('utf-8'):
            content=b''
            for k in range((i+len(tag)), new_size):
                content=content+bytes([raw_content[k]])
            break
        name=name+chr(raw_content[i])
    part=1
    size=len(content)
    key_file=''
    for line in raw_log.readlines():
        if part==1:
            if re.search(ctime(os.path.getctime(path+"/"+old_name)), line):
                if re.search('starting AES', line):
                    if re.search(str(size-8), line) or re.search(str(size-16), line) or re.search(str(size-32), line):
                        if MD5_sc(bytes(line, 'utf-8'))+configs['AES_log_mode']['MES_TYPE'] == old_name or MD5_sc(bytes(line, 'utf-8'))+configs['AES_log_mode']['FILE_TYPE'] == old_name:
                            print(2)
                            part=2
        if part==2:
            if re.search('ending AES', line):
                if re.search(str(size-8), line) or re.search(str(size-16), line) or re.search(str(size-32), line):
                    key_file=MD5_sc(bytes(line, 'utf-8'))+'.key'
    if key_file=='': return 2
    if msg_type == 'file':
        open(path+'/'+name, '+wb').write(AES_sc.decrypt(open(keysett.path+'/'+key_file, 'rb').read(),content))
    if msg_type == 'text':
        print(str(AES_sc.decrypt(open(keysett.path+'/'+key_file, 'rb').read(),content), 'utf-8'))
    os.remove(keysett.path+'/'+key_file)
    os.remove(path+"/"+old_name)
    return 0