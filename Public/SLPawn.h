// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrackBallController.h"
#include "UnrealClient.h"
#include "GameFramework/Pawn.h"
#include "SLPawn.generated.h"


UENUM(BlueprintType)
enum class  CamType:uint8
{
	FLY_CAM         UMETA(DisplayName = "Fly"),
	ARCBALL_CAM 	UMETA(DisplayName = "Arcball"),
	ORBIT_CAM    	UMETA(DisplayName = "Orbit")
	 
};


UCLASS()
class SIMLAB_API ASLPawn : public APawn
{
	GENERATED_BODY()

	AActor*             mClickedActor;
	TrackBallController mTrackBallController;
	class ASLPlayerController* mPlayerController;

	//Orbit cam settings:
	FVector mTargetCenter;

	float mAccumRotZ; //around Z (which is up axis)
	float mAccumRotY; //around Y (which is right axis)

	float mDistanceFromTarget;

	float mMaxDistanceFromTarget;

	float m_MinPitch;

	float  m_MaxPitch;

	float m_MinYaw;

	float m_MaxYaw;

	float m_ScrollAccel;

	////////////////////////

	bool mIsMouseDown;//needed for arcball interaction

	/**
	* Gets ray hit from mouse coordinates
	*
	*/
	void PickObjectOnMouseClick(class ASLPlayerController* playerController,const float mouseX, const float mouseY);

	void DoTrackballDrag(class ASLPlayerController* playerController);
public:
	// Sets default values for this pawn's properties
	ASLPawn();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	CamType mCamType;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* mCam;
	UPROPERTY()
	class UPawnMovementComponent* mMovementComponent;
	//UPROPERTY(EditAnywhere)
	//class USpringArmComponent* mSpringArm;
 

	//Any class can add a function that uses the delegate signature and responds to the Broadcast() event 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExplorerUIOpenSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExplorerUICloseSignature);



	UPROPERTY(BlueprintAssignable, Category = "SLPawn")
	FExplorerUIOpenSignature OnExplorerUIOpen;
	UPROPERTY(BlueprintAssignable, Category = "SLPawn")
	FExplorerUICloseSignature OnExplorerUIClose;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Begin Pawn overrides
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "SLPawn")
	void SetCameraType(const CamType camType);

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void OnMouseWheel(float value);
	void OnMousePressed();
	void OnMouseReleased();

	void OnViewportResize(FViewport* viewport, uint32 param);

	

};
