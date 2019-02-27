client:
	g++ -g -o pdiaspora main.c jabber.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c database_common.c memory.c web_access.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lX11 -lfreetype -lpng -ljpeg -lXinerama -pthread -lcurl

client_nighsoft:
	g++ -O10 -o pdiaspora main.c jabber.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c database_common.c memory.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lX11 -lfreetype -lpng -ljpeg -lXinerama -pthread -gloox

lab:
	gcc -O10 -o pdiaspora main.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c memory.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lfreetype -lpng -lm -lz -ldl -pthread -I/users/bt2003/reetu/mike/lib_installs/include/ -L/users/bt2003/reetu/mike/lib_installs/lib/ -L/usr/lib64/ -static

static:
	gcc -O10 -o pdiaspora main.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c database_common.c memory.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lfreetype -lpng -ltiff -ljpeg -lm -lz -ldl -lX11 -lXinerama -lXext -pthread -static
zip:
	zip pdiaspora_client_src *.c *.h makefile
	
zip_partial:
	zip -r pdiaspora_client_partial graphics fonts

zip_pdiaspora:
	zip -r pdiaspora_graphics_pDiaspora server

zip_linux:
	zip pdiaspora_client_linux pdiaspora

release_zip:
	tar -czf pdiaspora_client-beta-lin.tar.gz pdiaspora_client-beta
