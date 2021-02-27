runme-cairo: main-cairo.c xdg-shell.h xdg-shell.c
	gcc main-cairo.c xdg-shell.c -l wayland-client -lcairo -lm -lxkbcommon -o runme-cairo

runme: main.c xdg-shell.h xdg-shell.c
	gcc main.c xdg-shell.c -l wayland-client -o runme

xdg-shell-unstable-v6.xml:
	curl -O https://cgit.freedesktop.org/wayland/wayland-protocols/plain/unstable/xdg-shell/xdg-shell-unstable-v6.xml

xdg-shell.h: xdg-shell-unstable-v6.xml
	wayland-scanner client-header xdg-shell-unstable-v6.xml xdg-shell.h

xdg-shell.c: xdg-shell-unstable-v6.xml
	wayland-scanner public-code xdg-shell-unstable-v6.xml xdg-shell.c

.PHONY: clean
clean:
	rm runme-cairo runme xdg-shell.c xdg-shell.h xdg-shell-unstable-v6.xml
