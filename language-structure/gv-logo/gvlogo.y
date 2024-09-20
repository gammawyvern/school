%{
#define WIDTH 640
#define HEIGHT 480

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <string.h>

static SDL_Window* window;
static SDL_Renderer* rend;
static SDL_Texture* texture;
static SDL_Thread* background_id;
static SDL_Event event;
static int running = 1;
static const int PEN_EVENT = SDL_USEREVENT + 1;
static const int DRAW_EVENT = SDL_USEREVENT + 2;
static const int COLOR_EVENT = SDL_USEREVENT + 3;

typedef struct color_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color;

static color current_color;
static double x = WIDTH / 2;
static double y = HEIGHT / 2;
static int pen_state = 1;
static double direction = 0.0;

/***************************************
 * Stores a value based on id.
 * Lifetime is rest of program, type is
 * only float. id is a string but idc.
 * 
 * Use link list to make easier to
 * implement. Not fast for searching but
 * doesn't realistically matter here. 
 **************************************/

typedef struct Symbol {
	char* id;
	float value;	
	struct Symbol* next_symbol;
} symbol;

static symbol* symbol_table_head = NULL;

void assign_id(char* id, float value);
float get_value(char* id);
void free_symbol_table();

/**************************************/

int yylex(void);
int yyerror(const char* s);
void startup();
int run();
void prompt();
void penup();
void pendown();
void move(int num);
void turn(int dir);
void goto_pos(float x, float y);
void output(const char* s);
void change_color(int r, int g, int b);
void clear();
void save(const char* path);
void shutdown();

%}

%union {
	float f;
	char* s;
}

%locations

%token SEP
%token PENUP
%token PENDOWN
%token PRINT
%token CHANGE_COLOR
%token COLOR
%token CLEAR
%token TURN
%token LOOP
%token MOVE
%token END
%token SAVE
%token GOTO
%token WHERE
%token PLUS SUB MULT DIV EQ
%token<f> NUMBER
%token<s> STRING ID FILE_PATH
%type<f> expression id

%%

program:		statement_list END				{ printf("Program complete.\n"); shutdown(); exit(0); }
       		|	END						{ printf("Program complete.\n"); shutdown(); exit(0); }
		;

statement_list:		statement					
	      	|	statement statement_list			
		;

statement:		command SEP					{ prompt(); }
	 	|	expression SEP					{ printf("\t%f\n", $1); prompt(); }
			/* TODO Could also separate into 'assign' to be consistent */
		|	ID EQ expression SEP				{ assign_id($1, $3); printf("\t%s = %f\n", $1, $3); prompt(); }
		|	error SEP 					{ yyerrok; prompt(); }
		;

command:		PENUP						{ penup(); }
       		|	PENDOWN						{ pendown(); }
		|	PRINT STRING					{ printf("\t%s\n", $2); free($2); }
		|	PRINT expression				{ printf("\t%f\n", $2); }
		|	SAVE FILE_PATH					{ save($2); free($2); }
		|	CHANGE_COLOR expression expression expression	{ change_color($2, $3, $4); }
		|	CLEAR						{ clear(); }
		|	TURN expression 				{ turn($2); } 
		|	MOVE expression 				{ move($2); }
		|	GOTO expression expression			{ goto_pos($2, $3); }
		|	WHERE						{ printf("\t(x: %f, y: %f)\n", x, y); }
		;

expression:		id PLUS expression			{ $$ = $1 + $3; }
		|	id MULT expression			{ $$ = $1 * $3; }
		|	id SUB expression			{ $$ = $1 - $3; }
		|	id DIV expression			{ $$ = $1 / $3; }
		|	NUMBER PLUS expression			{ $$ = $1 + $3; }
		|	NUMBER MULT expression			{ $$ = $1 * $3; }
		|	NUMBER SUB expression			{ $$ = $1 - $3; }
		|	NUMBER DIV expression			{ $$ = $1 / $3; }
		|	id
		|	NUMBER
		;

id:			ID						{ $$ = get_value($1); }
		;  

%%

int main(){
	startup();
	return 0;
}

int yyerror(const char* s){
	printf("Error: %s\n", s);
	return -1;
};

void prompt(){
	printf("gv_logo > ");
}

void penup(){
	event.type = PEN_EVENT;		
	event.user.code = 0;
	SDL_PushEvent(&event);
}

void pendown() {
	event.type = PEN_EVENT;		
	event.user.code = 1;
	SDL_PushEvent(&event);
}

void move(int num){
	event.type = DRAW_EVENT;
	event.user.code = 1;
	event.user.data1 = (void*)(uintptr_t)num;
	SDL_PushEvent(&event);
}

