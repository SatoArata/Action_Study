// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	check(m_SpringArmComp != nullptr);

	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(m_CameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	m_SpringArmComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	m_SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	m_SpringArmComp->TargetArmLength = 400.f;
	m_SpringArmComp->bEnableCameraLag = true;
	m_SpringArmComp->CameraLagSpeed = 3.0f;

	m_CameraComponent->SetupAttachment(m_SpringArmComp, USpringArmComponent::SocketName);

	m_SpringArmComp->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));


}

void AMyPlayer::MoveForward(float Value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	direction.Z = 0.0f; // zŽ²¬•ª‚ð0‚ÉÝ’è
	AddMovementInput(direction.GetSafeNormal(), Value);
}

void AMyPlayer::MoveRight(float Value)
{
	FVector direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	direction.Z = 0.0f; // zŽ²¬•ª‚ð0‚ÉÝ’è
	AddMovementInput(direction.GetSafeNormal(), Value);
}

void AMyPlayer::CameraTurn(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		/*APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Val);*/

		m_SpringArmComp->AddRelativeRotation(FRotator(0, Val, 0));
	}
}

void AMyPlayer::CameraLookUp(float Val)
{
	if (Val != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		m_SpringArmComp->AddRelativeRotation(FRotator(Val, 0, 0));

		const float LOOK_UP_MAX = 35.f;
		const float LOOK_UP_MIN = -50.f;

		FRotator rot = m_SpringArmComp->GetRelativeRotation();
		if (rot.Pitch <= LOOK_UP_MIN)m_SpringArmComp->SetRelativeRotation(FRotator(LOOK_UP_MIN, rot.Yaw, rot.Roll));
		if (rot.Pitch >= LOOK_UP_MAX)m_SpringArmComp->SetRelativeRotation(FRotator(LOOK_UP_MAX, rot.Yaw, rot.Roll));
	}
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::CameraTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::CameraLookUp);

}

