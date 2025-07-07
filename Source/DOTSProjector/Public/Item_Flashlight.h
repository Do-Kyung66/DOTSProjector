// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStrategy.h"
#include "Item_Base.h"
#include "Components/SpotLightComponent.h"
#include "Item_Flashlight.generated.h"

UCLASS()
class DOTSPROJECTOR_API AItem_Flashlight : public AItem_Base
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Flashlight();

	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = "Flashlight")
    USpotLightComponent* SpotLightComp;

	UPROPERTY(ReplicatedUsing = OnRep_LightState)
    bool bIsOn;

    UFUNCTION()
    void OnRep_LightState();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
