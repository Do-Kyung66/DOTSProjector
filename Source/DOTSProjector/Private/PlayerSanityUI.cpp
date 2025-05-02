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

    playerArr.Sort([](const APlayerState& A, const APlayerState& B)
        {
            auto ExtractNumber = [](const FString& Name) -> int32
                {
                    FString NumStr;
                    for (int32 i = 0; i < Name.Len(); ++i)
                    {
                        if (FChar::IsDigit(Name[i]))
                        {
                            NumStr += Name[i];
                        }
                    }
                    return FCString::Atoi(*NumStr);
                };

            return ExtractNumber(A.GetPlayerName()) < ExtractNumber(B.GetPlayerName());
        });

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

