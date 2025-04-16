// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IPlayerBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Observer.h"
#include "Item_Base.h"
#include "Blueprint/UserWidget.h"
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
	UPROPERTY(VisibleAnywhere, Category = "Controller")
	class APhasmophobiaPlayerController* PC;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* CamComp;

	// Strategy var
	UPROPERTY()
	TObjectPtr<UObject> CurrentMoveStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentLookStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentCrouchStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentRunStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentEquipStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentSwitchStrategy;

	UPROPERTY()
	TObjectPtr<UObject> CurrentDetachStrategy;

	// InputAction var
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")

	class UInputAction* UseAction;

	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* ItemMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* EquipItemAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* SwitchItemAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* DetachItemAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* JournalAction;

	
	// Stamina Var
	bool bIsRunning = false;

	float WalkSpeed = 400.0f;
	float RunSpeed = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float MaxStamina = 50.0f; 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
    float CurrentStamina = 50.0f; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaDrainRate = 10.0f; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
    float StaminaRegenRate = 5.0f;

	// Item Var
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ItemComp;

	UPROPERTY(VisibleAnywhere)
	AActor* ownedItem = nullptr;
	AActor* currentItem = nullptr;

	UPROPERTY()
	TArray<AActor*> ItemActors;

	bool bHasItem = false;
	int32 CurrentItemIndex = -1;

	
	// Player Behavior Func
	void Move(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);

	void PlayerCrouch(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void OnRunReleased(const FInputActionValue& Value);

	// Item Behavior Func
	void Equip(const FInputActionValue& Value);
	void Switch(const FInputActionValue& Value);
	void Detach(const FInputActionValue& Value);

	void Journal(const FInputActionValue& Value);


	void SetMoveStrategy(TObjectPtr<UObject> NewMoveStrategy);

	void AddObserver(IObserver* Observer);
	void RemoveObserver(IObserver* Observer);
	void NotifySanityChanged();
	void DecreaseSanity(float Amount);

	void UseItem();

	TArray<IObserver*> Observers;

	float Sanity = 100.0f;

	void CheckGhostOnScreen(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> CenterWidget;

	UUserWidget* CenterUI;

};
