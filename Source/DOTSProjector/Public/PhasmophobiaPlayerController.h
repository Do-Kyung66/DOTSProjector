// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PhasmophobiaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API APhasmophobiaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APhasmophobiaPlayerController();


	UPROPERTY(Replicated)
	AActor* TargetItem = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetCursorForInteraction(bool bIsInteractable, AActor* tempItem);



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	bool bCanInteract = false;

	// door
	AActor* TargetDoor = nullptr;
	bool bDraggingDoor = false;
	float LastMousX = 0.f;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;



};
