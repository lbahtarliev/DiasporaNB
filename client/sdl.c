/*
Author: Mike Bok
Email: freaknigh@hotmail.com
Website: www.nighsoft.net
License and Rights: Full licensing and Rights available at www.nighsoft.net.
If licensing and rights not available at www.nighsoft.net, then all the following apply as bulleted:
-all rights reserved.
-you can not modify source code in this file.
-you can compile and use this source code.
*/

#include "main.h"

#define REPEAT_CHAR_MAX 255

//int screen_flipping;
int display_box_adding;
double repeat_button_time[REPEAT_CHAR_MAX];
int button_pressed[REPEAT_CHAR_MAX];
SDL_Thread *repeat_button_thread[REPEAT_CHAR_MAX];
int is_windowed;

void add_display_box_mutex();
int repeat_button(void *nothing);
void hit_hutton(int the_char);
void clear_all_buttons_pressed();

int init_sdl()
{
	int i;
	
	if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) <0)
	{
		printf("error: could not load sdl\n");
		return 0;
	}
	
	if( TTF_Init() <0)
	{
		printf("error: could not load sdl_ttf\n");
		return 0;
	}
	
	is_windowed = 1;
	//screen = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
	screen = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF);
	//screen = SDL_SetVideoMode(800,600,0,SDL_ANYFORMAT);
	if (!screen)
	{
		printf("error: could not set screen for 800x600 32bit\n");
		return 0;
	}
	
	ttf_font = TTF_OpenFont("fonts/arial.ttf",12);
	if (!ttf_font)
	{
		printf("error: could not load fonts/arial.ttf\n");
		return 0;
	}
	
	SDL_WM_SetCaption("Project Diaspora", "Project Diaspora");
	SDL_WM_SetIcon(IMG_Load("graphics/game_icon.gif"), NULL);
	SDL_EnableUNICODE(SDL_ENABLE);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	//mutexs
	//space_mutex = SDL_CreateMutex();
	flip_mutex = SDL_CreateMutex();
	
	
	atexit(SDL_Quit);
	
	//screen_flipping = 0;
	display_box_adding = 0;

	for(i=0;i<REPEAT_CHAR_MAX;i++)
	{
		repeat_button_thread[i] = 0;
		repeat_button_time[i] = 0;
		button_pressed[i] = 0;
	}
	
	return 1;
}

void toggle_windowed()
{
    SDL_Surface *copy;
    
    copy = SDL_DisplayFormatAlpha(screen);//SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, original->w, original->h, 32, 0xFF000000, 0x0000FF00, 0x00FF0000, 0x000000FF);
	//copy = ZSDL_ConvertImage(copy);
	
	SDL_BlitSurface(screen, NULL, copy, NULL);
	
	if(is_windowed)
	{
        is_windowed = 0;
        screen = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
    }
    else
    {
        is_windowed = 1;
        screen = SDL_SetVideoMode(800,600,32,SDL_HWSURFACE|SDL_DOUBLEBUF);   
    }
    
	SDL_BlitSurface(copy, NULL, screen, NULL);
	
	SDL_FreeSurface(copy);
}

int handle_sdl_events()
{
	SDL_Event event;
	int exitkey = 0;
	int i;
	int ctrl_down = 0;
	
	while(SDL_PollEvent(&event))
		switch( event.type ) 
	{
		case SDL_QUIT:
			if(map.visible || space.visible) break;
			//exitkey = 1;
			return 1;
			break;
		case SDL_KEYDOWN:
				//printf("Key hit sym:%c %d:caps %d\n", event.key.keysym.sym, event.key.keysym.sym, (((event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT)) - !(!(event.key.keysym.mod & KMOD_CAPS))));
				//printf("Key hit uni:%c %d\n", event.key.keysym.unicode, event.key.keysym.unicode);
				//handle_key(convert_key_sym(event.key.keysym.sym,(((event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT)) - !(!(event.key.keysym.mod & KMOD_CAPS)))));
			
			//escape key can close the game
			if(event.key.keysym.unicode == SDLK_ESCAPE)
			{
                if(map.visible || space.visible) break;
    			return 1;
            }
			
			clear_all_buttons_pressed();
				
			if(event.key.keysym.unicode && event.key.keysym.unicode <= 26)
				handle_ctrl_key((event.key.keysym.unicode - 1) + 'a');
				
			if(event.key.keysym.sym != 273 && event.key.keysym.sym != 274 && event.key.keysym.sym != 275 && event.key.keysym.sym !=  276 && event.key.keysym.sym != 127 && event.key.keysym.sym != 280 && event.key.keysym.sym != 281)
			{
				button_pressed[event.key.keysym.unicode] = 1;
				hit_hutton(event.key.keysym.unicode);
					//printf("hit number:%d\n",event.key.keysym.unicode);
			}
			else
			{
				button_pressed[event.key.keysym.sym] = 1;
				hit_hutton(event.key.keysym.sym);
					//printf("hit number:%d\n",event.key.keysym.sym);
			}
				
			break;
		case SDL_KEYUP:

			clear_all_buttons_pressed();
				
			if(event.key.keysym.sym != 273 && event.key.keysym.sym != 274 && event.key.keysym.sym != 275 && event.key.keysym.sym !=  276 && event.key.keysym.sym != 127 && event.key.keysym.sym != 280 && event.key.keysym.sym != 281)
			{
				button_pressed[event.key.keysym.unicode] = 0;
					//printf("unhit number:%d\n",event.key.keysym.unicode);
			}
			else
			{
				button_pressed[event.key.keysym.sym] = 0;
					//printf("unhit number:%d\n",event.key.keysym.sym);
			}
				

				
			break;
		case SDL_MOUSEBUTTONDOWN:
				//printf("Click down at x:%d y:%d\n", event.button.x, event.button.y);
			if(event.button.button == SDL_BUTTON_LEFT)
				handle_button(event.button.x,event.button.y);
			else if(event.button.button == SDL_BUTTON_RIGHT)
				handle_button_right(event.button.x,event.button.y);
			break;
		case SDL_MOUSEBUTTONUP:
				//printf("Click up at x:%d y:%d\n", event.button.x, event.button.y);
			handle_button_up(event.button.x,event.button.y);
			break;
		case SDL_MOUSEMOTION :
				//printf("Motion at x:%d y:%d\n", event.button.x, event.button.y);
			handle_motion(event.motion.x,event.motion.y);
			break;
	}
	
	return 0;
}