void goto_pos(float x2, float y2) {
	// Stole this code block directly from 
	// SDL event handler you made above
	if(pen_state != 0){
		SDL_SetRenderTarget(rend, texture);
		SDL_RenderDrawLine(rend, x, y, x2, y2);
		SDL_SetRenderTarget(rend, NULL);
		SDL_RenderCopy(rend, texture, NULL, NULL);
	}

	x = x2;
	y = y2;
}

void turn(int dir){
	event.type = PEN_EVENT;
	event.user.code = 2;
	event.user.data1 = (void*)(uintptr_t)dir;
	SDL_PushEvent(&event);
}

void output(const char* s){
	printf("%s\n", s);
}

void change_color(int r, int g, int b){
	event.type = COLOR_EVENT;
	current_color.r = r;
	current_color.g = g;
	current_color.b = b;
	SDL_PushEvent(&event);
}

void clear(){
	event.type = DRAW_EVENT;
	event.user.code = 2;
	SDL_PushEvent(&event);
}

void startup(){
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("GV-Logo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL){
		yyerror("Can't create SDL window.\n");
	}
	
	//rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	if(texture == NULL){
		printf("Texture NULL.\n");
		exit(1);
	}
	SDL_SetRenderTarget(rend, texture);
	SDL_RenderSetScale(rend, 3.0, 3.0);

	background_id = SDL_CreateThread(run, "Parser thread", (void*)NULL);
	if(background_id == NULL){
		yyerror("Can't create thread.");
	}
	while(running){
		SDL_Event e;
		while( SDL_PollEvent(&e) ){
			if(e.type == SDL_QUIT){
				running = 0;
			}
			if(e.type == PEN_EVENT){
				if(e.user.code == 2){
					double degrees = ((int)(intptr_t)e.user.data1) * M_PI / 180.0;
					direction += degrees;
				}
				pen_state = e.user.code;
			}
			if(e.type == DRAW_EVENT){
				if(e.user.code == 1){
					int num = (int)(intptr_t)event.user.data1;
					double x2 = x + num * cos(direction);
					double y2 = y + num * sin(direction);
					if(pen_state != 0){
						SDL_SetRenderTarget(rend, texture);
						SDL_RenderDrawLine(rend, x, y, x2, y2);
						SDL_SetRenderTarget(rend, NULL);
						SDL_RenderCopy(rend, texture, NULL, NULL);
					}

					x = x2;
					y = y2;
				} else if(e.user.code == 2){
					SDL_SetRenderTarget(rend, texture);
					SDL_RenderClear(rend);
					SDL_SetTextureColorMod(texture, current_color.r, current_color.g, current_color.b);
					SDL_SetRenderTarget(rend, NULL);
					SDL_RenderClear(rend);
				}
			}
			if(e.type == COLOR_EVENT){
				SDL_SetRenderTarget(rend, NULL);
				SDL_SetRenderDrawColor(rend, current_color.r, current_color.g, current_color.b, 255);
			}
			if(e.type == SDL_KEYDOWN){
			}

		}
		//SDL_RenderClear(rend);
		SDL_RenderPresent(rend);
		SDL_Delay(1000 / 60);
	}
}

int run(){
	prompt();
	yyparse();
	return 0;
}

void shutdown(){
	free_symbol_table();	
	running = 0;
	SDL_WaitThread(background_id, NULL);

	// TODO we got some problems here,
	// but I really don't know why.
	// This may cause some memory leaks,
	// but what else can I do?
	//SDL_DestroyWindow(window);
	//SDL_Quit();
}

void save(const char* path){
	printf("Should work\n");
	SDL_Surface *surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_RenderReadPixels(rend, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	SDL_SaveBMP(surface, path);
	SDL_FreeSurface(surface);
}

/***************************************
 * My stuff
 **************************************/

void assign_id(char* id, float value) {
	symbol* sym = symbol_table_head;
	while (sym != NULL) {
		if (!strcmp(sym->id, id)) {
			sym->value = value;			
			return;
		}

		sym = sym->next_symbol;
	}

	// If ID not found, add it, duhh...
	symbol* new_sym = (symbol*)malloc(sizeof(symbol));
	// ID was already duped, so just use it here
	// and free it later
	new_sym->id = id;
	new_sym->value = value;
	
	// Add to start of table
	new_sym->next_symbol = symbol_table_head;
	symbol_table_head = new_sym;
}

float get_value(char* id) {
	symbol* sym = symbol_table_head;
	while (sym != NULL) {
		if (!strcmp(sym->id, id)) {
			return sym->value;
		}

		sym = sym->next_symbol;
	}

	yyerror("ID not found");
	return 0;
}

void free_symbol_table() {
	while (symbol_table_head != NULL) {
		symbol* del_sym = symbol_table_head;
		symbol_table_head = symbol_table_head->next_symbol;
		free(del_sym);
	}
}

