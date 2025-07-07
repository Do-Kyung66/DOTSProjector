// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_UVLight.h"
#include "Item_UVLight.h"
#include "Evidence_Ghost.h"
#include "Engine/OverlapResult.h"
#include "EngineUtils.h"


void UFunction_UVLight::Use(AItem_Base* Item)
{
	if (AItem_UVLight* UVLight = Cast<AItem_UVLight>(Item)) {
        if (UVLight->SpotLightComp)
        {
            bOn = !UVLight->SpotLightComp->IsVisible();

            if (bOn) {
                UVLight->SpotLightComp->SetVisibility(bOn);
            }
            else {
                UVLight->SpotLightComp->SetVisibility(bOn);
            }
        }
	}
}

void UFunction_UVLight::Tick(float DeltaTime, AItem_UVLight* UVLight)
{
    for (TActorIterator<AEvidence_Ghost> It(GetWorld()); It; ++It)
    {
        It->bIsVisible = false;
    }

    if (!bOn) return;

    if (UVLight->SpotLightComp && UVLight->SpotLightComp->IsVisible())
    {
        FVector Start = UVLight->SpotLightComp->GetComponentLocation();
        FVector Forward = UVLight->SpotLightComp->GetForwardVector();
        FVector Right = UVLight->SpotLightComp->GetRightVector();
        FVector Up = UVLight->SpotLightComp->GetUpVector();

        TArray<FVector> Directions;
        Directions.Add(Forward);
        Directions.Add((Forward + Right * 0.15f).GetSafeNormal());
        Directions.Add((Forward - Right * 0.15f).GetSafeNormal());
        Directions.Add((Forward + Up * 0.15f).GetSafeNormal());
        Directions.Add((Forward - Up * 0.15f).GetSafeNormal());

        for (const FVector& Dir : Directions)
        {
            FVector End = Start + Dir * 600.f;

            FCollisionQueryParams Params;
            Params.AddIgnoredActor(UVLight);
            FHitResult Hit;

            bool bHit = GetWorld()->LineTraceSingleByChannel(
                Hit,
                Start,
                End,
                ECC_Pawn,
                Params
            );

            if (bHit)
            {
                FVector HitLocation = Hit.Location;

                TArray<FOverlapResult> Overlaps;
                FCollisionShape Sphere = FCollisionShape::MakeSphere(35.0f);
                bool bSphereHit = GetWorld()->OverlapMultiByChannel(
                    Overlaps,
                    HitLocation,
                    FQuat::Identity,
                    ECC_Pawn, 
                    Sphere,
                    Params
                );

                if (bSphereHit)
                {
                    for (const FOverlapResult& Overlap : Overlaps)
                    {
                        if (AEvidence_Ghost* Evidence = Cast<AEvidence_Ghost>(Overlap.GetActor()))
                        {
                            Evidence->bIsVisible = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}