void handle_events()
{
	SDL_Event event;
	int exitkey = 0;
	int i;
	int ctrl_down = 0;
	
	//clear this all out
	for(i=0;i<REPEAT_CHAR_MAX;i++)
	{
		repeat_button_thread[i] = 0;
		repeat_button_time[i] = 0;
		button_pressed[i] = 0;
	}

	//SDL_WaitEvent(&event)
	//SDL_PollEvent(&event)
	while(!exitkey)
	{
		//handle some connections
		handle_sockets();
		
		//handle sdl event
		if(SDL_PollEvent(&event))
		switch( event.type ) 
		{
			case SDL_QUIT:
				if(map.visible || space.visible) break;
				exitkey = 1;
				break;
			case SDL_KEYDOWN:
				//printf("Key hit sym:%c %d:caps %d\n", event.key.keysym.sym, event.key.keysym.sym, (((event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT)) - !(!(event.key.keysym.mod & KMOD_CAPS))));
				//printf("Key hit uni:%c %d\n", event.key.keysym.unicode, event.key.keysym.unicode);
				//handle_key(convert_key_sym(event.key.keysym.sym,(((event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT)) - !(!(event.key.keysym.mod & KMOD_CAPS)))));
				
				clear_all_buttons_pressed();
				
				if(event.key.keysym.unicode && event.key.keysym.unicode <= 26)
					handle_ctrl_key((event.key.keysym.unicode - 1) + 'a');
				
				if(event.key.keysym.sym != 273 && event.key.keysym.sym != 274 && event.key.keysym.sym != 275 && event.key.keysym.sym !=  276 && event.key.keysym.sym != 127 && event.key.keysym.sym != 280 && event.key.keysym.sym != 281)
				{
					button_pressed[event.key.keysym.unicode] = 1;
					hit_hutton(event.key.keysym.unicode);
					//printf("hit number:%d\n",event.key.keysym.unicode);
				}
				else
				{
					button_pressed[event.key.keysym.sym] = 1;
					hit_hutton(event.key.keysym.sym);
					//printf("hit number:%d\n",event.key.keysym.sym);
				}
				
				break;
			case SDL_KEYUP:

				clear_all_buttons_pressed();
				
				if(event.key.keysym.sym != 273 && event.key.keysym.sym != 274 && event.key.keysym.sym != 275 && event.key.keysym.sym !=  276 && event.key.keysym.sym != 127 && event.key.keysym.sym != 280 && event.key.keysym.sym != 281)
				{
					button_pressed[event.key.keysym.unicode] = 0;
					//printf("unhit number:%d\n",event.key.keysym.unicode);
				}
				else
				{
					button_pressed[event.key.keysym.sym] = 0;
					//printf("unhit number:%d\n",event.key.keysym.sym);
				}
				

				
				break;
			case SDL_MOUSEBUTTONDOWN:
				//printf("Click down at x:%d y:%d\n", event.button.x, event.button.y);
				if(event.button.button == SDL_BUTTON_LEFT)
				    handle_button(event.button.x,event.button.y);
				else if(event.button.button == SDL_BUTTON_RIGHT)
				    handle_button_right(event.button.x,event.button.y);
				break;
			case SDL_MOUSEBUTTONUP:
				//printf("Click up at x:%d y:%d\n", event.button.x, event.button.y);
				handle_button_up(event.button.x,event.button.y);
				break;
			case SDL_MOUSEMOTION :
				//printf("Motion at x:%d y:%d\n", event.button.x, event.button.y);
				handle_motion(event.motion.x,event.motion.y);
				break;
		}
		
#ifdef WIN32
		Sleep(1);
#else
		usleep(1000);
#endif
	}
	
}

void sdl_flip_mutex()
{
// 	while (screen_flipping)
// 	{
// #ifdef WIN32
// 	Sleep(1);
// #else
// 	usleep(1000);
// #endif
// 	}
	
	SDL_LockMutex ( flip_mutex );
	
	SDL_Flip(screen);
	
	SDL_UnlockMutex ( flip_mutex );
}

void add_display_box_mutex()
{
	while (display_box_adding)
	{
#ifdef WIN32
	Sleep(1);
#else
	usleep(1000);
#endif
	}
}

void clear_all_buttons_pressed()
{
	int i;
	
	for(i=0;i<REPEAT_CHAR_MAX;i++)
		button_pressed[i] = 0;
}

