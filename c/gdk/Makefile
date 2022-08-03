
CFLAGS := -pthread -Wl,--copy-dt-needed-entries

# pkg-config --cflags gdk-3.0
CPPFLAGS := -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
CPPFLAGS += -I/usr/include/gtk-3.0
CPPFLAGS += -I/usr/include/pango-1.0
CPPFLAGS += -I/usr/include/harfbuzz
CPPFLAGS += -I/usr/include/gdk-pixbuf-2.0
CPPFLAGS += -I/usr/include/cairo
CPPFLAGS += -I/usr/include/freetype2
CPPFLAGS += -I/usr/include/libmount
CPPFLAGS += -I/usr/include/blkid
CPPFLAGS += -I/usr/include/sysprof-6
CPPFLAGS += -I/usr/include/libpng16
CPPFLAGS += -I/usr/include/libpng16
CPPFLAGS += -I/usr/include/gio-unix-2.0
CPPFLAGS += -I/usr/include/cloudproviders
CPPFLAGS += -I/usr/include/fribidi
CPPFLAGS += -I/usr/include/atk-1.0

# pkg-config --libs gdk-3.0
LIBS :=  -lgdk-3 -lz -lharfbuzz -lpangocairo-1.0 -lpango-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lglib-2.0 -lgobject-2.0
LIBS += -lgtk-3

all:
	gcc $(CFLAGS) $(CPPFLAGS) $(LIBS) -o helloworld helloworld.c

