#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>

static char* data = NULL;
static int fd;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
static int size;
static struct termios restore;

struct color {
    union {
    	uint16_t val;
		struct {
			uint16_t red : 5;
			uint16_t green : 6;
			uint16_t blue : 5;
		};
	};
} __attribute__((packed));

typedef struct color color_t;

color_t*
create_color(uint16_t r, uint16_t g, uint16_t b){
	color_t* c = (color_t*) malloc(sizeof(color_t));
	c->red = r;
	c->green = g;
	c->blue = b;
	
	return c;
}

int
get_screen_size(int fd){
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

//    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    size = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
}

void
disable_echo(){
	/*NOTE: THIS WAY MAY NOT BE ALLOWED FIX LATER*/
	
	struct termios t;
	
	tcgetattr(fileno(stdin), &t);
	/* create a copy to restore on exit */
	tcgetattr(fileno(stdin), &restore);
	
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fileno(stdin), TCSANOW, &t);
}

int
init_graphics(){
	fd = open("/dev/fb0", O_RDWR);
	if(fd == -1){
		perror("FAILED TO OPEN DEVICE");
		return -1;
	}
	
	printf("/dev/fb0 is open.\n");

	/* Get screen dimensions */
	get_screen_size(fd);
	
	/* Map in the file with mmamp */
	data = mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

	disable_echo();
		
	return 0;	
}

int
exit_graphics(){
	
	/* Unmap memory */
	int rez = munmap(data, size);
	if(rez < 0){
	    perror("FAILED TO UNMAP MEMORY \n");
	}
	
	/* Close fd */
	close(fd);
	
	/* Restore terminal settings */
	tcsetattr(fileno(stdin), TCSANOW, &restore);
	
	return 0;
}

unsigned char*
get_key(){
	unsigned char* ch = NULL;
	
	fd_set rfds;
	int retval;
	
	/* watch stdin */
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);	
	
	retval = select(1, &rfds, NULL, NULL, NULL);
	
	if(retval == -1){
		perror("select failed\n");
	} else if (retval){
		read(fileno(stdin), ch);
	}
	
	return ch;
}

int
sleep_ms(long ms){
	struct timespec req;
	req.tv_sec = (int) (ms / 1000);
	req.tv_nsec = (ms % 10000) * 1000000;
	
	return nanosleep(&req, NULL);
}

char*
new_offscreen_buffer(){
	char* buf = mmap(NULL, 	size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if(buf == NULL){
		perror("FAILED TO ALLOCATE MEMORY FOR BUFFER\n");
		return NULL;
	}
		
	return buf;
}

void
blit(char* buf){
	if(buf != NULL){
        int i;
		for(i = 0; i < size; i++){
			data[i] = buf[i];
		}
	}else{
		perror("blit FAILED: Buffer is NULL\n");
	}
}

void 
clear_screen(char* buff){
	int i;
    for(i = 0; i < size; i++){
		buff[i] = 0;
	}
}

int
draw_pixel(char* buff, int x, int y, color_t c){
    long int pos = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                       (y + vinfo.yoffset) * finfo.line_length;

    *((unsigned short int*)(buff + pos)) = c.val;
}



int
main(){
	init_graphics();
    
    char* buff = new_offscreen_buffer();
    
    color_t c;
    c.red = 20;

    int i;
    for(i = 0; i < 50; i++){
        draw_pixel(buff, i, 100, c);
    }

    blit(buff);

    exit_graphics();
	return 0;
}	