void hit_hutton(int the_char)
{
	static int the_input;
	
//	repeat_button_time[the_char] = current_time();
	handle_key(the_char);
	
//	if(!repeat_button_thread[the_char])
//	{
//		the_input = the_char;
//		repeat_button_thread[the_char] = SDL_CreateThread(repeat_button, (void*)&the_input);
//	}
}

int repeat_button(void *nothing)
{
	int i, j;
	const int delay_time = 80;
	const int delay_time_start = 250;
	const int delay_time_short = delay_time / 10;
	
	i = *((int*)nothing);
	
	for(j=0;j<delay_time_start && button_pressed[i];j+=delay_time_short)
		SDL_Delay(delay_time_short);

	while(button_pressed[i])
	{
		SDL_Delay(delay_time_short);

		if(!button_pressed[i])
			break;

		if(current_time() - repeat_button_time[i] < delay_time * 0.001)
			continue;
		
		if(!button_pressed[i])
			break;
		
		repeat_button_time[i] = current_time();
		handle_key(i);
	}

	repeat_button_thread[i] = 0;
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void put32pixel(SDL_Surface *surface, int x, int y, SDL_Color color)
{
	SDL_PixelFormat *fmt;
	//SDL_Surface *surface;
	SDL_Color return_color;
	Uint32 temp, *pixel;
	Uint8 red, green, blue, alpha;

	fmt = surface->format;
	//SDL_LockSurface(surface);
	//pixel = *((Uint32*)surface->pixels);
	//pixel = *((Uint32*)((Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel));
	//SDL_UnlockSurface(surface);
	
	pixel = (Uint32*)((Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
	
	//clear red
	((Uint8*)pixel)[3] = color.r;
	((Uint8*)pixel)[2] = color.g;
	((Uint8*)pixel)[1] = color.b;
	((Uint8*)pixel)[0] = color.unused;
}

SDL_Color get32pixel(SDL_Surface *surface, int x, int y)
{
	SDL_PixelFormat *fmt;
	//SDL_Surface *surface;
	SDL_Color return_color;
	Uint32 temp, pixel;
	Uint8 red, green, blue, alpha;

	fmt = surface->format;
	//SDL_LockSurface(surface);
	//pixel = *((Uint32*)surface->pixels);
	pixel = *((Uint32*)((Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel));
	//SDL_UnlockSurface(surface);
	 
	/* Get Red component */
	temp = pixel & fmt->Rmask;  /* Isolate red component */
	temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
	red = (Uint8)temp;
	 
	/* Get Green component */
	temp = pixel & fmt->Gmask;  /* Isolate green component */
	temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
	green = (Uint8)temp;
	
	/* Get Blue component */
	temp = pixel & fmt->Bmask;  /* Isolate blue component */
	temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
	temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
	blue = (Uint8)temp;
	 
	/* Get Alpha component */
	temp = pixel & fmt->Amask;  /* Isolate alpha component */
	temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
	temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
	alpha = (Uint8)temp;

	return_color.r = red;
	return_color.g = green;
	return_color.b = blue;
	return_color.unused = alpha;
	
	return return_color;
}

SDL_Surface *add_red_glow(SDL_Surface *the_img)
{
	SDL_Surface *new_img;
	SDL_Color color;
	int x, y, w, h;
	int xi, yi;
	const int max_runs = 10;
	const int glow_inc = -15;
	const int glow_start = 150;
	const int alpha_inc = -25;
	const int alpha_start = 255;
	int i, r, cur_glow, cur_alpha;
	char **temp_img;
	SDL_Rect rect;
	
	if(!the_img) return NULL;
	
	//set some stuff for ease
	w = the_img->w;
	h = the_img->h;
	
	//make temp_img
	temp_img = (char**)malloc(w * sizeof(char*));
	for(i=0;i<w;i++)
		temp_img[i] = (char*)malloc(h);
	
	//make the 32 bit image
	new_img = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,w,h,32,0xFF000000,0x0000FF00,0x00FF0000,0x000000FF);
	if(!new_img)
	{
		printf("unable to make a new sos glow image\n");
		return NULL;
	}
	
	//put the original on top of it
	rect.x = 0;
	rect.y = 0;
	rect.w = w;
	rect.h = h;
	SDL_BlitSurface(the_img, NULL, new_img, &rect);
	the_img = new_img;
	
	SDL_LockSurface(the_img);
	
	//do it
	for(r=0, cur_glow=glow_start, cur_alpha=alpha_start;r<max_runs;r++, cur_glow+=glow_inc, cur_alpha+=alpha_inc)
	{
		//find all the pixels we want
		for(x=0;x<w;x++)
			for(y=0;y<h;y++)
		{
			//clear this pixel out
			temp_img[x][y] = 0;
			
			color = get32pixel(the_img, x, y);
			
			//skip if this sector is a good color
			if(color.unused) continue;
			
			//check its neighbors
			for(xi=x-1;xi<=x+1;xi++)
			{
				//out of bounds?
				if(xi<0) continue;
				if(xi>=w) continue;
				
				for(yi=y-1;yi<=y+1;yi++)
				{
					//out of bounds?
					if(yi<0) continue;
					if(yi>=h) continue;
					
					//specials, no corners
					if(xi==x-1 && yi==y-1) continue;
					if(xi==x+1 && yi==y-1) continue;
					if(xi==x-1 && yi==y+1) continue;
					if(xi==x+1 && yi==y+1) continue;
					
					color = get32pixel(the_img, xi, yi);
			
					//skip if this neighbor is the alpha color
					if(!color.unused) continue;
					
					temp_img[x][y] = 1;
					
					//cause a break out
					xi=x+3;
					break;
				}
			}
		}
		
		//set those pixels
		color.r = cur_glow;
		color.g = 0;
		color.b = 0;
		color.unused = cur_alpha;
		
		for(x=0;x<w;x++)
		for(y=0;y<h;y++)
		if(temp_img[x][y])
			put32pixel(the_img, x, y, color);
	}
	
	/* Unlock the surface */
	SDL_UnlockSurface(the_img);
	
	//make temp_img
	for(i=0;i<w;i++)
		free(temp_img[i]);
	free(temp_img);

	return the_img;
}

SDL_Color get_pixel(SDL_Surface *the_img, int x, int y)
{
	Uint8 *ubuff8;
		
	ubuff8 = (Uint8*) the_img->pixels;
	ubuff8 += (y * the_img->pitch) + x;
		
	return the_img->format->palette->colors[*ubuff8];
}

void set_pixel(SDL_Surface *screan, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
	Uint8 *ubuff8;
	Uint16 *ubuff16;
	Uint32 *ubuff32;
	Uint32 color;
	char c1, c2, c3;
	
	/* Lock the screen, if needed */
	if(SDL_MUSTLOCK(screan)) 
	{
		if(SDL_LockSurface(screan) < 0) 
		return;
	}
	
	/* Get the color */
	color = SDL_MapRGB( screan->format, r, g, b );
	
	/* How we draw the pixel depends on the bitdepth */
	switch(screan->format->BytesPerPixel) 
	{
		case 1:
			ubuff8 = (Uint8*) screan->pixels;
			ubuff8 += (y * screan->pitch) + x; 
			*ubuff8 =  color;
			break;
		
		case 2:
			ubuff8 = (Uint8*) screan->pixels;
			ubuff8 += (y * screan->pitch) + (x*2);
			ubuff16 = (Uint16*) ubuff8;
			*ubuff16 = (Uint16) color; 
			break;
		
		case 3:
			ubuff8 = (Uint8*) screan->pixels;
			ubuff8 += (y * screan->pitch) + (x*3);
			
			
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
				c1 = (color & 0xFF0000) >> 16;
				c2 = (color & 0x00FF00) >> 8;
				c3 = (color & 0x0000FF);
			} else {
				c3 = (color & 0xFF0000) >> 16;
				c2 = (color & 0x00FF00) >> 8;
				c1 = (color & 0x0000FF);	
			}
			
			ubuff8[0] = c3;
			ubuff8[1] = c2;
			ubuff8[2] = c1;
			break;
		
		case 4:
			ubuff8 = (Uint8*) screan->pixels;
			ubuff8 += (y*screan->pitch) + (x*4);
			ubuff32 = (Uint32*)ubuff8;
			*ubuff32 = color;
			break;
		
		default:
			fprintf(stderr, "Error: Unknown bitdepth!\n");
	}
	
	/* Unlock the screen if needed */
	if(SDL_MUSTLOCK(screan)) 
	{
		SDL_UnlockSurface(screan);
	}
}

void draw_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b)
{
	SDL_Surface *text;
	SDL_Color textcolor = {0,0,0,0};

	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	SDL_BlitSurface( text, NULL, screan, &label->location);
	
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	
	if(label->message != message)
		strcpy(label->message, message);
	
	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	SDL_BlitSurface( text, NULL, screan, &label->location);
}

void draw_center_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b)
{
	SDL_Rect temp_location;
	SDL_Surface *text;
	SDL_Color textcolor = {0,0,0,0};
	
	temp_location.y = label->location.y;
	temp_location.w = label->location.w;
	temp_location.h = label->location.h;
	
	
	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	if (text) 
	{
		temp_location.x = label->location.x + (label->location.w / 2) - (text->w / 2); //center text
		SDL_BlitSurface( text, NULL, screan, &temp_location);
	}
	
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	
	if(label->message != message)
		strcpy(label->message, message);
	
	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	if (text) 
	{
		temp_location.x = label->location.x + (label->location.w / 2) - (text->w / 2); //center text
		SDL_BlitSurface( text, NULL, screan, &temp_location);
	}
}

