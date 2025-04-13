// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStrategy.h"
#include "DT_Item.h"
#include "Item_Base.generated.h"

UCLASS()
class DOTSPROJECTOR_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Base();

	void SetUsageStrategy(UItemStrategy* NewStrategy);

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Item")
	UItemStrategy* UsageStrategy;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	TArray<FName> RowNames;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	int32 ItemID;

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;

};
