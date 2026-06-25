## Sockets `socket()`
Creates the actual network endpoint. It returns a File Descriptor (FD) that represents your server.

## Setsockopt `setsockopt()`
Absolutely critical. You will use this with the `SO_REUSEADDR` flag. Without it, if the server crashes and you try to restart it immediately, the port will be blocked, and you'll get an "Address already in use" error.

## FNCTL `fcntl()`
You must use this to set the server socket (and all tfuture client sockets) to non-blocking mode.



- close
- setsockeopt
- getsockname
- getprotobyname
- gethostbyname
- getaddrinfo
- freeaddrinfo
- bind
- connect
- listen
- accept
- htons
- htonl
- ntohs
- ntohl
- inet_addr
- inet_ntoa
- inet-ntop
- send
- recv
- signal
- sigaction
- sigemptyset
- sigfillset
- sigaddset
- sigdelset
- sigismember
- lseek
- fstat
- fcntl

## Multiplexer recomendated `poll()`
It is a simple function where you just have to manage a `std::vector<struct pollfd>` and loop through it.

- I/O operations

- IRC clients, we have to choose one as a reference

- The reference client must br able to connect to your server without encountering any error

- Comunication TCP/IP

- You must to be able to authenticate, wich means user identificator and password, set a nickname, a username, join a channel, send and receive private messages using your client reference.

- The messages has to be forwarded to every other client that joined the channel

- Have operators and regular users (role distinction).

- The commands for the channel operators:
    - KICK: Eject a client from the channel
    - INVITE: invite a client to a channel
    - TOPIC: change or view the channel topic.
    - MODE: change the channel's mode:
        - i: set/remove Invite-only channel
        - t: Set/remove the restrictions of the TOPIC command to channel operators
        - k/remove the channel key (password)
        - o: Give/take channel operator privilege
        - l: Set/remove the user limit to channel
