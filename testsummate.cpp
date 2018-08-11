#include <iostream>
using namespace std;

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <hls_opencv.h>
#include <opencv2/core/core.hpp>

//------------------------From summation.h --------------------------------//
// Use the axi stream side-channel (TLAST,TKEEP,TUSR,TID)
#include <ap_axi_sdata.h>
typedef ap_axiu<8,2,5,6> uint_8_side_channel;
typedef ap_axiu<8,2,5,6> binary;


#define img_x 274
#define img_y 320
//-----------------------------------------------------
//Opencv to load image
#define INPUT_IMAGE_CORE "/home/ab/Downloads/doc2.png"
#define OUTPUT_IMAGE "/home/ab/Downloads/b_image.txt"





void loc_binarizer(hls::stream<uint_8_side_channel> &inStream, hls::stream<binary> &outStream);


void saveImage(const char* filename, char image[][img_y])
{
	// Open file to compare later ....
		FILE *pFile;
		pFile = fopen(filename,"w");
		if (pFile != NULL)
		{
			for (int r = 0;  r < img_x; r++)
			{
				for(int c = 0; c<img_y; c++)
				fprintf(pFile,"%d ", image[r][c]);

				fprintf(pFile,"\n");
			}
		}
}

//-------------------------------------------------------------------------//
int main()
{
	hls::stream<uint_8_side_channel> inputStream;
	hls::stream<binary> outputStream;

	// Read input image
	printf("Loading Image .......\n");
	cv::Mat imageSrc;
	imageSrc = cv::imread(INPUT_IMAGE_CORE);
	// Convert to grayscale
	cv::cvtColor(imageSrc, imageSrc, CV_BGR2GRAY);
	printf("Image Rows:%d Cols:%d\n",imageSrc.rows, imageSrc.cols);



	uint_8_side_channel valIn;

	printf("Input\n");
	for(int r = 0; r<img_x; r++)
	{
		for(int c = 0; c<img_y; c++)
		{
			//printf("%d\t",A[r][c]);
			valIn.data = imageSrc.at<unsigned char>(r,c);;
			valIn.keep = 1; valIn.strb=1; valIn.user=1;
			valIn.id = 0; valIn.dest=0;
			inputStream << valIn;
		}
	}
	cv::imshow("Original Image", imageSrc);

	//cv::waitKey(0);

	printf("Performing function on RTL");
	//Function declared in "summation.h"

	loc_binarizer(inputStream, outputStream);


	binary valOut;
	printf("Output\n");

	//unsigned char bin_image[img_x][img_y]={99};

	unsigned char bin_matrix[img_x][img_y] = {0};

	for(int r = 0; r<img_x; r++)
	{
		for(int c = 0; c<img_y; c++)
		{

			outputStream.read(valOut);
			//printf("%d\t", (int)valOut.data);
			//bin_image.at<uchar>(r,c)=(int)valOut.data;
			bin_matrix[r][c]=(int)valOut.data;
			//if(bin_matrix[r][c]==1)
			//	bin_matrix[r][c]=255;
		}
	}
	//Mat size=(columns,rows)
	cv::Mat bin_image (cv::Size(img_y,img_x), CV_8U, bin_matrix);
	bin_image = 255*bin_image;
	/*
	for(int r = 0; r< 3; r++)
	{
		for(int c=0; c<img_y; c++)
		{
			printf("%d ",bin_image.at<uchar>(r,c));
		}
		printf("\n");
	}
	*/

	printf("Saving Binary Image ..........");
	//cv::imwrite('binary_doc.bmp', bin_image);
	cv::imshow("Binary Image", bin_image);
	cv::waitKey(0);

	//saveImage(OUTPUT_IMAGE,b);
	printf("\nDone\n");
	return 0;
}

