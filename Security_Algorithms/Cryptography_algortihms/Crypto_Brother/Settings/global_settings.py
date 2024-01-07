import sys
import re
import os
from getpass import getuser

class keysett():
    OS=sys.platform
    #You can change the config variables and methods from here
    path=0
    defhash=0
    #Windows OS
    if re.search("win32", OS) or re.search("cygwin", OS):
        if not path:path="C:\\users\\"+getuser()+"\\desktop"
    #Linux OS
    elif re.search("linux", OS):
        if not path:path="/home/"+getuser()
    #If you want to add MAC OS you can use the next condition:
    #elif re.search("darwin", OS):
    else :
        print("error OS not supported, this app only supports windows/linux OS")

class filesett():
    OS=sys.platform
    #You can change the config variables and methods from here
    path=0
    #Windows OS
    if re.search("win32", OS) or re.search("cygwin", OS):
        if not path:path="C:\\users\\"+getuser()+"\\desktop"
    #Linux OS
    elif re.search("linux", OS):
        if not path:path="/home/"+getuser()
    #If you want to add MAC OS you can use the next condition:
    #elif re.search("darwin", OS):
    else :
        print("error OS not supported, this app only supports windows/linux OS")

class logsett():
    OS=sys.platform
    #You can change the config variables and methods from here
    path=0
    name="Sinacrypto_history.log"
    #Windows OS
    if re.search("win32", OS) or re.search("cygwin", OS):
        if not path:path="C:\\users\\"+getuser()+"\\desktop"
    #Linux OS
    elif re.search("linux", OS):
        if not path:path="/home/"+getuser()
    #If you want to add MAC OS you can use the next condition:
    #elif re.search("darwin", OS):
    else :
        print("error OS not supported, this app only supports windows/linux OS")
if __name__ == "__main__":
    a=1