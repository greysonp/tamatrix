/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include "lcd.h"
#include <stdio.h>
#include <string.h>

void lcdShow(Display *lcd) {
	int i;
	int x,y;
	char *icons[]={"INFO", "FOOD", "TOILET", "DOORS", "FIGURE",
				"TRAINING", "MEDICAL", "IR", "ALBUM", "ATTENTION"};
	const char *grays[4]={"█","▓","░"," "};
	printf("\033[45;1H\033[1J\033[1;1H");
	for (y=0; y<32; y++) {
		for (x=0; x<48; x++) {
			printf("%s%s", grays[lcd->p[y][x]&3], grays[lcd->p[y][x]&3]);
		}
		putchar('\n');
	}
	printf(">>> ");
	i=lcd->icons;
	for (x=0; x<10; x++) {
		if (i&1) printf("%s ", icons[x]);
		i>>=1;
	}
	printf("<<<\n");
}

void lcdCopy(Display *out, Display *in) {
	int y;
	for (y=0; y<32; y++) {
		memcpy(out->p[y], in->p[y], 48);
	}
	out->icons=in->icons;
}

int lcdSame(Display *a, Display *b) {
	int y;
	for (y=0; y<32; y++) {
		if (memcmp(a->p[y], b->p[y], 48)!=0) return 0;
	}
	if (a->icons!=b->icons) return 0;
	return 1;
}

//tama lcd is 48x32
void lcdRender(uint8_t *ram, int sx, int sy, Display *lcd) {
	int x, y;
	int b, p;
	for (y=0; y<sy+1; y++) {
		for (x=0; x<sx; x++) {
			if (y>=16) {
				p=x+(sy-y-1)*sx;
			} else {
				p=x+(sy-(15-y)-1)*sx;
			}
			b=ram[p/4];
			b=(b>>((3-(p&3))*2))&3;
			if (y<32 && x<48) lcd->p[y][x]=b;
		}
	}
	
	y=1;
	lcd->icons=0;
	for (x=19; x<29; x++) {
		b=ram[x/4];
		b=(b>>((3-(x&3))*2))&3;
		if (b!=0) lcd->icons|=y;
		y<<=1;
	}
}

void lcdDump(uint8_t *ram, int sx, int sy, char *out) {
	int x, y;
	Display lcd;
	char ochar[]="   X";
	FILE *f=fopen(out, "a");
	if (f==NULL) return;
	lcdRender(ram, sx, sy, &lcd);
	for (y=0; y<32; y++) {
		for (x=0; x<48; x++) {
			fprintf(f, "%c", ochar[lcd.p[y][x]&3]);
		}
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
	fclose(f);
}

