# puppybbs
This is a minimalist telnet BBS.

It is pre-alpha, does not function.

I am old enough to remember BBS via dialup modem.  That world still
exists out there, but, I don't have a modem any more. But, there is
such a thing as a BBS over Telnet.  I thought I would write one.

When complete, this should allow one to post to a message board, and to
download files via XMODEM or maybe ZMODEM.


## Puppy's internal databases

When thinking about how to set up the databases, I am using as
inspiration a much older BBS called Puppy BBS.

### The global state

Most of the global state lives for Puppy BBS lived in `struct _pup`.
This is initialized by an INI file, and the INI file is overwritten
when values like `top` or `tries` change.  An INI file that gets
modified by the program itself is unfortunate.

| Variable    | Format        | Init       | Description                                      |
|-------------|---------------|------------|--------------------------------------------------|
| `callers`   | `int32_t`     | persistent | running total of number of callers to the system |
| `quote_pos` | `int32_t`     | persistent | current position in quotes file                  |
| `id`        | `_node`       | unused     | a Fidonet node id                                |
| `nlimit`    | `int16_t`     | INI set    | max connection length in minutes                 |
| `klimit`    | `int16_t`     | unused     | max kB download per connection                   |
| `top`       | `int16_t`     | persistent | ID of top of circular message buffer             |
| `msgnbr`    | `int16_t`     | ?          | current highest message number                   |
| `messages`  | `int16_t`     | INI set    | total number of messages allowed                 |
| `msgsize`   | `int16_t`     | INI set    | max chars per message                            |
| `topic`     | `_topic[16] ` | INI set    | names a descriptions of topics                   |
| `maxbaud`   | `int16_t`     | INI set    | max baud rate of a connection                    |
| `mdmstr`    | `char[80]`    | INI set    | initialization string for the modem              |
| `cd_bit`    | `uint16_t`    | INI set    | modem Carrier Detect mask                        |
| `iodev`     | `int16_t`     | INI set    | the COM port number                              |
| `tries`     | `int16_t`     | unused     | FidoNet dial attempts w/o connects               |
| `connects`  | `int16_t`     | unused     | FidoNet dial attempts w/ connects                |
| `sched`     | `_sched[35]`  | unused     | Event table for periodic events                  |
| `filepref`  | `char[80]`    | INI set    | path of upload/download files                    |

This is a mix of constant and persistent variable data.  For, Pupper,
this might be modified as follows.

The INI constants

| Variable   | Format       | Init    | Description                      |
|------------|--------------|---------|----------------------------------|
| `nlimit`   | `int16_t`    | INI set | max connection length in minutes |
| `klimit`   | `int16_t`    | INI set | max kB download per connection   |
| `messages` | `int16_t`    | INI set | total number of messages allowed |
| `msgsize`  | `int16_t`    | INI set | max chars per message            |
| `topic`    | `_topic[16]` | INI set | names and descriptions of topics |
| `filepref` | `char[80]`   | INI set | path of upload/download files    |
| `sqlname`  | `char[]`     | INI set | username for the MySQL database  |
| `sqlpass`  | `char[]`     | INI set | password for the MySQL database  |

The persistent data

| Variable    | Format | Init       | Description                                      |
|-------------|--------|------------|--------------------------------------------------|
| `callers`   | `INT`  | persistent | running total of number of callers to the system |
| `quote_pos` | `INT`  | persistent | current position in quotes file                  |

### Quotes

The quotes database for Puppy BBS was the QUOTES.PUP file.  The quotes
are stored as variable length plain text, and are separated from one
another by a blank line.

For Pupper, I could just store them in the database as a simple map.

| Variable | Format | Init | Description |
|----------|--------|------|-------------|
| `id`     | `INT`  |      |             |
| `quote`  | `TEXT` |      |             |
|          |        |      |             |

### Topics

The 16 topics are stored as

| Variable | Format     | Init    | Description            |
|----------|------------|---------|------------------------|
| `name`   | `char[8]`  | INI set | short topic name       |
| `desc`   | `char[24]` | INI set | long topic description |


### The caller database

In the Puppy BBS the caller was stored as

