#include <am.h>
#include <stdio.h>
#include <klib.h> 
#include <klib-macros.h>
#define MAX3(a,b,c) ((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))
#define UDIFF(a,b) (((a)>(b)) ? ((a)-(b)) : ((b)-(a)))
#define sign(sign,a,b) if(sign){(a)-=(b);}else{(a)+=(b);}
uint32_t current_x = 0;
void draw(uint32_t *color,int w,int h) {
  // change the code below
  io_write(AM_GPU_FBDRAW, 0, 0, color, w,h,true);
}
void draw_line(uint32_t *line_color,int x,int h){
  io_write(AM_GPU_FBDRAW,x,0,line_color,1,h,true);
}
uint32_t calculate(int j,uint32_t color0,uint32_t color_k){
	uint32_t low_abs = UDIFF((color_k & 0xff),(color0 & 0xff));
	uint32_t mid_abs = UDIFF((color_k & 0xff00),(color0 & 0xff00));
	uint32_t top_abs = UDIFF((color_k & 0xff0000),(color0 & 0xff0000));

	int32_t low = ((color_k & 0xff) - (color0 & 0xff));
	int32_t mid = ((color_k & 0xff00) - (color0 & 0xff00));
	int32_t top = ((color_k & 0xff0000) - (color0 & 0xff0000));
/*
	float low_k;if(low==0){low_k=0;}else{low_k = (float)j/low_abs;}
	float mid_k;if(mid==0){mid_k=0;}else{mid_k = (float)j/(mid_abs>>8);}
	float top_k;if(top==0){top_k=0;}else{top_k = (float)j/(top_abs>>16);}
*/	
	int low_k=1,mid_k=1,top_k=1;
	if(j > low_abs)low_k = 0;
	if(j > (mid_abs>>8))mid_k = 0;
	if(j > (top_abs>>16))top_k = 0;

	//uint32_t color_return = (((color0 + (top_abs * top_k)) + (mid_abs * mid_k)) + (low_abs * low_k));
	uint32_t color_return = color0;
	sign((low<0),color_return,j* 0x000001 * low_k);
	sign((mid<0),color_return,j* 0x000100 * mid_k);
	sign((top<0),color_return,j* 0x010000 * top_k);
	return color_return;
}
uint32_t max(uint32_t color0,uint32_t color_k){

	uint32_t low = ((color_k & 0xff) - (color0 & 0xff));
	uint32_t mid = ((color_k & 0xff00) - (color0 & 0xff00));
	uint32_t top = ((color_k & 0xff0000) - (color0 & 0xff0000));

	return MAX3(low,(mid>>8),(top>>16));
}
void line_gradient(int w,int h,uint32_t color0,uint32_t color_k){
	uint32_t *color_buf = malloc(sizeof(uint32_t)*h);
  
	color_buf[0] = color0;
	int j = 0;
	while((color_buf[0] != color_k) && (current_x < w)){
		for (int i = 0; i < h; i++) {
		    color_buf[i] = calculate(j,color0,color_k);
		}
		draw_line(color_buf,current_x,h);
		j++;
		current_x++;
	}
	free(color_buf);
}
int main() {
  ioe_init(); // initialization for GUI
  int w = io_read(AM_GPU_CONFIG).width;
  int h = io_read(AM_GPU_CONFIG).height;
//  line_gradient(w,h,0xad099a,0x0000ff);
  line_gradient(w,h,0xff0000ff,0x00000000);
  line_gradient(w,h,0x00000000,0xff0000ff);
  printf("%d %d",w,h);
  while (1);
  return 0;
}
