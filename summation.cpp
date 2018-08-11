// Use the class stream
#include <hls_stream.h>
#include <hls_math.h>

//Constants are defined
//Function declarations
#include "summation.h" //macros are defined here

//pow is not part of hls_math
#include <math.h>




void pre_padarray(unsigned int image[][img_y],char pad_size[2], unsigned int padded_img[][img_y+pre_pad])
{	//Rows and Columns of the output image are passed

	for(int r=0; r<img_x+pad_size[0]; r++)
	{
		for(int c = 0; c<img_y+pad_size[1]; c++)
		{
			//Handling border cases
			if(r <= pad_size[0] && c <= pad_size[1])
							{
								padded_img[r][c] = image[0][0];
							}

			else if(r > pad_size[0] && c <= pad_size[1])
							{
								padded_img[r][c] = image[r-pad_size[0]][0];
							}
			else if(r <= pad_size[0] && c > pad_size[1])
							{
								padded_img[r][c] = image[0][c-pad_size[1]];
							}

			else //For other cases (r > pad_size[0] && c > pad_size[1])
				//, just copy image to padded image
			{
				padded_img[r][c] = image[r-pad_size[0]][c-pad_size[1]];
			}

		}
	}

	//padded_img is returned as it is passed by reference
}

void post_padarray(unsigned int padded_img[][img_y+pre_pad],char pad_size[2], unsigned long double_padded[][img_y+pre_pad+post_pad])
{	//rows and columns is telling about size of pa

    for(int r=0; r<img_x+pre_pad+post_pad; r++)
				{
					for(int c = 0; c<img_y+pre_pad+post_pad; c++)
					{
						//Handling border cases
						if(r < img_x+pad_size[0] && c < img_y+pad_size[1]) //case for top left
									{
										double_padded[r][c] = padded_img[r][c];
									}

						else if(r >= img_x+pad_size[0] && c < img_y+pad_size[1])// case for bottom left
									{
										double_padded[r][c] = padded_img[img_x+pad_size[0]][c];
									}
						else if(r >= img_x+pad_size[0] && c >= img_y+pad_size[1])// case for bottom right
									{
										double_padded[r][c] = padded_img[img_x+pad_size[0]][img_y+pad_size[1]];
									}

						else //For other cases (r < img_x && c >= img_y),//case for top right
							 //, just copy image to padded image
						{
							double_padded[r][c] = padded_img[r][img_y+pad_size[1]];
						}


					}

				}


    //double_padded is returned as it is passed by reference
}



void summate(unsigned int image[][img_y], float mean[][img_y])
{
	unsigned int padded_img[img_x+pre_pad][img_y+pre_pad]={0};
	unsigned long double_padded[img_x+pre_pad+post_pad][img_y+pre_pad+post_pad]={0};

	char pre_p_size[2]={int((window_size+1)/2),int((window_size+1)/2)};
	char post_p_size[2]={int((window_size-1)/2),int((window_size-1)/2)};


	//------------------------------------------Padding-----------------------------------//
	pre_padarray(image,pre_p_size,padded_img);
	post_padarray(padded_img,post_p_size,double_padded);

	//--------------------------------- Making Integral Image ---------------------------//

	for(int r = 0; r<img_x+window_size; r++)
	{
		for(int c = 0; c<img_y+window_size; c++)
		{
			if (r > 0)
			   double_padded[r][c] = double_padded[r][c] + double_padded[r-1][c];

			if (c > 0)
				double_padded[r][c] = double_padded[r][c] + double_padded[r][c-1];

			if (r>0 && c>0)
				double_padded[r][c] = double_padded[r][c] - double_padded[r-1][c-1];

		}
	}
	//------------------------------Calculating Mean--------------------------------//
	//double mean[img_x][img_y] = {0};

	for(int r = 0 + window_size; r<img_x+window_size; r++)
	{
		for(int c = 0 + window_size; c<img_y+window_size; c++)
		{
			 mean[r-window_size][c-window_size] = double_padded[r][c] + double_padded[r-window_size][c-window_size]- double_padded[r][c-window_size] - double_padded[r-window_size][c];
			 mean[r-window_size][c-window_size] = mean[r-window_size][c-window_size]/(std::pow(window_size,2));
		}
	}

	//mean is returned

}