| Variable | Format         | Init   | Description                                            |
|----------|----------------|--------|--------------------------------------------------------|
| `name`   | `char[36]`     |        | user name                                              |
| `date`   | `uint16_t[16]` |        | MS-DOS dates of most recent message read in each topic |
| `topic`  | `uint16_t`     |        | number 1 to 16 of last topic selected                  |
| `lines`  | `uint8_t`      |        | preferred number of screen lines                       |
| `cols`   | `uint8_t`      |        | preferred number of screen cols                        |
| `calls`  | `uint16_t`     |        | number of calls by this user                           |
| `extra`  | `uint16_t`     | unused |                                                        |

The number of callers is limited to 25 in PUP.SET or 100 in defaults

In Pupper, we'll query rows/cols from the Terminfo database.

In Pupper's SQL, this could be

| Variable   | Format         | Init             | Description                                 |
|------------|----------------|------------------|---------------------------------------------|
| `CallerID` | `INT`          | `AUTO_INCREMENT` | Primary key                                 |
| `name`     | `CHAR[36]`     |                  | user name                                   |
| `date`     | `DATETIME[16]` |                  | per-topic dates of most recent message read |
| `topic`    | `TINYINT`      | 1 to 16          | last topic selected                         |
| `calls`    | `INT     `     |                  | number of calls                             |
|            |                |                  |                                             |

Where `CallerID` is the primary key.

### The messages database

The in Puppy BBS, the message header was

| Variable    | Format       | Init   | Description             |
|-------------|--------------|--------|-------------------------|
| `from`      | `char[36]`   |        | Name of author          |
| `to`        | `char[36]`   |        | Name of recipient       |
| `subj`      | `char[36]`   |        | Topic of message        |
| `date`      | `uint16_t`   |        | MS-DOS 16-bit date      |
| `time`      | `uint16_t`   |        | MS-DOS 16-bit time      |
| `extra`     | `uint16_t`   | unused |                         |
| `attr`      | `uint16_t`   | unused | bitmask                 |
| `topic`     | `uint16_t`   |        | topics                  |
| `topic_map` | `uint16_t`   | unused | ?                       |
| `message`   | `char[2048]` |        | The text of the message |
|             |              |        |                         |

The message size of 2048 was probably chosen as around 80 * 25.

The maximum number of messages is limited to 10 by 2560 in PUP.SET, or 50 by
2048 in defaults, with old ones expiring, so the message database is limited
to 25k in PUP.SET or  100kB max in default. The 25k was probably for MS-DOS
memory handling or floppy disk storage.

For Pupper, in SQL this could be rendered as

| Variable    | Format         | Init             | Description             |
|-------------|----------------|------------------|-------------------------|
| `msgid`     | `INT UNSIGNED` | `AUTO_INCREMENT` | the primary key         |
| `from`      | `CHAR[36]`     |                  | Name of author          |
| `to`        | `CHAR[36]`     |                  | Name of recipient       |
| `subj`      | `CHAR[36]`     |                  | Topic of message        |
| `timestamp` | `DATETIME`     |                  | MySQL Date/Time         |
| `topic`     | `TINYINT`      |                  | topics                  |
| `message`   | `TEXT`         |                  | The text of the message |
|             |                |                  |                         |


### The files database

In Puppy BBS, all downloadable files were stored on the local file
system in a single folder.  There was a file "FILES.PUP" that held a
database of files that could be downloaded.

The "FILES.PUP" file format was
- filename: char[13]
- description: char[40]

If FILENAME began with a space or hyphen then description is
just a comment.

If FILENAME began with ^Z or @, that was the end of the database.

Other part of the files 'database' comes from the filesystem
itself, including creation time and file size.


## On Making a Telnet BBS

So original PuppyBBS was probably a MS-DOS program.  From the original
code, it looks like it started up as a foreground application where
the sysop could interact with it, and it also listened to a single
modem port.

There are a couple of routes to making this into something I could
use.

- original MS-DOS application with one serial-port-attached modem
- inetd-style application on my Fedora GNU/Linux server
- multi-threaded server on my Fedora GNU/Linux server
- service on my Microsoft Windows 10 box

