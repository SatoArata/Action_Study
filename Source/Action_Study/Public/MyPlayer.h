// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayer.generated.h"

UCLASS()
class ACTION_STUDY_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void CameraTurn(float Val);

	UFUNCTION()
		void CameraLookUp(float Val);


	UPROPERTY(EditAnywhere)
		USpringArmComponent* m_SpringArmComp;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* m_CameraComponent;

	//入力角にモデルの方向を設定
	void RotateToMatchDirection();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
