#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

int 
main(){

    int fd = 0;
    fd = open("/dev/fb0", O_RDWR);
    if (fd == -1) {
    	perror("Error: cannot open framebuffer device");
    	exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo; 
    //Get variable screen information
    ioctl(fd, FBIOGET_VSCREENINFO, &vinfo);

    //Get fixed screen information
    ioctl(fd, FBIOGET_FSCREENINFO, &finfo);

    printf("vinfo.bits_per-pixel: %d\n",vinfo.bits_per_pixel);
    close(fd);
    return(0);
}
