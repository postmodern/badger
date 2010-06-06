Badger! Badger! Badger!
=======================


                            ___,,___
                       _,-='=- =-  -`"--.__,,.._
                    ,-;// /  - -       -   -= - "=.
                  ,'///    -     -   -   =  - ==-=\`.
                 |/// /  =    `. - =   == - =.=_,,._ `=/|
                ///    -   -    \  - - = ,ndDMHHMM/\b  \\
              ,' - / /        / /\ =  - /MM(,,._`YQMML  `|
             <_,=^Kkm / / / / ///H|wnWWdMKKK#""-;. `"0\  |
                    `""QkmmmmmnWMMM\""WHMKKMM\   `--. \> \
             hjm          `""'  `->>>    ``WHMb,.    `-_<@)
                                            `"QMM`.
                                               `>>>


Badger is a fear-less, robust and portable systems management solution.

Features
--------

* Uses ZeroMQ for networking and message delivery.
* Uses MessagePack for the packet format.
* Implements a Remote Procedure Call (RPC) design-pattern.
* Supports multiple optional Services:
  * sys - misc system related functions.
  * fs - remote file-system related functions.
  * ffi - dynamically load libraries on the remote system and call functions
    from them.
* Compiles a portable statically linked executable.
* Compiles multiple library archives for developers to roll their own badger
  servers.

Requirements
------------

* CMake (http://cmake.org/)

* core:
  * libzmq >= 2.0.7 (http://www.zeromq.org/)
  * libmsgpack >= 0.5.0 (http://msgpack.sourceforge.net)

* 'ffi' service:
  * libffi (http://sourceware.org/libffi/)

Build
-----

Interactively configure badger:

    $ ccmake .

Build:

    $ make

Install:

    $ make install

Built Files
-----------

* Library archives:
  * libbadger_core - The core server, services and function code.
  * libbadger_sys - The 'sys' service.
  * libbadger_fs - The 'fs' service.
  * libbadger_ffi - The 'ffi' service.
* Executables:
  * badger - The statically linked badger server.

Usage
-----

Connect to a client which is accepting connections:

    $ ./badger -c tcp://localhost:5555

Listen for clients connecting in:

    $ ./badger -l tcp://eth0:5555

Links
-----

* Source: http://github.com/postmodern/badger
* Bugs: http://github.com/postmodern/badger/issues
* IRC: irc://irc.freenode.net/badgerbadgerbadger

License
-------

Badger is licensed under the LGPL-3 license. See `COPYING.txt` for more
details.

