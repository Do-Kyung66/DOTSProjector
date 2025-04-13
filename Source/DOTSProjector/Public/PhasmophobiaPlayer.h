// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IPlayerBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Observer.h"
#include "PhasmophobiaPlayer.generated.h"

UCLASS()
class DOTSPROJECTOR_API APhasmophobiaPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APhasmophobiaPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// variable
	// Player Behavior
	UPROPERTY()
	TObjectPtr<UObject> CurrentMoveStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentLookStrategy;

	// Input Action
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* UseAction;
	
	
	// function
	// Player Behavior Input Action connet
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);


	void SetMoveStrategy(TObjectPtr<UObject> NewMoveStrategy);

	void AddObserver(IObserver* Observer);
	void RemoveObserver(IObserver* Observer);
	void NotifySanityChanged();
	void DecreaseSanity(float Amount);

	void UseItem();

	TArray<IObserver*> Observers;

	float Sanity = 100.0f;
};
