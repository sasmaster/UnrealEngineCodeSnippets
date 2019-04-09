// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackBallController.h"

//http://rainwarrior.ca/dragon/arcball.html
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
//https://github.com/subokita/Arcball/blob/master/Arcball/Arcball.cpp
//https://github.com/devernay/glm/blob/master/examples/trackball.c
//https://github.com/iauns/cpm-arc-ball/blob/master/tests/TestRotations.cpp
//http://web.cse.ohio-state.edu/~shen.94/781/Site/Slides_files/trackball.pdf

TrackBallController::TrackBallController()
	:mWinWidth(0),mWinHeight(0), mRotAngle(0.0f)
{
}

TrackBallController::TrackBallController(uint32_t w, uint32_t h, const FVector& center)
{
	SetWindowsSize(w, h);
}

void TrackBallController::SetWindowsSize(uint32_t w, uint32_t h)
{
	mWinWidth = w;
	mWinHeight = h;
}

/**
 * Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
 * into normalized screen coordinate (i.e. (-1, -1) to (1, 1)
 */
FVector TrackBallController::ToScreenCoord(const float x,const  float y) {
	//in Unreal:
	// X - forward
	// Y - right
	// Z - up
	
	check(mWinWidth);
	check(mWinHeight);

	FVector coord(0.0f);
	
	coord.Y =      (2 * x - mWinWidth)  / mWinWidth; 
	coord.Z =     -(2 * y - mWinHeight) / mWinHeight;
	 
	/* Clamp it to border of the windows, comment these codes to allow rotation when cursor is not over window */
	coord.Y = FMath::Clamp(coord.Y, -1.0f, 1.0f);
	coord.Z = FMath::Clamp(coord.Z, -1.0f, 1.0f);

	//UE_LOG(LogTemp, Warning, TEXT("Mouse screen coords:%f,%f"), coord.Y, coord.Z);

	float length_squared = coord.Y * coord.Y + coord.Z * coord.Z;
	if (length_squared <= 1.0)
	{
		coord.X =   FMath::Sqrt(1.0f - length_squared);
	}
	else
	{
		coord.Normalize();
		//coord = glm::normalize(coord);
	}
	return coord;
}

void TrackBallController::OnMouseDown(const float x, const  float y)
{
	mStartP = ToScreenCoord(x, y);
}

void TrackBallController::OnMouseDrag(const float x, const  float y)
{
#if 0
		if (mouseEvent == 0)
			return;
		else if (mouseEvent == 1) {
			/* Start of trackball, remember the first position */
			prevPos = toScreenCoord(x, y);
			mouseEvent = 2;
			return;
		}
#endif
		// Tracking the subsequent 
		mCurrP = ToScreenCoord(x, y);

		
		/* Calculate the angle in radians, and clamp it between 0 and 90 degrees */
		mRotAngle =  FMath::Acos(FMath::Min(1.0f, FVector::DotProduct(mStartP, mCurrP)));
		//angle = acos(std::min(1.0f, glm::dot(prevPos, currPos)));

		/* Cross product to get the rotation axis, but it's still in camera coordinate */
		mRotAxis = FVector::CrossProduct(mStartP, mCurrP);
		//camAxis = glm::cross(prevPos, currPos);
	
}

TrackBallController::~TrackBallController()
{
}
