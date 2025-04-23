// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Evidence_Ghost.generated.h"

UCLASS()
class DOTSPROJECTOR_API AEvidence_Ghost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEvidence_Ghost();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UMaterialInstanceDynamic* DynamicMat;

	void RevealWithUV();

	void InvisibleMaterial();

	bool bIsVisible = false;

	bool IsRevealing = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GlowIntensity = 0.0f;

	UPROPERTY(EditAnywhere)
	float GlowInterpSpeed = 3.0f;
};
