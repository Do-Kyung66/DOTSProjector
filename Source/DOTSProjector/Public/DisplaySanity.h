// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisplaySanity.generated.h"

UCLASS()
class DOTSPROJECTOR_API ADisplaySanity : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADisplaySanity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class UWidgetComponent* WidgetComponent;
};
