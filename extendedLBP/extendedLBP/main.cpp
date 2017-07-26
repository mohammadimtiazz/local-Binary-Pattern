#include <iostream>
#include <stdlib.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 
#include <math.h>

using namespace	std;
typedef unsigned char u_int8_t;

//Initialize extended LBP function

//NI_LBP takes avg of the pix in current window, used it as a threshold and returns three histogram features for scale 1,2,3
void extended_Lbp_ni(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3);		


//CI_LBP takes avg of the entire image, used it as a threshold to binarize central pixels and returns three histogram features for scale 1,2,3
void extended_Lbp_ci(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3);


//Supporting functions
double round(double d);		//rounding



void extended_Lbp_ni(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3){

	int scale;
	int indexX = 0, indexY = 0, binConv = 0;
	double pixAvgInWin = 0.0;

	for(scale = 1; scale < 4; scale++) {

		cout << "ExNiLBP operation is running in Scale " << scale << " ... " << endl << endl;

		//create a scale sized IplImage for saving LBP image
		unsigned char **pLbpImg;
		pLbpImg = new unsigned char *[rows - (2 * scale)];

		for(int i = 0; i < (rows - (2 * scale)); i++){
			pLbpImg[i] = new unsigned char[cols - (2*scale)];
		}



		//lbp running for scale 1
		for(int y = 0; y < rows - (2 * scale); y++){
			for(int x = 0; x < cols - (2*scale); x++){

				indexX = 0, indexY = 0, binConv = 0;

				indexX = x + scale; 
				indexY = y + scale;


				pixAvgInWin = round( ( (double)pGray[indexY - scale][indexX - scale] + (double)pGray[indexY - scale][indexX] + (double)pGray[indexY - scale][indexX + scale] +
				(double)pGray[indexY][indexX + scale] + (double)pGray[indexY + scale][indexX + scale] + (double)pGray[indexY + scale][indexX] +
				(double)pGray[indexY + scale][indexX - scale] + (double)pGray[indexY][indexX - scale] + (double)pGray[y + scale][x + scale] ) / 9.0);			//(double)pGray[y + scale][x + scale] --> central pix



				if (pixAvgInWin <= (int)pGray[indexY - scale][indexX - scale]){		// pos [0,0]
					//cout << (int)pGray[indexY - scale][indexX - scale] << endl;
					binConv |= (1u << 0);
				}

				if (pixAvgInWin <= (int)pGray[indexY - scale][indexX]){				// pos [0,1]
					//cout << (int)pGray[indexY - scale][indexX]<< endl;
					binConv |= (1u << 1);
				}

				if (pixAvgInWin <= (int)pGray[indexY - scale][indexX + scale]){		// pos [0,2]
					//cout << (int)pGray[indexY - scale][indexX + scale] << endl;
					binConv |= (1u << 2);
				}

				if (pixAvgInWin <= (int)pGray[indexY][indexX + scale]){				// pos [1,2]
					//cout << (int)pGray[indexY][indexX + scale] << endl;
					binConv |= (1u << 3);
				}

				if (pixAvgInWin <= (int)pGray[indexY + scale][indexX + scale]){		// pos [2,2]
					//cout << (int)pGray[indexY + scale][indexX + scale] << endl;
					binConv |= (1u << 4);
				}

				if (pixAvgInWin <= (int)pGray[indexY + scale][indexX]){				// pos [2,1]
					//cout << (int)pGray[indexY + scale][indexX] << endl;
					binConv |= (1u << 5);
				}

				if (pixAvgInWin <= (int)pGray[indexY + scale][indexX - scale]){		// pos [2,0]
					//cout << (int)pGray[indexY + scale][indexX - scale] << endl;
					binConv |= (1u << 6);
				}

				if (pixAvgInWin <= (int)pGray[indexY][indexX - scale]){				// pos [1,0]
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

			cvSaveImage("savedExNiLbpImgeS1.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedExNiLbpImgeS1.bmp", imgSave))
				cout << "Failed to save NiLBPimageS1" << endl << endl;
			else
				cout<<"Successfully save NiLBPimageS1" << endl << endl;

		}
		else if (scale == 2){

		cvSaveImage("savedExNiLbpImgeS2.bmp", imgSave);
		//check save image status
		if (!cvSaveImage("savedExNiLbpImgeS2.bmp", imgSave))
			cout << "Failed to save NiLBPimageS2" << endl << endl;
		else
			cout<<"Successfully save NiLBPimageS2" << endl << endl;

		}
		else {

			cvSaveImage("savedExNiLbpImgeS3.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedExNiLbpImgeS3.bmp", imgSave))
				cout << "Failed to save NiLBPimageS3" << endl << endl;
			else
				cout<<"Successfully save NiLBPimageS3" << endl << endl;

		}

		//clean memory
		delete [] pLbpImg;
		cvReleaseImage(&imgSave);
		indexX = 0, indexY = 0, binConv = 0;

	}

}


