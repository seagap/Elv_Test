#include "ihcFloodFillFun.h"
#include <cv.h>
#include <highgui.h>


void ihcFloodFill(IplImage* inputImage,IhcFloodFillSeg* pstSegment,IhcContourInfo* pstCountour,char UpDownFlag,IplImage* outputImage){

	unsigned char *pcImageData,*pcImageDataLeft,*pcImageDataRight;
	short	syIndex;
	short	sLeftLeft,sLeftRight;
	short	sRightLeft,sRightRight;

	char	cBreakFlag = 0;

	syIndex = pstSegment->syIndex + UpDownFlag;

	if(syIndex <= 0 || syIndex >= 720)
		return;

	pcImageData = (unsigned char*)inputImage->imageData + syIndex * inputImage->widthStep + pstSegment->sxParentLeft;

	pcImageDataLeft		= pcImageData;
	pcImageDataRight	= pcImageData;
	sLeftLeft			= pstSegment->sxParentLeft;
	sLeftRight			= pstSegment->sxParentLeft;

	sRightLeft			= pstSegment->sxParentRight;
	sRightRight			= pstSegment->sxParentRight;

	while(pcImageData != NULL && !cBreakFlag){

		cBreakFlag = 1;

		if(*pcImageDataLeft > pstSegment->ucThreshold && sLeftLeft > 0){
			pcImageDataLeft--;
			sLeftLeft--;
			cBreakFlag = 0;
		}
		if(*pcImageDataRight > pstSegment->ucThreshold && sLeftRight < 1280){
			pcImageDataRight++;
			sLeftRight++;
			cBreakFlag = 0;
		}
		else{
			if(sLeftLeft == sLeftRight && sLeftRight < sRightLeft){
				pcImageDataRight++;
				sLeftLeft++;
				sLeftRight++;
				cBreakFlag = 0;
			}

		}
	}
	cBreakFlag = 0;
	if(sLeftRight < pstSegment->sxParentRight){

		pcImageData = (unsigned char*)inputImage->imageData + syIndex * inputImage->widthStep + pstSegment->sxParentRight;

		pcImageDataLeft		= pcImageData;
		pcImageDataRight	= pcImageData;

		while(pcImageData != NULL && !cBreakFlag){
			cBreakFlag = 1;
			if(*pcImageDataLeft > pstSegment->ucThreshold && sRightLeft > 0){
				pcImageDataLeft--;
				sRightLeft--;
				cBreakFlag = 0;
			}
			if(*pcImageDataRight > pstSegment->ucThreshold && sRightRight < 1280){
				pcImageDataRight++;
				sRightRight++;
				cBreakFlag = 0;
			}
			if(sRightLeft == sRightLeft && sRightLeft > sLeftRight){
				sRightLeft--;
				sRightRight--;
				pcImageDataLeft--;
				cBreakFlag = 0;
			}

		}
		//for showImage
		for(int i = sLeftLeft;i < sLeftRight;i++){
			outputImage->imageData[syIndex*outputImage->widthStep+i] = 255;
		}
		for(int i = sRightLeft;i < sRightRight;i++){
			outputImage->imageData[syIndex*outputImage->widthStep+i] = 255;
		}
	}
	else{
		//for showImage
		for(int i = sLeftLeft;i < sLeftRight;i++){
			outputImage->imageData[syIndex*outputImage->widthStep+i] = 255;
		}
		sRightRight = sLeftRight;
	}

	if(sLeftLeft == sLeftRight && sRightLeft == sRightRight)
		return;
	else{
		pstSegment->sxParentLeft = sLeftLeft;
		pstSegment->sxParentRight = sRightRight;
		pstSegment->syIndex += UpDownFlag;
		ihcFloodFill(inputImage,pstSegment,pstCountour,UpDownFlag,outputImage);
	}

}