// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Base.h"
#include "Components/SphereComponent.h"
#include "Item_EMFReader.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AItem_EMFReader : public AItem_Base
{
	GENERATED_BODY()
	
public:
	AItem_EMFReader();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* DetectGhostSphere;

	UFUNCTION()
	void GhostInRange(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                       bool bFromSweep, const FHitResult& SweepResult);
};
