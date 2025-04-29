// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSanityUI.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API UPlayerSanityUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UFUNCTION(BlueprintCallable)
 //   void UpdateSanityList(const TArray<APlayerState*>& PlayerStates);

	//UPROPERTY(Replicated, meta = (BindWidget))
 //   class UVerticalBox* SanityVerticalBox;

	void GetPlayerSanity();

	void UpdateSanityList(const TArray<FString>& SanityDisplayList);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()

	TArray<class APhasmophobiaPlayer*> NumberOfPlayers;

	//UPROPERTY(meta = (BindWidget))
	//class  UTextBlock* Text_PlayerNum1;

	//UPROPERTY(meta = (BindWidget))
	//class  UTextBlock* Text_PlayerNum2;

	/*UPROPERTY(meta = (BindWidget))
	class  UTextBlock* Text_SanityNum1;*/

	UPROPERTY(meta = (BindWidget))
	class  UTextBlock* Text_SanityNum;

	UPROPERTY()
	FString name;

	UPROPERTY()
	TArray<APlayerState*> playerArr;

	/*UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateSanityList(const TArray<FString>& SanityList);

	void RequestSanity();

	UFUNCTION(Server, Reliable)
	void Server_RequestSanityList();
	*/
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
};
