#include <iostream>
#include <stdlib.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 

using namespace	std;
typedef unsigned char u_int8_t;


//Initialize standard_Lbp function
void standard_Lbp(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3);			//used center pix as a threshold and returns three histogram features for scale 1,2,3


void standard_Lbp(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3){

	int scale;
	int centerPix = 0, indexX = 0, indexY = 0, binConv = 0;

	for(scale = 1; scale < 4; scale++) {

		cout << "LBP operation is running in Scale " << scale << " ... " << endl << endl;

		//create a scale sized IplImage for saving LBP image
		unsigned char **pLbpImg;
		pLbpImg = new unsigned char *[rows - (2 * scale)];

		for(int i = 0; i < (rows - (2 * scale)); i++){
			pLbpImg[i] = new unsigned char[cols - (2*scale)];
		}



		//lbp running for scale 1
		for(int y = 0; y < rows - (2 * scale); y++){
			for(int x = 0; x < cols - (2*scale); x++){

				centerPix = 0, indexX = 0, indexY = 0, binConv = 0;

				centerPix = (int)pGray[y + scale][x + scale];
				//cout << centerPix << endl;			//// Print center pixel data (comment this line if not necessary)

				indexX = x + scale; 
				indexY = y + scale;

				if (centerPix <= (int)pGray[indexY - scale][indexX - scale]){		// pos [0,0]
					//cout << (int)pGray[indexY - scale][indexX - scale] << endl;
					binConv |= (1u << 0);
				}

				if (centerPix <= (int)pGray[indexY - scale][indexX]){				// pos [0,1]
					//cout << (int)pGray[indexY - scale][indexX]<< endl;
					binConv |= (1u << 1);
				}

				if (centerPix <= (int)pGray[indexY - scale][indexX + scale]){		// pos [0,2]
					//cout << (int)pGray[indexY - scale][indexX + scale] << endl;
					binConv |= (1u << 2);
				}

				if (centerPix <= (int)pGray[indexY][indexX + scale]){				// pos [1,2]
					//cout << (int)pGray[indexY][indexX + scale] << endl;
					binConv |= (1u << 3);
				}

				if (centerPix <= (int)pGray[indexY + scale][indexX + scale]){		// pos [2,2]
					//cout << (int)pGray[indexY + scale][indexX + scale] << endl;
					binConv |= (1u << 4);
				}

				if (centerPix <= (int)pGray[indexY + scale][indexX]){				// pos [2,1]
					//cout << (int)pGray[indexY + scale][indexX] << endl;
					binConv |= (1u << 5);
				}

				if (centerPix <= (int)pGray[indexY + scale][indexX - scale]){		// pos [2,0]
					//cout << (int)pGray[indexY + scale][indexX - scale] << endl;
					binConv |= (1u << 6);
				}

				if (centerPix <= (int)pGray[indexY][indexX - scale]){				// pos [1,0]
					//cout << (int)pGray[indexY][indexX - scale] << endl;
					binConv |= (1u << 7);	
				}

				pLbpImg[y][x] = binConv;			//saving LBP values
				//cout << centerPix << "    " << binConv << endl;

				//Adding Histgiram feature
				if (scale == 1)
					histScale1[int(binConv)]++;
				else if (scale == 2)
					histScale2[int(binConv)]++;
				else
					histScale3[int(binConv)]++;

			}
		}


		////Print LBP image values
		//cout << "Print LBP image values ::: " << endl << endl;
		//for (int y = 0; y < rows - (2 * scale); y++)
		//{
		//	for (int x = 0; x < cols - (2*scale); x++)
		//	{
		//		cout << (int) pLbpImg[y][x] << " ";
		//	}
		//	cout << endl;
		//}


		//Create a blank IplImage with zeros filled in
		IplImage *imgSave = cvCreateImage(cvSize(cols - (2*scale), rows - (2 * scale)), 8, 1);
		cvZero(imgSave);

		//moving lbp image data from pLbpImg arry to imgSave IplImage
		for (int y = 0; y < rows - (2 * scale); y++)
		{
			for (int x = 0; x < cols - (2*scale); x++)
			{
				imgSave->imageData[imgSave->widthStep * y + x * 1] = pLbpImg[y][x];
			}
		}
		cout << endl << endl;
	


		if (scale == 1){

			cvSaveImage("savedLbpImgeS1.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedLbpImgeS1.bmp", imgSave))
				cout << "Failed to save LBPimageS1" << endl << endl;
			else
				cout<<"Successfully save LBPimageS1" << endl << endl;

		}
		else if (scale == 2){

		cvSaveImage("savedLbpImgeS2.bmp", imgSave);
		//check save image status
		if (!cvSaveImage("savedLbpImgeS2.bmp", imgSave))
			cout << "Failed to save LBPimageS2" << endl << endl;
		else
			cout<<"Successfully save LBPimageS2" << endl << endl;

		}
		else {

			cvSaveImage("savedLbpImgeS3.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedLbpImgeS3.bmp", imgSave))
				cout << "Failed to save LBPimageS3" << endl << endl;
			else
				cout<<"Successfully save LBPimageS3" << endl << endl;

		}

		//clean memory
		delete [] pLbpImg;
		cvReleaseImage(&imgSave);
		centerPix = 0, indexX = 0, indexY = 0, binConv = 0;

	}

}








int main(){
	//Read input image (just one plane)
	IplImage *inputImg = cvLoadImage("lena512.bmp", 0);
	//IplImage *inputImg = cvLoadImage("filename.png", 0);



	//verify reading image
	if (inputImg)
		cout << "read input image successfully!" << endl << endl;
	else{
		cout << "Fail to read input image!" << endl << endl;
		abort();
	}



	//Extract row and col info
	int rows, cols;
	rows = inputImg->height;
	cols = inputImg->width;



	//copy all gray values in a 2D matrix
	unsigned char **pGray;
	pGray = new unsigned char *[rows];

	for(int i = 0; i < rows; i++){
		pGray[i] = new unsigned char[cols];
	}



	cout << "Print Image data ::: " << endl << endl;			//(comment this line if not necessary)
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			pGray[y][x] = inputImg->imageData[inputImg->widthStep * y + x * 1];
			//cout << (int) pGray[y][x] << " ";		// Print Image data (comment this line if not necessary)
		}
		cout << endl;
	}
	cout << endl << endl;



	//intialize histScale for saving histogram feature
	const int binSize = 256;
	int histScale1[binSize] = {0}, histScale2[binSize] = {0}, histScale3[binSize] = {0};
	int scale = 1;		//scale = 1
	double featureConCat[binSize * 3] = {0};	//For concatinating three histogram feature


	//Perfrom standred_Lbp function 
	standred_Lbp(pGray, rows, cols, histScale1, histScale2, histScale3);		


	//Concatinating features of scale 1 to 3
	for (int i = 0; i < 256; i++){
		featureConCat[i] = (double)histScale1[i];
		featureConCat[i + (256 * 1)] = (double)histScale2[i];
		featureConCat[i + (256 * 2)] = (double)histScale3[i];
	}


	
	cin.get();		// press enter to finish the execution
	return 0;

}