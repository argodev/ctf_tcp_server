"""
A 'simple' CTF server.  Win by executing commands on the server!

Below is a very basic client to get you started.  Have fun!
"""
import struct
import socket

if __name__ == '__main__':
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    s.connect(('localhost', 8888))
    #All commands are 24 bytes long
    #  The command number
    #  arg1 (possibly unused)
    #  arg2 (possibly unused)
    s.sendall(struct.pack('QQQ', 0x61, 0, 0x1000)) #allocate
    buf = s.recv(24, socket.MSG_WAITALL)
    print(repr(buf))
