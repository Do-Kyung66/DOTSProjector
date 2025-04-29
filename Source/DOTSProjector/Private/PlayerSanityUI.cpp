// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSanityUI.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "PhasmophobiaPlayer.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Components/VerticalBox.h"
#include "Net/UnrealNetwork.h"


void UPlayerSanityUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    playerArr = GetWorld()->GetGameState()->PlayerArray;

    name.Empty();

    for (APlayerState* pState : playerArr)
    {
		if (APhasmophobiaPlayer* PlayerPawn = Cast<APhasmophobiaPlayer>(pState->GetPawn()))
		{
			int32 PlayerSanity = PlayerPawn->Sanity;
			name.Append(FString::Printf(TEXT("%s : %d\n"), *pState->GetPlayerName(), PlayerSanity));
		}
    }

    Text_SanityNum->SetText(FText::FromString(name));
}

//void UPlayerSanityUI::Multicast_UpdateSanityList_Implementation(const TArray<FString>& SanityList)
//{
//    UpdateSanityList(SanityList);
//}
//
//void UPlayerSanityUI::RequestSanity()
//{
//    Server_RequestSanityList();
//}
//
//void UPlayerSanityUI::Server_RequestSanityList_Implementation()
//{
//    AGameStateBase* GS = GetWorld()->GetGameState();
//    TArray<FString> SanityDisplayList;
//
//    for (APlayerState* PS : GS->PlayerArray)
//    {
//        if (AController* PC = Cast<AController>(PS->GetOwner()))
//        {
//            if (APhasmophobiaPlayer* PlayerPawn = Cast<APhasmophobiaPlayer>(PC->GetPawn()))
//            {
//                int32 PlayerSanity = PlayerPawn->Sanity;
//                FString SanityString = FString::Printf(TEXT("%s - Sanity: %d"), *PS->GetPlayerName(), PlayerSanity);
//                SanityDisplayList.Add(SanityString);
//            }
//        }
//    }
//
//    Multicast_UpdateSanityList(SanityDisplayList);
//}
//
//void UPlayerSanityUI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//    DOREPLIFETIME(UPlayerSanityUI, NumberOfPlayers);
//    DOREPLIFETIME(UPlayerSanityUI, name);
//    DOREPLIFETIME(UPlayerSanityUI, Text_SanityNum);
//    DOREPLIFETIME(UPlayerSanityUI, playerArr);
//    
//}

//void UPlayerSanityUI::UpdateSanityList(const TArray<FString>& SanityDisplayList)
//{
//	SanityVerticalBox->ClearChildren();
//
//	for (const FString& TextLine : SanityDisplayList)
//	{
//		UTextBlock* NewText = NewObject<UTextBlock>(this);
//		NewText->SetText(FText::FromString(TextLine));
//		NewText->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f)));
//		SanityVerticalBox->AddChildToVerticalBox(NewText);
//	}
//}
//
//
//void UPlayerSanityUI::GetPlayerSanity()
//{
//	AGameStateBase* GS = GetWorld()->GetGameState();
//
//    TArray<FString> SanityDisplayList;
//
//    for (APlayerState* PS : GS->PlayerArray)
//    {
//        if (AController* PC = Cast<AController>(PS->GetOwner()))
//        {
//            if (APhasmophobiaPlayer* PlayerPawn = Cast<APhasmophobiaPlayer>(PC->GetPawn()))
//            {
//                int32 PlayerSanity = PlayerPawn->Sanity;
//                FString SanityString = FString::Printf(TEXT("%s - Sanity: %d"), *PS->GetPlayerName(), PlayerSanity);
//                SanityDisplayList.Add(SanityString);
//            }
//        }
//    }
//
//    UpdateSanityList(SanityDisplayList);
//}
//
