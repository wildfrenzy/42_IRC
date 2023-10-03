<div align="center">

# FT_IRC

[About](#about-the-project) •
[Subject requirements](#subject-requirements) •
[Usage](#usage) •
[Useful links](#useful-links)

</div>

## About The Project
It is a group project about creating your own IRC server.
We use an actual IRC client [Irssi v1.4.4](https://irssi.org/) to connect to our server and test it.

**Internet Relay Chat** or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.

## Subject requirements

* Your code must comply with the C++ 98 standard. Then, it should still compile if you add the flag -std=c++98.
* The server must be capable of handling multiple clients at the same time and never
hang.
* Forking is not allowed. All I/O operations must be non-blocking.
* Communication between client and server has to be done via TCP/IP (v4 or v6).
* Using your reference client with your server must be similar to using it with any
  official IRC server. However, you only have to implement the following features:
  * You must be able to authenticate, set a nickname, a username, join a channel,
    send and receive private messages using your reference client.
  * All the messages sent from one client to a channel have to be forwarded to
    every other client that joined the channel.
  * You must have operators and regular users.
  * Then, you have to implement the commands that are specific to channel
    operators:
    * KICK - Eject a client from the channel
    * INVITE - Invite a client to a channel
    * TOPIC - Change or view the channel topic
    * MODE - Change the channel’s mode:
      * i: Set/remove Invite-only channel
      * t: Set/remove the restrictions of the TOPIC command to channel operators
      * k: Set/remove the channel key (password)
      * o: Give/take channel operator privilege
      * l: Set/remove the user limit to channel


## Usage

To start server:
```sh
make
./irc <port> <pass>
```
For client:
```sh
irssi
/connect -nocap -notls localhost <port>
/quote PASS <server pass>
/join <channel name>
```

![irssi example](https://i.imgur.com/h7YfGgX.png)

## Useful links
1. https://modern.ircdocs.horse/
2. https://irssi.org/
3. http://chi.cs.uchicago.edu/chirc/irc_examples.html
4. https://ircgod.com/