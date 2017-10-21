CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99 -g
LDFLAGS=-lpthread
SDLCFLAGD=`sdl-config --cflags`
SDLLIBS=`sdl-config --libs`
BINARY=snake schlanga unittesting server client
SOURCE=client.c schlangalib.c snake.c unittesting.c schlanga.c server.c snakelib.c
OBJDIR=./OBJECT
DOCTOOL=doxygen
DOCCONF=snake.conf
DOCDIR=DOC
DOCHTML='<html><head><meta http-equiv="refresh" content="0; URL=DOC/html/index.html"></head><body></body></html>'
all: $(BINARY)
snake.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@ $(LDFLAGS) $(SDLCFLAGD)
schlanga.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@ $(LDFLAGS) $(SDLCFLAGD)
snakelib.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@
schlangalib.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@
unittesting.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@
server.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@ $(LDFLAGS)
client.o:
	$(CC) $(CFLAGS) -c SOURCE/$(@:.o=.c) -o $(OBJDIR)/$@ $(LDFLAGS) $(SDLCFLAGD)
snake: snake.o snakelib.o
	$(CC) $(CFLAGS) $(addprefix OBJECT/,$^) -o $@ $(LDFLAGS) $(SDLLIBS)
schlanga: schlanga.o snakelib.o schlangalib.o
	$(CC) $(CFLAGS) $(addprefix OBJECT/,$^) -o $@ $(LDFLAGS) $(SDLLIBS)
unittesting: unittesting.o snakelib.o
	$(CC) $(CFLAGS) $(addprefix OBJECT/,$^) -o $@ $(LDFLAGS) $(SDLLIBS)
server: server.o
	$(CC) $(CFLAGS) $(addprefix OBJECT/,$^) -o $@ $(LDFLAGS)
client: client.o snakelib.o
	$(CC) $(CFLAGS) $(addprefix OBJECT/,$^) -o $@ $(LDFLAGS) $(SDLLIBS)
doc:
	$(DOCTOOL) $(DOCCONF)
	echo $(DOCHTML) > doc.html
	cd $(DOCDIR)/latex && make
	cd $(DOCDIR)/latex && cp refman.pdf ../../doc.pdf
clean-doc:
	rm -rf DOC
	rm doc.*
log: snake schlanga unittesting
	valgrind --log-file="VALGRIND/snake.log" --leak-check=full --show-leak-kinds=all ./snake
	valgrind --log-file="VALGRIND/schlanga.log" --leak-check=full --show-leak-kinds=all ./schlanga
	valgrind --log-file="VALGRIND/unittesting.log" --leak-check=full --show-leak-kinds=all ./unittesting
clean-log:
	rm -rf VALGRIND/*.log
.PHONY: clean mrproper
clean:
	rm -rf OBJECT/*.o
mrproper: clean
	rm -rf $(BINARY)
