This is a demo for *TTY Keyboard Status Request* — a UNIX OS feature
implemented in many \*BSDs and currently being proposed for Linux.

## Description

Citing __termios(3)__ from [man-pages](https://www.kernel.org/doc/man-pages/index.html):
```
   VSTATUS
      (not  in  POSIX;  not  supported under Linux; status re‐
      quest: 024, DC4, Ctrl-T).   Status  character  (STATUS).
      Display  status information at terminal, including state
      of foreground process and amount of CPU time it has con‐
      sumed.   Also  sends  a SIGINFO signal (not supported on
      Linux) to the foreground process group.
```
In existing implementations, it only works when isig, icanon and
iexten are set.
The main selling point of this is that various programs actually react
to SIGINFO in a meaningful way: for example, `cp(1)` and `dd(1)` print data
transmission progress, `sleep(1)` prints how long it intends to continue
for, and `tcpdump(1)` prints the same stats as it does when interrupted.
The kerninfo line itself is also useful e. g. to test if SSH remote peer
is alive and responds on the connection.

## How to use

```
    % git submodule init
    % git submodule update	# clones a patched busybox from github
    % make
```

These will build a minimal initrd with a patched busybox (the patches
only make stty learn about ^T and kerninfo lflag), and a couple of
simple executables:
- _/bin/siginfo_ — non-trivially handles SIGINFO.
  - Can accept command line arguments; e. g. `--read` makes it easier to
    check if keyboard input is not discarded.
- _/bin/uash_ — starts a /bin/ash instance in a separate PID namespace.

To run the kernel in this environment, you can say:

```
    qemu-system-x86_64 -smp cores=2 -enable-kvm -cpu host \
    -display none \
    -serial telnet:localhost:4440,server,nowait \
    -serial telnet:localhost:4444,server \
    -kernel path/to/your/arch/x86/boot/bzImage \
    -initrd path/to/this/tty-status.initrd.img \
    -append "console=ttyS1"
```
Adjust for guest architecture and desired kernel cmdline and QEMU parameters.

Connect to the running instance with e.g. `telnet ::1 4444`.
