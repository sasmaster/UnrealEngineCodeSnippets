// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SIMLAB_API TrackBallController
{
	
	FVector ToScreenCoord(const float x,const float y);
	uint32_t mWinWidth;
	uint32_t mWinHeight;
	FVector  mCenter;
	FVector  mStartP;
	FVector  mCurrP;
	FVector  mRotAxis;
	float    mRotAngle;
public:
	TrackBallController();
	//can enhance this trackball with also setting target center for more precise
	// control of rotation :
	//https://github.com/iauns/cpm-arc-ball/blob/master/arc-ball/ArcBall.hpp
	explicit TrackBallController(uint32_t w, uint32_t h,const FVector& center = FVector(0.0f));
	void SetWindowsSize(uint32_t w, uint32_t h);

	void OnMouseDown(const float x, const  float y);
	void OnMouseDrag(const float x, const  float y);

	FQuat GetRotation()
	{

		FQuat q(mRotAxis, mRotAngle);
		// q.X *= -1;
		 q.Y *= -1;
		 q.Z *= -1;

		return q;
	}

	FQuat GetRotation(const FTransform& viewMatrix)
	{
		//currently not sure the inverse changes anything
		//as the viewMatrix is not camera but camera Actor transform
	//	FTransform viewInverse = viewMatrix.Inverse();
		 

	 	FVector rotAxisInview = viewMatrix.TransformVector(mRotAxis);
 
		FQuat qa(rotAxisInview, mRotAngle);
 

		qa.Y *= -1;
		qa.Z *= -1;
		 
		return qa;
	}

	~TrackBallController();
};
