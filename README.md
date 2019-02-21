system :- 
```bash
$ uname -a
Linux block1 4.20.3-200.fc29.x86_64 #1 SMP Thu Jan 17 15:19:35 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux

$ cat /etc/redhat-release
Fedora release 29 (Twenty Nine)
```

- compile the server :- 
```bash
gcc myserver.c -o myserver -lpthread
```

- compile the client :- 
```bash
gcc myclient.c -o myclient
```

- run the server :- 
```bash
./myserver
```

- run the client :- 
```bash
./myclient
```

We can see that myserver is blocked until client closes the connection
which happens after 20 second (sleep 20).

In the test program the server sets the LOWAT to 80 byte and client just sends
23 byte ("Hi Pawan Prakash Sharma") of data. The server requests to read 20 
byte of data only.

recv(myClient_s, buf, 20, MSG_WAITALL)

Since client has already sends 23 byte of data, recv call should return immediately,
but it waits till the client closes the connection which happens after 20 seconds as
I have put sleep of 20 second before closing the connection in the client. In case
of connection is there for longer time, this recv call will never return and we will
be blocked forever.

There are 2 scenarios as described below :-

Test case - 1
- start client and server, with server reading the data after a sleep of 1s. 
- client sends 23 bytes
- server wakes up from 1s sleep.
- server requests 23 bytes (passed)

The above test cases passes both in fedora and ubuntu. 

Test case - 2
- start client and server, with client having a sleep of 1s before sending the packet.
- server requests 20 bytes (blocked)
- client sends 23 bytes
- server request for 20 bytes (unblocked -- only in case Ubuntu. remains in blocked state on Fedora)

The above test cases passes in Ubuntu and fails with fedora. 