void extended_Lbp_ci(u_int8_t **pGray, int rows, int cols, int *histScale1, int *histScale2, int *histScale3){

	int scale;
	int centerPix = 0, indexX = 0, indexY = 0, binConv = 0;
	double pixAvgInWin = 0.0;

	//average of pGray Image
	long double sumV = 0;
	double meanValueX;
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			sumV = (int) pGray[y][x] + sumV;
			//cout << (int) pGray[y][x] << " ";		// Print Image data (comment this line if not necessary)
		}

	}
	meanValueX = sumV/(rows * cols);

	//run through 3 scale
	for(scale = 1; scale < 4; scale++) {

		cout << "LBP operation is running in Scale " << scale << " ... " << endl << endl;

		//create a scale sized IplImage for saving LBP image
		unsigned char **pLbpImg;
		pLbpImg = new unsigned char *[rows - (2 * scale)];

		for(int i = 0; i < (rows - (2 * scale)); i++){
			pLbpImg[i] = new unsigned char[cols - (2*scale)];
		}




		//CI- lbp running for scale 1
		for(int y = 0; y < rows - (2 * scale); y++){
			for(int x = 0; x < cols - (2*scale); x++){

				indexX = 0, indexY = 0, binConv = 0;

				centerPix = (int)pGray[y + scale][x + scale];
				//cout << centerPix << endl;			//// Print center pixel data (comment this line if not necessary)

				indexX = x + scale; 
				indexY = y + scale;


				if ((int)meanValueX <= centerPix){		// centerPix
					//cout << (int)pGray[indexY - scale][indexX - scale] << endl;
					binConv |= (1u << 0);
				}

				pLbpImg[y][x] = binConv * 255;			//saving LBP values using binarization
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

			cvSaveImage("savedExCiLbpImgeS1.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedExCiLbpImgeS1.bmp", imgSave))
				cout << "Failed to save ciLBPimageS1" << endl << endl;
			else
				cout<<"Successfully save ciLBPimageS1" << endl << endl;

		}
		else if (scale == 2){

		cvSaveImage("savedExCiLbpImgeS2.bmp", imgSave);
		//check save image status
		if (!cvSaveImage("savedExCiLbpImgeS2.bmp", imgSave))
			cout << "Failed to save ciLBPimageS2" << endl << endl;
		else
			cout<<"Successfully save ciLBPimageS2" << endl << endl;

		}
		else {

			cvSaveImage("savedExCiLbpImgeS3.bmp", imgSave);
			//check save image status
			if (!cvSaveImage("savedExCiLbpImgeS3.bmp", imgSave))
				cout << "Failed to save ciLBPimageS3" << endl << endl;
			else
				cout<<"Successfully save ciLBPimageS3" << endl << endl;

		}

		//clean memory
		delete [] pLbpImg;
		cvReleaseImage(&imgSave);
		indexX = 0, indexY = 0, binConv = 0;

	}

}


double round(double d)
{
  return floor(d + 0.5);
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
	int histScale1niLbp[binSize] = {0}, histScale2niLbp[binSize] = {0}, histScale3niLbp[binSize] = {0};
	int scale = 1;		//scale = 1
	double featureConCatNiLbp[binSize * 3] = {0};	//For concatinating three histogram feature

	//Perfrom NI_Lbp function 
	extended_Lbp_ni(pGray, rows, cols, histScale1niLbp, histScale2niLbp, histScale3niLbp);	

	//Concatinating features of scale 1 to 3
	for (int i = 0; i < binSize; i++){
		featureConCatNiLbp[i] = (double)histScale1niLbp[i];
		featureConCatNiLbp[i + (binSize * 1)] = (double)histScale2niLbp[i];
		featureConCatNiLbp[i + (binSize * 2)] = (double)histScale3niLbp[i];
	}


	//intialize histScale for saving histogram feature
	const int binSize1 = 2;
	int histScale1ciLbp[binSize1] = {0}, histScale2ciLbp[binSize1] = {0}, histScale3ciLbp[binSize1] = {0};
	scale = 1;		//scale = 1
	double featureConCatCiLbp[binSize1 * 3] = {0};	//For concatinating three histogram feature

	//Perfrom CI_Lbp function 
	extended_Lbp_ci(pGray, rows, cols, histScale1ciLbp, histScale2ciLbp, histScale3ciLbp);

	//Concatinating features of scale 1 to 3
	for (int i = 0; i < binSize1; i++){
		featureConCatCiLbp[i] = (double)histScale1ciLbp[i];
		featureConCatCiLbp[i + (binSize1 * 1)] = (double)histScale2ciLbp[i];
		featureConCatCiLbp[i + (binSize1 * 2)] = (double)histScale3ciLbp[i];
	}



	
	cin.get();		// press enter to finish the execution
	return 0;

}