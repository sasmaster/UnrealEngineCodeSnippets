// Fill out your copyright notice in the Description page of Project Settings.

#include "SLPawn.h"

#include "Components/InputComponent.h"
#include "SLPlayerController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameEngine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Runtime/Engine/Public/SceneView.h"
 
// Sets default values
ASLPawn::ASLPawn()
	: mClickedActor(nullptr)
	, mTargetCenter(0.0f)
	, mAccumRotZ(0.0f)
	, mAccumRotY(0.0f)
	, mDistanceFromTarget(200.0f)
	, m_MinPitch(-50.0f)
	, m_MaxPitch(50.0f)
	, m_MinYaw(-359.9)
	, m_MaxYaw(359.9)
	, m_ScrollAccel(10.0f)
	, mIsMouseDown(false)
	, mCamType(CamType::ORBIT_CAM)
	
	
{
	 
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mMovementComponent = CreateDefaultSubobject<UPawnMovementComponent,UFloatingPawnMovement>("SL_movement");
 	mCam = CreateDefaultSubobject<UCameraComponent>("cam1");

	/*mSpringArm = CreateDefaultSubobject<USpringArmComponent>("sarm1");
	mSpringArm->TargetArmLength = 400.f;
	mSpringArm->bEnableCameraLag = true;
	mSpringArm->CameraLagSpeed = 3.0f;*/
 
	//mCam->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
	
	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw   = true;
	bUseControllerRotationRoll  = false;

 
}

void ASLPawn::OnViewportResize(FViewport* viewport,uint32 param)
{
	UE_LOG(LogTemp, Warning, TEXT("New Viewport size:%i , %i\n"), viewport->GetSizeXY().X, viewport->GetSizeXY().Y);
}

// Called when the game starts or when spawned
void ASLPawn::BeginPlay()
{

	mPlayerController = Cast<ASLPlayerController>(GetController());
	mPlayerController->bShowMouseCursor = true;
	
	mCamType = CamType::ORBIT_CAM;
	if (mCamType == CamType::ARCBALL_CAM)
	{
		FVector2D viewportSize;
		GEngine->GameViewport->GetViewportSize(viewportSize);
		mTrackBallController.SetWindowsSize((uint32_t)viewportSize.X, (uint32_t)viewportSize.Y);
	}
	
	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &ASLPawn::OnViewportResize);

	Super::BeginPlay();
	
}

void ASLPawn::SetCameraType(const CamType camType)
{
	mCamType = camType;
}

// Called every frame
void ASLPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Controller)
	{
		printf("no controller\n");
	}
	 
 
	//UE_LOG(LogTemp, Warning, TEXT("Mouse screen coords:%f,%f"), mX1, mY1); 
	if (mCamType == CamType::ARCBALL_CAM)
	{
		DoTrackballDrag(mPlayerController);
	}
	else if (mCamType == CamType::ORBIT_CAM)
	{
		const float angleAroundZ = FMath::DegreesToRadians(FMath::Clamp(mAccumRotZ, m_MinYaw, m_MaxYaw));
		const float angleAroundY = FMath::DegreesToRadians(FMath::Clamp(mAccumRotY, m_MinPitch, m_MaxPitch));
		FQuat qYaw = FQuat(FVector(0, 0, 1), angleAroundZ);
		FQuat qPitch = FQuat(FVector(0, 1, 0), angleAroundY);
		FVector camOffset(mDistanceFromTarget, 0.0f, 0.0f);
		FVector camPos = mTargetCenter - (qYaw * qPitch * camOffset);

		auto rotLookAt = FRotationMatrix::MakeFromX(mTargetCenter - camPos).ToQuat();
		SetActorLocation(camPos);
		SetActorRotation(rotLookAt);
	
	}

	
}