void draw_right_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b)
{
	SDL_Rect temp_location;
	SDL_Surface *text;
	SDL_Color textcolor = {0,0,0,0};
	
	temp_location.y = label->location.y;
	temp_location.w = label->location.w;
	temp_location.h = label->location.h;
	
	
	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	if (text) 
	{
		temp_location.x = label->location.x + (label->location.w - text->w); //right align test
		SDL_BlitSurface( text, NULL, screan, &temp_location);
	}
	
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	
	if(label->message != message)
		strcpy(label->message, message);
	
	text = TTF_RenderText_Solid(ttf_font, label->message, textcolor);
	if (text) 
	{
		temp_location.x = label->location.x + (label->location.w - text->w); //right align test
		SDL_BlitSurface( text, NULL, screan, &temp_location);
	}
}

void draw_raw_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b)
{
	SDL_Surface *text;
	SDL_Color textcolor;
			
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	textcolor.unused = 0;
			
	text = TTF_RenderText_Solid(ttf_font, message, textcolor);
	
	SDL_BlitSurface( text, NULL, screan, &location);
}

void draw_raw_right_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b)
{
	SDL_Rect temp_location;
	SDL_Surface *text;
	SDL_Color textcolor = {0,0,0,0};
	
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	
	text = TTF_RenderText_Solid(ttf_font, message, textcolor);
	if (text) 
	{
		location.x = location.x + (location.w - text->w); //right align test
		SDL_BlitSurface( text, NULL, screan, &location);
	}
}

