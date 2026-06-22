# 💬 ft_irc - Custom IRC Server

An Internet Relay Chat (IRC) server built entirely from scratch in C++98. This project demonstrates low-level network programming, single-threaded concurrency handling, and strict adherence to protocol standards.

By utilizing non-blocking sockets and I/O multiplexing, the server is capable of efficiently managing multiple simultaneous client connections (such as Irssi or Netcat) without the need for multithreading.

---

## 🚀 Core Features

* **100% C++98 Compliant:** Strict adherence to legacy C++ standards with no external libraries.
* **I/O Multiplexing:** Utilizes `poll()` to handle multiple file descriptors concurrently in a single thread.
* **Non-Blocking Sockets:** Completely non-blocking reads and writes, ensuring the server never hangs on a slow client connection.
* **Real Client Compatibility:** Fully tested and compatible with standard IRC clients like **Irssi** and **HexChat**.
* **Robust Channel Management:** Supports channel creation, user limits, operator privileges, and standard modes (`i`, `t`, `k`, `o`, `l`).

---

## 🧠 System Architecture

The project is strictly divided into two primary subsystems to ensure clean separation of concerns and prevent memory leaks:

1.  **The Network Core:** Handles all raw system calls (`socket`, `bind`, `listen`, `accept`). It manages the infinite `poll()` loop, tracks client states, and carefully appends incoming raw bytes into dedicated client buffers until a complete `\r\n` terminated line is formed.
2.  **The Protocol Engine:** Responsible for parsing raw strings into actionable commands. It handles user authentication (`PASS`, `NICK`, `USER`), routes messages across channels (`PRIVMSG`), enforces channel operator logic, and generates accurately formatted RFC 2812 numerical replies.

---

## 🛠️ Compilation & Usage

### 1. Build the project
Clone the repository and run `make` to compile the server executable.

```bash
git clone git@github.com:fog-3/ft_irc.git
cd ft_irc
make
```

### 2. Start the Server

The server requires two arguments: a port to listen on and a password for client authentication.

```bash
./ircserv <port> <password>
# Example: ./ircserv 6667 mypassword
```

### 3. Connect as a Client

You can test the server using `netcat` for raw commands or `irssi` for a graphical terminal experience.

**Using Netcat (for debugging):**

```bash
nc -c 127.0.0.1 6667
# Type: PASS mypassword
# Type: NICK yourname
# Type: USER yourname 0 * :realname
```

**Using Irssi:**

```bash
irssi
# Inside irssi type:
/connect 127.0.0.1 6667 mypassword
```

## 🤝 Team organization

### Role 1: The C++ Engine & IRC Protocol

Responsabilities of this role:
- **The parser:** writing the logic to take a raw string `JOIN #general` and slice it into a Command, Target, and Parameter. This requires heavy use of `std::string` methods, iterators, and vectors.

- **The Command Factory:** Building the routing logic. When a parsed command arrives, he will use maps or function pointers to trigger the correct execution function (e.g., `executeJoin()`, `executePrivmsg()`).

- **The Numeric Replies (RPLs):** The IRC protocol requires specific, highly formatted string responses (like `001 RPL_WELCOME` or `404 ERR_CANNOTSENDTOCHAN`). This role can build a dedicated class to generate these cleanly.

- **Channel Logic:** managing the data structures for channels, checking who is an operator, and applying channel modes.

### Role 2: System Core & Network Architecture

- **The Multiplexer (poll):** Setting up the non-blocking sockets and the infinite loop that monitors all file descriptors. You decide when to read from a client and when it is safe to write to them.

- **Buffer Management (Crucial):** Network streams are unpredictable. You will read raw bytes and append them to a specific client's buffer. You must hunt for the \r\n delimiter. When you find it, you extract that complete line and hand it over to the parser.

- **Client State Machine:** Tracking whether a client has just connected, has sent their PASS, has fully registered with NICK/USER, or is ready to chat.

- **Memory & Disconnect Handling:** Ensuring that when a client drops the connection (Ctrl+C), their file descriptor is closed, their data is wiped from all channels, and absolutely no memory leaks occur.