// Called to bind functionality to input
void ASLPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASLPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASLPawn::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
 	PlayerInputComponent->BindAxis("Turn", this, &ASLPawn::Turn);
	//PlayerInputComponent->BindAxis("TurnRate", this, &APhysicsArenaGameCharacter::TurnAtRate);
 	PlayerInputComponent->BindAxis("LookUp", this, &ASLPawn::LookUp);
	PlayerInputComponent->BindAxis("MouseWheel", this, &ASLPawn::OnMouseWheel);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &APhysicsArenaGameCharacter::LookUpAtRate);


	//PlayerInputComponent->BindAxis("Turn", this, &ASLPawn::Turn);
	//PlayerInputComponent->BindAxis("TurnRate", this, &APhysicsArenaGameCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &ASLPawn::LookUp);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASLPawn::OnMousePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASLPawn::OnMouseReleased);
	
	
}

UPawnMovementComponent* ASLPawn::GetMovementComponent() const
{
	return mMovementComponent;
}


void ASLPawn::MoveForward(float value)
{
	if (mCamType == CamType::FLY_CAM)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
	
}
void ASLPawn::MoveRight(float value)
{
	if (mCamType == CamType::FLY_CAM)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
	 
}

void ASLPawn::Turn(float value)
{
	if (mCamType == CamType::FLY_CAM)
	{
		AddControllerYawInput(value);
	}

	mAccumRotZ += value;
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Turning %f\n"), value));
	//}
	
}
void ASLPawn::LookUp(float value)
{
	if (mCamType == CamType::FLY_CAM)
	{
		AddControllerPitchInput(value);
	}
	mAccumRotY += value;
}

void ASLPawn::OnMouseWheel(float value)
{
	mDistanceFromTarget += value * m_ScrollAccel;
	mDistanceFromTarget = FMath::Clamp(mDistanceFromTarget, 20.0f, 1000.0f);
	UE_LOG(LogTemp, Warning, TEXT("On mouse wheel:%f\n"), value);
}

void ASLPawn::DoTrackballDrag(ASLPlayerController* playerController)
{
	if (mIsMouseDown)
	{

		//PlayerController->bShowMouseCursor = true;
		if (playerController && mClickedActor)
		{
			// Get the coordinates of the mouse from our controller  
			float mX;
			float mY;

			playerController->GetMousePosition(mX, mY);



			mTrackBallController.OnMouseDrag(mX, mY);
			//mClickedActor->AddActorWorldRotation(mTrackBallController.GetRotation(), false, nullptr);
			//GetActorTransform
			const FTransform viewMatr = GetActorTransform();// .Inverse();// GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform();

			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetTransform();

			FQuat rot = mTrackBallController.GetRotation(viewMatr);
			FTransform rotationMatrix(rot);
			FTransform targetTransform = mClickedActor->GetTransform();

			mClickedActor->SetActorRelativeTransform( targetTransform * rotationMatrix);

		}
	}
}

void ASLPawn::PickObjectOnMouseClick(class ASLPlayerController* playerController,const float mouseX,const float mouseY)
{
 
		// Do a trace and see if there the position intersects something in the world  
		FVector2D MousePosition(mouseX, mouseY);
		FHitResult HitResult;
		const bool bTraceComplex = false;
		if (playerController->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, bTraceComplex, HitResult) == true)
		{
			// If the actor we intersected with is a controller posses it  
			AActor* hitACtor = HitResult.GetActor();
			UE_LOG(LogTemp, Warning, TEXT("Hit actor name is:%s\n"), *hitACtor->GetName());
			if (hitACtor && hitACtor->GetName() == "SphereTest")
			{
				mClickedActor = hitACtor;

				OnExplorerUIOpen.Broadcast();
			}
			else
			{
				mClickedActor = nullptr;
			}

			if (mClickedActor != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Clicked Actor's name:%s\n"), *mClickedActor->GetName());
			}
		}

	//}
}

void ASLPawn::OnMousePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("On Mouse Pressed triggered"));
	mIsMouseDown = true;
	if (mCamType == CamType::FLY_CAM)
	{

	}
	else 	
	{
		 
		// Get the coordinates of the mouse from our controller  
		float mX;
		float mY;
		mPlayerController->GetMousePosition(mX, mY);
		this->PickObjectOnMouseClick(mPlayerController, mX, mY);
		if (mCamType == CamType::ARCBALL_CAM)
		{


			mTrackBallController.OnMouseDown(mX, mY);
		}

	
	}

	
}

void ASLPawn::OnMouseReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("On Mouse Released triggered"));
	mIsMouseDown = false;
}



