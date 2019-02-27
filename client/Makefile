client:
	g++ -Wno-write-strings -g -o pdiaspora main.c jabber.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c database_common.c memory.c web_access.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lX11 -lfreetype -lpng -ljpeg -lXinerama -pthread -lcurl

static:
	gcc -Wno-write-strings -O10 -o pdiaspora main.c sdl.c form.c common.c sock.c protocol.c hub.c server.c game.c database.c database_common.c memory.c -lSDL_gfx -lSDL_image -lSDL_ttf -lSDL -lfreetype -lpng -ltiff -ljpeg -lm -lz -ldl -lX11 -lXinerama -lXext -pthread -static
