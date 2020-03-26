# ctf_tcp_server
Feel free to look at the example client in `client.py`.

The best challenge is to avoid looking at the server source code.  Use Ghidra
to reverse engineer the binary.

The goal is to run commands on the server.

Good luck!

## Building
Tested on Ubuntu 18.04, 64-bit system:

```sh
gcc ctf_tcp_server.c -lpthread -o server
```

## Running
Run the server, will listen on port 8888:

```sh
./server
```

Connecting with the example client:

```sh
python client.py
```


