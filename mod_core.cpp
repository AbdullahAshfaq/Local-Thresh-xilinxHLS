#include <hls_stream.h>
#include <hls_math.h>
//#include "summation.cpp"
#include "summation.h"

#include <math.h>

#include <ap_axi_sdata.h>
typedef ap_axiu<8,2,5,6> uint_8_side_channel;
typedef ap_axiu<8,2,5,6> binary;

#include <ap_int.h> //Used later to produce binary data type
#include <ap_fixed.h>
typedef ap_ufixed<23,21> my_float;

void loc_binarizer(hls::stream<uint_8_side_channel> &inStream, hls::stream<unsigned char> &outStream);



void loc_binarizer(hls::stream<uint_8_side_channel> &inStream, hls::stream<unsigned char> &outStream)
{
#pragma HLS INTERFACE axis port=inStream
#pragma HLS INTERFACE axis port=outStream
#pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	//For summate to work, input=unsigned int output=double
	unsigned int image[img_x][img_y];
	my_float mean[img_x][img_y];
	unsigned int image_sq[img_x][img_y];
	my_float mean_sq[img_x][img_y];


	//uint_8_side_channel valIn[img_x][img_y];

		//-----------------Loop over the image to get it through the inStream-------------------//
		for(int r = 0; r<img_x; r++)
		{
			for(int c = 0; c<img_y; c++)
			{
	#pragma HLS PIPELINE

				//valIn[r][c] = inStream.read();
				//image[r][c] = valIn[r][c].data;
				image[r][c] = inStream.read().data;
			}
		}
		//----------------------- Calculating mean of image and image squared ----------------//

		//Populating "mean"
		summate(image,mean);
		//creating square of image
		for(int r = 0; r<img_x; r++)
			{
				for(int c = 0; c<img_y; c++)
				{
		#pragma HLS PIPELINE
					image_sq[r][c] = std::pow(image[r][c],2);
				}
			}

		//Calculating mean of square image
		//Summate should be able to take "int" as input and give "double" output
		summate(image_sq,mean_sq);

		//---------------------------------------Calculating Std_dev---------------------------//
		//double deviation[img_x][img_y];
		float R = 0;//Will be used in threshold calculation


		/*
		for(int r = 0; r<img_x; r++)
			{
				for(int c = 0; c<img_y; c++)
				{
		#pragma HLS PIPELINE
					deviation[r][c]=hls::sqrt(mean_sq[r][c]-std::pow(mean[r][c],2.0));

					if (deviation[r][c]>R)
						R=deviation[r][c];
				}

			}*/
		//-----------------------------Calculating local thresholds--------------------//
		float dev;
		//float thres[img_x][img_y];
		float k = 0.34;
		float threshold;

		//To Do: change sqrt and pow to shifting operations
		/*
		 * void CalculateSomeTrig(ap_fixed<16,5>* angle, ap_fixed<16,5>* output)
							{
								ap_fixed<32,5> functionInput = *angle;
								*output = hls::sinf(functionInput);
							}
		 * */

		float temp_mean_sq;
		float temp_mean;

		for(int r = 0; r<img_x; r++)
				{
					for(int c = 0; c<img_y; c++)
					{
			#pragma HLS PIPELINE
						temp_mean_sq = mean_sq[r][c];
						temp_mean = mean[r][c];
						//sqrt and pow is defined for float so assign in
						dev = hls::sqrt(temp_mean_sq-std::pow(temp_mean,2.0));

						if (dev > R)
								R = dev;
						threshold = temp_mean*(1-k+(k/R)*dev);
						if (image[r][c]>threshold)
							outStream.write(char(1));
						else
							outStream.write(char(0));
					}

				}

		//******* Modified to find deviation and threshold and write at same time *********//


		// --------------------------------- Binarization----------------------------------//

		//unsigned char bin_image[img_x][img_y];
		/*
		for(int r = 0; r<img_x; r++)
						{
							for(int c = 0; c<img_y; c++)
							{
					#pragma HLS PIPELINE

								if(image[r][c]>thres[r][c])
									{//bin_image[r][c] = 1;
										outStream.write(char(1));
									}
								else{
									//bin_image[r][c] = 0;
									outStream.write(char(0));
								}

							}
						}*/

		// ---------------------------------- Output binary image to outStream------------------------//
		/*
				for(int r = 0; r<img_x; r++)
						{
							for(int c = 0; c<img_y; c++)
							{
					#pragma HLS PIPELINE

								outStream.write(bin_image[r][c]);
							}
						}

		 */

}