### original MS-DOS application with one serial-port-attached modem

The problem here is testing out a modem, and also that no one will
ever call me.  Kind of pointless, but, awesome.

The original Puppy BBS ha the sysop-and-user style, with sysop at the
console and a user on the modem.

### inetd-style application on my Fedora Server GNU/Linux box

This is obviously the simplest.  With each new client connection, a
new instance of the application is spawned.  The TCP I/O is converted
to stdin/stdout I/O by the inetd (or systemd) server.

The challenge here is that there needs to be file locking of the
database files, since multiple instantiations of the PupperBBS
executable may be accessing the files at the same time.  Basically, I
should use a proper database, instead of ad hoc files, unless I want
to write proper file locking, which is a pain on GNU systems.

Info on making systemd to act like an inetd spawner.
<http://0pointer.de/blog/projects/inetd.html>

### multi-threaded server on my Fedora Server GNU/Linux box

Now we get a bit more complicated.  Multiple threads and multiple
connections in a single application.

For info on making a daemon
<http://0pointer.de/public/systemd-man/daemon.html>

For info on making a server application, I can look back at the
lessons learned from GNU serveez.

### service on Windows 10

This is new to me.  There is info about writing services in C++ at
[MSDN Windows 10 services](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686953(v=vs.85).aspx)

It seems a bit like coding up an old school DLL back in the day.

This is also pointless, since my Windows 10 box is not exposed to the
world, but it was fun to read about.

## Required algorithms

### Getting user's terminal size

In original Puppy, the user is queried for the number of rows and
columns his terminal supports.  In Pupper we're going to ask for the
terminal instead.  The default terminal size is in the terminfo
database.

Note that for ncurses, we need `use_env(FALSE)` and
`use_tioctl(FALSE)` to be called before any ncurses stuff, so that
ncurses uses the number of rows and columns in the terminfo database,
instead of trying to query it from environment variables or ioctl
calls.  Those aren't relevant here because we're running on a client's
terminal over telnet.

## Replaceable Code

- ascii.h has #ifdefs like 8 = BS, 9 = TAB
- edit.c is an interactive console editor.  Ncurses Forms also
  has an editor.
- quote.c has a date/time parser
- there is initialization data, which could become an ini file, registry,
  or database.  Boost::property_tree?
- there is a message file-based database, which could become a standard
  database.  MariaDB
- the non-portable serial port code could become ASIO to make it both
  portable serial port and TCP
- there is a lot of parsing and string handling.  Boost::string_algo
- there is a lot of filename handling.
- xmodem I/O.  <https://github.com/caseykelso/xmodem>
- there is a task scheduler. This could also be Boost::ASIO
- what is the best c++ method to parse user input?  getline and regex?
  Is there something easier than regex?
- download() searches files by regex.

### Libraries
-   Standard C++ filesystem library:
    [documentation](http://en.cppreference.com/w/cpp/filesystem)
-   Standard C++ locale library sets up text encoding:
    [documentation](http://en.cppreference.com/w/cpp/locale)
-   Standard C++ messages library stores lists of translations:
    [documentation](http://en.cppreference.com/w/cpp/locale/messages)
-   The MySQL Connector/C++ (GPL):
    [documentation](https://dev.mysql.com/doc/connector-cpp/en/) and
    [code](https://dev.mysql.com/downloads/connector/cpp/)
-   Ncurses (BSD):
    [documentation](http://invisible-island.net/ncurses/ncurses.html) and
    [code](ftp://ftp.invisible-island.net/ncurses/ncurses.tar.gz)
    including the underdocumented C++ bindings
-   Boost [documentation](http://www.boost.org/doc/libs/1_66_0/) and
    [code](https://dl.bintray.com/boostorg/release/1.66.0/source/)
-   lrzsz (GPL2)
    [documentation and code](https://ohse.de/uwe/software/lrzsz.html), but
    see jnavila's patched version on
    [github](https://github.com/jnavila/lrzsz)
-   caseykelso's xmodem on [github](https://github.com/caseykelso/xmodem)

