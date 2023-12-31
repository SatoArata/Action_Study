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
	if (Value == 0.f)return;

	FVector direction = FRotationMatrix(m_SpringArmComp->GetComponentRotation()).GetScaledAxis(EAxis::X);
	direction.Z = 0.0f; // z軸成分を0に設定
	AddMovementInput(direction.GetSafeNormal(), Value);
}

void AMyPlayer::MoveRight(float Value)
{
	if (Value == 0.f)return;

	FVector direction = FRotationMatrix(m_SpringArmComp->GetComponentRotation()).GetScaledAxis(EAxis::Y);
	direction.Z = 0.0f; // z軸成分を0に設定
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

void AMyPlayer::RotateToMatchDirection()
{
	if (InputComponent->GetAxisValue("MoveForward") == 0.f &&
		InputComponent->GetAxisValue("MoveRight") == 0.f)
		return;

	// 目標の回転（カメラの回転）を取得
	// メッシュの位置とカメラの位置を取得
	FVector MeshLocation = GetMesh()->GetComponentLocation();
	FVector CameraLocation = m_CameraComponent->GetComponentLocation();

	// メッシュからカメラへのベクトルを計算
	FVector MeshToCamera = CameraLocation - MeshLocation;
	MeshToCamera.Z = 0;
	MeshToCamera.Normalize(); // ベクトルを正規化

	//目標の角度を算出

	// 左スティックの水平方向の入力を取得
	FVector2D LeftStickInput = FVector2D(InputComponent->GetAxisValue("MoveForward"), InputComponent->GetAxisValue("MoveRight"));
	// 入力ベクトルを角度に変換
	float InputAngle = FMath::Atan2(LeftStickInput.Y, LeftStickInput.X) * (180.0f / PI);

	FRotator TargetRotation = MeshToCamera.Rotation() + FRotator(0, 90.f + InputAngle, 0);

	// 現在の回転を取得
	FRotator CurrentRotation = GetMesh()->GetComponentRotation();

	// 補間率を計算（DeltaTimeはフレーム時間）
	const float INTERP_SPEED = 7.f;
	const float DELTA_TIME = GetWorld()->GetDeltaSeconds();

	// 補間を行い、メッシュの回転を更新
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DELTA_TIME, INTERP_SPEED);
	GetMesh()->SetWorldRotation(NewRotation);
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToMatchDirection();
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

