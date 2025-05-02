// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Base.h"
#include "Components/SpotLightComponent.h"
#include "Item_UVLight.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AItem_UVLight : public AItem_Base
{
	GENERATED_BODY()
	
public:
	AItem_UVLight();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)

	class USpotLightComponent* SpotLightComp;

};
