// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IPlayerBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Observer.h"
#include "Item_Base.h"
#include "Blueprint/UserWidget.h"
#include "IItemBehavior.h"
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

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* HandMesh;

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

	UPROPERTY(Replicated)
	TObjectPtr<UObject> CurrentEquipStrategy;

	UPROPERTY(Replicated)
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

	UPROPERTY(EditAnywhere, Category = "Input")

	class UInputAction* ItemAction;

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

	UPROPERTY(Replicated)
	AActor* ownedItem = nullptr;

	UPROPERTY(Replicated)
	AActor* currentItem = nullptr;

	UPROPERTY(Replicated)
	TArray<AActor*> ItemActors;

	UPROPERTY(Replicated)
	bool bHasItem = false;

	UPROPERTY(Replicated)
	int32 CurrentItemIndex = -1;

	UPROPERTY(Replicated)
	bool bIsDead = false;

	UPROPERTY(Replicated)
	bool bIsCursorOverItem = false;
	
	// Player Behavior Func
	void Move(const FInputActionValue& Value);
	void OnMoveReleased(const FInputActionValue& Value);

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
	void ActivateItem();

	TArray<IObserver*> Observers;

	UPROPERTY(Replicated)
	float Sanity = 100.0f;

	void CheckGhostOnScreen(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> CenterWidget;

	class UDefaultCursorWidget* CenterUI;

	class IItemBehavior* EquipStrategy;

	class IItemBehavior* DetachStrategy;

	UPROPERTY(Replicated)
	TScriptInterface<IItemBehavior> SwitchStrategy;

	UPROPERTY(Replicated)
	AActor* TargetItem = nullptr;

	void ItemTrace();


// Network
public:

	UPROPERTY(Replicated, BlueprintReadWrite)
	EItemType CurrentItemType = EItemType::None;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Server, Reliable)
	void ServerRPC_Equip();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Equip();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ItemTrace();

	UFUNCTION(Server, Reliable)
	void ServerRPC_UseItem();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_UseItem(AItem_Base* Item);

	UFUNCTION(Server, Reliable)
	void ServerRPC_Detach();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Detach();

	UFUNCTION(Server, Reliable)
	void ServerRPC_Switch(float ScrollData);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_Switch(int32 NextItemIndex);

	UPROPERTY(Replicated)
	float ScrollValue = 0.f;

	UPROPERTY(Replicated)
	int32 NextIndex = 0.f;

	UPROPERTY(Replicated)
	int32 StartIndex = 0.f;

	UFUNCTION(Server, Reliable)
	void ServerRPC_ItemAction();

	// sound
	FTimerHandle FootstepTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> FootstepSounds;

	int32 CurrentFootstepIndex = 0;


	bool bIsMoving = false;
	float Interval = 0.5f;


	void StartFootstepSound();
	void StopFootstepSound();
	void PlayFootstepSound();
};