void draw_raw_center_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b)
{
	SDL_Surface *text;
	SDL_Color textcolor;
	
	if(message[0] == '\0') return;
			
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	textcolor.unused = 0;
			
	text = TTF_RenderText_Solid(ttf_font, message, textcolor);
	
	location.x = location.x + (location.w / 2) - (text->w / 2);
	
	SDL_BlitSurface( text, NULL, screan, &location);
}

void draw_text_box(SDL_Surface *screan, char *message, struct textbox *text_box, int r, int g, int b)
{
	int temp_width;
	int temp_frame;
	SDL_Surface *text;
	SDL_Color textcolor = {0,0,0,0};
	SDL_Rect temp_location, clr_box;
	
	//just is needed
	temp_width = text_box->location.w; //preserve the correct width
	
	//mess with this
	temp_location = text_box->location;
	temp_location.y += 4;
	temp_location.h = 12;
	
	//set this
	clr_box = text_box->location;
	
	//colors!
	textcolor.r = r;
	textcolor.g = g;
	textcolor.b = b;
	
	//text = TTF_RenderText_Solid(ttf_font, text_box->prev_message, textcolor);
	//SDL_BlitSurface( text, NULL, screan, &text_box->location);//<-changes "location" width to width of text
	
	//clear the shit
	SDL_FillRect(screan, &clr_box, SDL_MapRGB(screen->format, 0, 0, 0));
	
	//strcpy(text_box->prev_message, message);
	
	//set the text
	text = TTF_RenderText_Solid(ttf_font, message, textcolor);
	
	if(!text) return;
	
	//set the offset
	temp_location.x = text_box->last_shift;
	temp_location.y = 0;
	temp_location.h = text->h;
	
	if(text_box->cursor_loc - text_box->last_shift > temp_location.w)
		temp_location.x = text_box->cursor_loc - temp_location.w;
		
	if(text_box->cursor_loc - 3 < text_box->last_shift)
		temp_location.x = text_box->cursor_loc - 3;
	
	if(temp_location.x < 0) temp_location.x = 0;
	
	text_box->last_shift = temp_location.x;
	
	//draw
	SDL_BlitSurface( text, &temp_location, screan, &text_box->location);//<-changes "location" width to width of text
	
	//do this
	text_box->location.w = temp_width;
}

void clear_text_box(SDL_Surface *screan, struct textbox *text_box)
{	
	text_box->message[0] = '\0';
	
	text_box->cursor_loc = 0;
	text_box->last_shift = 0;
	
	if (screan) //if not NULL
	{
		if (text_box->selected)
		{
			draw_text_box(screan, "|", text_box, 255, 255, 255);
			//3;
        }
		else
		{
			draw_text_box(screan, "", text_box, 255, 255, 255);
        }
	}
	else
	{
		text_box->prev_message[0] = '\0';
		text_box->selected = 0;
	}
	
	text_box->cursor = 0;
	text_box->char_amt = 0;
}

void set_text_box_text(SDL_Surface *screan, struct textbox *text_box, char *message)
{	
	if(text_box->message != message)
		strcpy(text_box->message, message);

	text_box->char_amt = strlen(message);
	text_box->cursor = text_box->char_amt;
	text_box->last_shift = 0;
	
	if (screan) //if not NULL
	{
        if (text_box->selected)
            select_text_box(screan,text_box);
        else
            unselect_text_box(screan,text_box);
	}
	else
	{
		;//text_box->prev_message[0] = '\0';
	}
}

