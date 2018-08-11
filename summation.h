// Use the axi stream side-channel (TLAST,TKEEP,TUSR,TID)
#include <ap_axi_sdata.h>
typedef ap_axiu<8,2,5,6> uint_8_side_channel;

//******8 bit means

#define img_x 274
#define img_y 320

//---------------------------------------------------------------------------------------
//#define img_x 3
//#define img_y 3

#define window_size 5 //Ensure that window size is odd

#define pre_pad ((window_size+1)/2)
#define post_pad ((window_size-1)/2)

//---------------------------------------------------------------------------------------

void pre_padarray(unsigned int image[][img_y],char pad_size[2], unsigned int padded_img[][img_y+pre_pad]);
void post_padarray(unsigned int padded_img[][img_y+pre_pad],char pad_size[2], unsigned long double_padded[][img_y+pre_pad+post_pad]);
//This function sends out mean of image in same shape as input image
void summate(unsigned int in[][img_y], float mean[][img_y]);

