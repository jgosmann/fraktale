CPP = /usr/bin/g++
fraktale_CPPFLAGS = -Wall -ansi -DTIXML_USE_STL
fraktale_LDFLAGS = -lm -lalfont -lalmp3 `allegro-config --libs` -ljpgal
fractalviewer_CPPFLAGS = -Wall -ansi -DTIXML_USE_STL
fractalviewer_LDFLAGS = -L/usr/lib/qt3/lib/ -L/usr/X11R6/lib/ -lqt-mt -lXext -lX11 -lm -lalfont `allegro-config --libs` -ljpgal
icon_CPPFLAGS = -Wall -ansi

all: bin/fraktale bin/fractalviewer lexikon

bin/fraktale: tinyxml.o fraktale.o icon.o data
	$(CPP) $(fraktale_CPPFLAGS) -o bin/fraktale icon.o tinyxml.o tinystr.o tinyxmlerror.o tinyxmlparser.o fraktale.o $(fraktale_LDFLAGS)
	cp -r data/fraktale/content bin/
	cp -r data/fraktale/credits bin/
	cp data/fraktale/load2.tga bin/
	cp data/gui/fraktale/* bin/

bin/fractalviewer: tinyxml.o fractalviewer.o icon.o data
	$(CPP) $(fractalviewer_CPPFLAGS) -o bin/fractalviewer icon.o tinyxml.o tinystr.o tinyxmlerror.o tinyxmlparser.o fractalviewer.o $(fractalviewer_LDFLAGS)
	cp data/fractalviewer/load*.tga bin/
	cp -r data/fractalviewer/locale bin/
	cp -r data/fractalviewer/tutorial bin/

lexikon:
	cp -r data/lexikon bin/

tinyxml.o:
	make -C src/tinyxml
	mv src/tinyxml/*.o ./

icon.o:
	$(CPP) $(icon_CPPFLAGS) -c -o icon.o src/linux_icon.c

data:
	cp data/fonts/* bin/
	cp data/gui/*.* bin/

fraktale.o:
	make -C src/fraktale
	mv src/fraktale/*.o ./

fractalviewer.o:
	make -C src/fractalviewer
	mv src/fractalviewer/*.o ./

clean:
	-rm *.o

clean-all: clean
	-rm -r bin/*

.PHONY: all data clean clean-all