void add_text_box(SDL_Surface *screan, struct textbox *text_box, int c)
{
	char temp_message[405] = "";
	char temp_left[405];
	int i, w, h;
	
	switch (c)
	{
	case 8: //backspace
		if (text_box->cursor == 0) return;

		text_box->cursor = text_box->cursor - 1;
		text_box->char_amt = text_box->char_amt - 1;
		
		if (text_box->cursor != 0)
		{
			left(temp_left, text_box->message, text_box->cursor); 
			strcat(temp_message, temp_left);
		}
		if (text_box->cursor != text_box->char_amt) 
		{
			right(temp_left, text_box->message, text_box->cursor + 1); 
			strcat(temp_message, temp_left);
		}
		
		strcpy(text_box->message, temp_message);
		text_box->message[text_box->char_amt] = '\0';
		
		if (text_box->pass_char == '\0')
		{
			if (text_box->cursor != 0)
			{
				left(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			right(temp_left, text_box->message, text_box->cursor);
			strcat(temp_message, temp_left);
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	case 127: //delete
		if (text_box->cursor == text_box->char_amt) return;

		text_box->cursor = text_box->cursor;
		text_box->char_amt = text_box->char_amt - 1;
		
		if (text_box->cursor != 0)
		{
			left(temp_left, text_box->message, text_box->cursor); 
			strcat(temp_message, temp_left);
		}
		if (text_box->cursor != text_box->char_amt) 
		{
			right(temp_left, text_box->message, text_box->cursor + 1); 
			strcat(temp_message, temp_left);
		}
		
		strcpy(text_box->message, temp_message);
		text_box->message[text_box->char_amt] = '\0';
		
		if (text_box->pass_char == '\0')
		{
			if (text_box->cursor != 0)
			{
				left(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			if (text_box->cursor != text_box->char_amt) 
			{
				right(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	case 273: // up key
		use_next_text_box_entry(text_box);
		
		if (text_box->pass_char == '\0')
		{
			if (text_box->cursor != 0)
			{
				left(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			right(temp_left, text_box->message, text_box->cursor);
			strcat(temp_message, temp_left);
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	case 274: // up key
		use_previous_text_box_entry(text_box);
		
		if (text_box->pass_char == '\0')
		{
			if (text_box->cursor != 0)
			{
				left(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			right(temp_left, text_box->message, text_box->cursor);
			strcat(temp_message, temp_left);
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	case 276: //left key
		if (text_box->cursor == 0) return;
		
		text_box->cursor = text_box->cursor - 1;
		
		if (text_box->pass_char == '\0')
		{
			if (text_box->cursor != 0)
			{
				left(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			right(temp_left, text_box->message, text_box->cursor);
			strcat(temp_message, temp_left);
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	case 275: //right key
		if (text_box->cursor == text_box->char_amt) return;
		text_box->cursor = text_box->cursor + 1;
		
		if (text_box->pass_char == '\0')
		{
			left(temp_left, text_box->message, text_box->cursor); strcat(temp_message, temp_left);
		
			temp_message[text_box->cursor] = '|';
			temp_message[text_box->cursor + 1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
			if (text_box->cursor != text_box->char_amt) 
			{
				right(temp_left, text_box->message, text_box->cursor); 
				strcat(temp_message, temp_left);
			}
		}
		else
		{
			for(i=0;i<text_box->cursor;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '|';
			temp_message[i+1] = '\0';
			
			TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
			
			for(i++;i<text_box->char_amt + 1;i++)
				temp_message[i] = text_box->pass_char;
			
			temp_message[i] = '\0';
		}
		
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
		break;
	}

	if (c < 32 || c > 126) return;

	
	if (text_box->max_char == text_box->char_amt) return;

	////---------------------------------- add this char to the message
	if (text_box->cursor != 0)
	{
		left(temp_left, text_box->message, text_box->cursor); 
		strcat(temp_message, temp_left);
	}
	
	temp_message[text_box->cursor] = c;
	temp_message[text_box->cursor + 1] = '\0';
	
	
	
	right(temp_left, text_box->message, text_box->cursor);
	strcat(temp_message, temp_left);
	
	//TTF_SizeText(ttf_font, text_box->message, &w, &h); //check how long this will be
	//if (w + 6 > text_box->location.w) return; //exceeds boundaries so exit before making real changes
	
	
	
	text_box->char_amt = text_box->char_amt + 1;
	strcpy(text_box->message, temp_message);
	//////---------------------------------------
	text_box->cursor = text_box->cursor + 1;
	
	//now add in the cursor and check for pass char.	
	if (text_box->pass_char == '\0') //if not a pass box
	{
		if (text_box->cursor != 0)
		{
			left(temp_left, text_box->message, text_box->cursor); 
			strcat(temp_message, temp_left);
		}
	
		temp_message[text_box->cursor] = '|';
		temp_message[text_box->cursor + 1] = '\0';
		
        //set!
        TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
	
		if (text_box->cursor != text_box->char_amt) 
		{
			right(temp_left, text_box->message, text_box->cursor); 
			strcat(temp_message, temp_left);
		}
	}
	else
	{
		for(i=0;i<text_box->cursor;i++)
			temp_message[i] = text_box->pass_char;
		
		temp_message[i] = '|';
		temp_message[i+1] = '\0';
		
		//set!
        TTF_SizeText(ttf_font, temp_message, &(text_box->cursor_loc), &h);
		
		for(i++;i<text_box->char_amt + 1;i++)
			temp_message[i] = text_box->pass_char;
		
		temp_message[i] = '\0';
	}

	draw_text_box(screan, temp_message, text_box, 250, 250, 250);
}

void select_text_box(SDL_Surface *screan, struct textbox *text_box)
{
	char temp_message[405] = "";
	char temp_left[405];
	int i;
	
	text_box->selected = 1;
	
	if (text_box->pass_char == '\0')
	{
		if (text_box->cursor != 0)
		{
			left(temp_left, text_box->message, text_box->cursor); 
			strcat(temp_message, temp_left);
		}
		
		temp_message[text_box->cursor] = '|';
		temp_message[text_box->cursor + 1] = '\0';
	
		right(temp_left, text_box->message, text_box->cursor);
		strcat(temp_message, temp_left);
	}
	else
	{
		for(i=0;i<text_box->cursor;i++)
			temp_message[i] = text_box->pass_char;
		
		temp_message[i] = '|';
		
		for(i++;i<text_box->char_amt + 1;i++)
			temp_message[i] = text_box->pass_char;
		
		temp_message[i] = '\0';
	}
	
	draw_text_box(screan, temp_message, text_box, 250, 250, 250);
	
}

void unselect_text_box(SDL_Surface *screan, struct textbox *text_box)
{
	char temp_message[405];
	int i;
	
	text_box->selected = 0;
	
	if (text_box->pass_char == '\0')
		draw_text_box(screan, text_box->message, text_box, 250, 250, 250);
	else
	{
		for(i=0;i<text_box->char_amt;i++)
			temp_message[i] = text_box->pass_char;
	
		temp_message[i] = '\0';
		draw_text_box(screan, temp_message, text_box, 250, 250, 250);
	}
}

void clear_display_box(SDL_Surface *screan, struct displaybox *display_box)
{
	int i;
	
	if (screan)
	{
		for(i=0;i<display_box->lines;i++) //clear all visible entrys
			draw_label(screan, "", &display_box->label[i], 255, 255, 255);
		
		//clear all entries
		for(i=0;i<DISPLAYBOX_ENTRY_MAX;i++)
			display_box->entry[i][0] = 0;
		//display_box->entry[i + display_box->scroll]
		//display_box->entry_max = display_box->lines;
		
		//draw_display_box(screan, display_box); //make the clearing
	}
	
	display_box->entry_max = 0;
	display_box->scroll = 0;
}

void add_display_box(SDL_Surface *screan, struct displaybox *display_box, int text_color, char *message)
{
	int i, j, point = 0, done = 0, entry_ammount = 1, w, h;
	char *temp_ptr;
	char wrapped_entry[20][500] = {0}; //not quite sure what potential maxes are for wraping text...
	
	add_display_box_mutex();
	display_box_adding = 1;

	//check wrapping..
	TTF_SizeText(ttf_font, message, &w, &h); //check how long this will be
	
	if (w > display_box->w) //time for wrapping
	{
		for(i=0;i<50 && !done;i++) //standard setup for all new entries
			for(j=0;;j++)
			{
				if(j>=500)
				{
					point--;
					wrapped_entry[i][j-1] = 0;
					break;
				}
				
				if(point>=500)
				{
					wrapped_entry[i][j] = 0;
					done = 1;
					break;
				}
				
				wrapped_entry[i][j] = message[point];
				//wrapped_entry[i][j+1] = '\0';
				if (message[point] == '\0')
				{
					done = 1;
					break;
				}
				
				TTF_SizeText(ttf_font, wrapped_entry[i], &w, &h);
				if (w > display_box->w) //one letter too many it seems
				{
					//point--;
					wrapped_entry[i][j] = '\0';
					break;
				}
				point++;
			}
			
		entry_ammount = i;
	}
	
	for(i=entry_ammount - 1;i>-1;i--) //add all new entries
	{
		//-----------make slot 1 avaible.
		if (display_box->entry_max < 50) display_box->entry_max++; //set "new entry" if space avaible
		temp_ptr = display_box->entry[display_box->entry_max - 1]; //required for a switch that happens
		
		for(j=display_box->entry_max - 1;j>0;j--) //move all elements up one (deleting the last position)
		{
			display_box->entry[j] = display_box->entry[j-1];
			display_box->color[j] = display_box->color[j-1];
		}
		
		display_box->entry[0] = temp_ptr; //now to use the switch ptr
		display_box->color[0] = text_color; //now to use the switch ptr
		//-----------one slot now avaible...
		
		//add our message
		if(entry_ammount==1)
			strcpy(display_box->entry[0],message);
		else
			strcpy(display_box->entry[0],wrapped_entry[i]);
	}
	
	if (screan)
		draw_display_box(screan, display_box);
	
	display_box_adding = 0;
}

void draw_display_box(SDL_Surface *screan, struct displaybox *display_box)
{
	int i;
	
	for(i=0;i < display_box->lines; i++) //draw all lines
		if (i < display_box->entry_max) //if we have reached our max, then write in nothing for remaining places
		{
			switch (display_box->color[i + display_box->scroll])
			{
			case 0: //white/blueish
				draw_label(screan, display_box->entry[i + display_box->scroll], &display_box->label[i], 0, 240, 240);
				break;
			case 1: //red
				draw_label(screan, display_box->entry[i + display_box->scroll], &display_box->label[i], 174, 1, 1);
				break;
			case 2: //green
				draw_label(screan, display_box->entry[i + display_box->scroll], &display_box->label[i], 0, 201, 14);
				break;
			case 3: //yellowish brown
				draw_label(screan, display_box->entry[i + display_box->scroll], &display_box->label[i], 150, 150, 0);
				break;
			default: //well... white if none else...
				draw_label(screan, display_box->entry[i + display_box->scroll], &display_box->label[i], 0, 240, 240);
				break;
			}
		}
		else
		{
			draw_label(screan, "", &display_box->label[i], 255, 255, 255);
		}
}

void setup_display_box(struct displaybox *display_box)
{
	int i;
	
	for(i=0;i<display_box->lines; i++)
	{
		display_box->label[i].location.x = display_box->x;
		display_box->label[i].location.y = display_box->y + 15 * i;
		display_box->label[i].location.w = display_box->w;
		display_box->label[i].location.h = 15;
		
		display_box->entry[i] = display_box->entry_mem[i]; //just taking advantage of this loop..
	}
	
	for(;i<50;i++) //ah safety
		display_box->entry[i] = display_box->entry_mem[i];
	
	display_box->scroll = 0;
}

void scroll_up_display_box(SDL_Surface *screan, struct displaybox *display_box)
{
	//check
	if(!display_box) return;
	
	//scrolly
	display_box->scroll -= 1;
	
	if(display_box->scroll < 0)
		display_box->scroll = 0;
	
	//drawlly
	if(screan)
		draw_display_box(screan, display_box);
}

void scroll_down_display_box(SDL_Surface *screan, struct displaybox *display_box)
{
	//check
	if(!display_box) return;
	
	//scrolly
	display_box->scroll += 1;
	
	if(display_box->scroll > display_box->entry_max - display_box->lines)
		display_box->scroll = display_box->entry_max - display_box->lines;
	
	if(display_box->scroll < 0)
		display_box->scroll = 0;
	
	if(display_box->scroll + display_box->lines >= DISPLAYBOX_ENTRY_MAX)
		display_box->scroll = (DISPLAYBOX_ENTRY_MAX - 1) - display_box->lines;
	
	//drawlly
	if(screan)
		draw_display_box(screan, display_box);
}

void setup_text_box(struct textbox *text_box)
{
	int i;
	
	//clearrr
	text_box->entry_max = 0;
	text_box->display_box_link = 0;
	
	//clear a donkey
	for(i=0;i<TEXTBOX_SUBMISSION_MAX;i++)
		text_box->past_entry[i] = text_box->past_message[i];
}

void set_text_box_displaybox(struct textbox *text_box, struct displaybox *display_box)
{
	text_box->display_box_link = display_box;
}

void add_text_box_entry(struct textbox *text_box, char *message)
{
	int i;
	char *temp_ptr;
	
	//this entry already in?
	for(i=0;i<text_box->entry_max;i++)
		if(str_match(text_box->past_entry[i],message))
			break;
	
	if(i < text_box->entry_max)
	{
		text_box->entry_cursor = i;
		return;
	}
	
	//move it all
	temp_ptr = text_box->past_entry[TEXTBOX_SUBMISSION_MAX-1];
	
	//shift
	for(i=TEXTBOX_SUBMISSION_MAX-1;i>0;i--)
		text_box->past_entry[i] = text_box->past_entry[i-1];
	
	text_box->past_entry[0] = temp_ptr;

	//add
	strcpy(text_box->past_entry[0],message);

	//inc
	if(text_box->entry_max < TEXTBOX_SUBMISSION_MAX)
		text_box->entry_max += 1;
	
	text_box->entry_cursor = 0;
}

int use_next_text_box_entry(struct textbox *text_box)
{
	//check monkey
	if(text_box->entry_max == 0) return 0;
	
	//use
	strcpy(text_box->message, text_box->past_entry[text_box->entry_cursor]);
	text_box->char_amt = strlen(text_box->message);
	text_box->cursor = text_box->char_amt;
	
	//set
	text_box->entry_cursor += 1;
	
	//check
	if(text_box->entry_cursor >= text_box->entry_max)
		text_box->entry_cursor = 0;
	
	//sex
	return 1;
}

int use_previous_text_box_entry(struct textbox *text_box)
{
	//check monkey
	if(text_box->entry_max == 0) return 0;
	
	//use
	strcpy(text_box->message, text_box->past_entry[text_box->entry_cursor]);
	text_box->char_amt = strlen(text_box->message);
	text_box->cursor = text_box->char_amt;
	
	//set
	text_box->entry_cursor -= 1;
	
	//check
	if(text_box->entry_cursor < 0)
		text_box->entry_cursor = text_box->entry_max - 1;
	
	//sex
	return 1;
}

int init_audio()
{
	SDL_AudioSpec fmt;
	
	/* Set 16-bit stereo audio at 22Khz */
	fmt.freq = 8000;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;        /* A good value for games */
	fmt.callback = mixaudio;
	fmt.userdata = NULL;
	
	if (SDL_OpenAudio(&fmt, NULL) < 0) {
		printf("error: could not load audio: %s\n", SDL_GetError());
		return 0;
	}
	
	SDL_PauseAudio(0);
	
	sound_max = 0;
	
	return 1;
}

int make_sound(char *filename)
{
	int i;
	SDL_AudioCVT cvt;
	
	i = sound_max;

	/* Load the WAV */
	SDL_LoadWAV(filename, &game_sound[i].sound_info, &game_sound[i].data, &game_sound[i].dlen);
	
	//convert it
	SDL_BuildAudioCVT(&cvt, game_sound[i].sound_info.format, game_sound[i].sound_info.channels, game_sound[i].sound_info.freq, AUDIO_S16, 2, 8000);
	cvt.buf = (Uint8 *)malloc(game_sound[i].dlen * cvt.len_mult);
	memcpy(cvt.buf, game_sound[i].data, game_sound[i].dlen);
	cvt.len = game_sound[i].dlen;
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(game_sound[i].data);

	SDL_LockAudio();
	game_sound[i].data = cvt.buf;
	game_sound[i].dlen = cvt.len_cvt;
	game_sound[i].dpos = game_sound[i].dlen;
	SDL_UnlockAudio();
	
	
	game_sound[i].dpos = game_sound[i].dlen;
	
	sound_max++;
	
	return i;
}

void play_sound(int sound_num)
{
	if(!game.sound_on) return; //wir hassen du
	
	if(game_sound[sound_num].dpos < game_sound[sound_num].dlen) return;
	
	game_sound[sound_num].dpos = 0;
}

void play_sound_repeat(int sound_num)
{
	game_sound[sound_num].repeat = 1;
	
	if(!game.sound_on) return; //wir hassen du
	if(!game.engine_on) return; //wir hassen du
	
	game_sound[sound_num].dpos = 0;
	
}


void stop_sound_repeat(int sound_num)
{
	game_sound[sound_num].repeat = 0;
	game_sound[sound_num].dpos = game_sound[sound_num].dlen;
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	int i;
	Uint32 amount;
	
	for (i = 0; i < sound_max;i++) 
	{
		amount = (game_sound[i].dlen - game_sound[i].dpos);
		
		if (amount > (Uint32)len) 
			amount = len;
		
		SDL_MixAudio(stream, &game_sound[i].data[game_sound[i].dpos], amount, SDL_MIX_MAXVOLUME);
		game_sound[i].dpos += amount;
		
		if(game_sound[i].repeat && game_sound[i].dpos >= game_sound[i].dlen && game.sound_on && game.engine_on)
			game_sound[i].dpos = 0;
	}
}

