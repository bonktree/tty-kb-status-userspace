LISTS := busybox.list tty-status.list devices.list

all: tty-status.initrd.img

busybox/busybox:
	make -C busybox defconfig
	sed -i '/CONFIG_STATIC/c CONFIG_STATIC=y' busybox/.config
	make -C busybox

%.initrd.img: %.list usr/gen_init_cpio
	usr/gen_init_cpio $< > $@

usr/gen_init_cpio: gen_init_cpio.c
	mkdir -p usr/
	cc $< -o $@

$(LISTS): %.list: %.list.in
	cpp -I. $< -o $@

tty-status.list: busybox.list siginfo-test/siginfo uash-launcher

busybox-symlinks.list.in: busybox/busybox busybox-symlinks.base
	echo '#include <busybox-symlinks.base>' >$@
	busybox/busybox --list | sed 's)^)slink /bin/); s($$( busybox 777 0 0(' >>$@

busybox.list.in: busybox/busybox base.list devices.list
	echo '#include <base.list>' >$@
	echo '#include <devices.list>' >>$@
	echo 'file /init init 0755 0 0' >>$@
	echo "file /bin/busybox busybox/busybox 0755 0 0 $$(busybox/busybox --list | sed 's)^)/bin/)' | tr '\n' ' ')" >>$@

siginfo-test/siginfo: $(wildcard siginfo-test/*.c)
	$(MAKE) -C siginfo-test siginfo

clean:
	rm -rf $(LISTS)		# generated
	rm -f busybox.list.in	# generated
	rm -rf usr
	$(MAKE) -C siginfo-test clean

.PHONY: clean
