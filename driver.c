/* Friedrich Doku frd20
 * Driver code to test grphics library */
#include "graphics.h"


static color_t* red = NULL;
static color_t* blue = NULL;
static color_t* green = NULL;
static color_t* cyan = NULL;
static char* buff = NULL;

int
test_color_creation(){
    red =   create_color(30, 0, 0);
    blue =  create_color(0, 0, 31);
    green = create_color(0, 63, 0);
    cyan =  create_color(0, 63, 31);
}

int
test_getkey(){
    char* prompt = "Enter a key: ";
    draw_text(buff, 0, 0, prompt, *cyan);
    blit(buff);

    char rez = get_key();
    char* output = NULL;
    asprintf(&output, "I GOT %c", rez);

    draw_text(buff, 0, 16, output, *green);
    blit(buff);

    sleep_ms(10000);
    clear_screen(buff);
    blit(buff);
}

int
test_draw_line(){
    draw_line(buff, 0, 0, 100, 200, *blue);
    draw_line(buff, 0, 0, 200, 0, *cyan);
    draw_line(buff, 0, 100, 400, 100, *green);
    draw_line(buff, 0, 200, 600, 300, *red);
    blit(buff);
    
    sleep_ms(10000);
    clear_screen(buff);
    blit(buff);
}

int
test_draw_text(){
    char* jfk = "We shall send to the moon 240,000 miles away, a giant rocket,"
        " more than 300 feet tall on an untried mission to an unknown celestial body,"
        " and then return it safely to Earth. But why some say the moon? Why choose this"
        " as our goal? And they may well ask, why climb the highest mountain? Why 35 years"
        " ago fly the Atlantic? We choose to go to the moon. We chose to go to the moon."
        " We choose to go to the moon in this decade and do the other things not because"
        " they are easy, but because they are hard. Because that goal will serve to organize"
        " and measure the best of our energies and skills, because that challenge is one that"
        " we’re willing to accept. One we are unwilling to postpone. And therefore, as we set"
        " sail, we ask God’s blessing on the most hazardous and dangerous and greatest adventure that man has ever gone.";

    draw_text(buff, 0, 0, jfk, *cyan);
    blit(buff);
    
    sleep_ms(10000);
    clear_screen(buff);
    blit(buff);
}

int
main(){
	init_graphics();


    /* Create a new buffer */
    buff = (char*) new_offscreen_buffer();
    test_color_creation();
    test_getkey();
    test_draw_line();
    test_draw_text();
    exit_graphics();
	return 0;
}	


