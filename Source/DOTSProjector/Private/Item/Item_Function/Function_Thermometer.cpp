// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_Thermometer.h"
#include "Item_Thermometer.h"
#include "GhostBase.h"

void UFunction_Thermometer::Use(AItem_Base* Item)
{
	/*if (AItem_Thermometer* Thermometer = Cast<AItem_Thermometer>(Item)) {

	}*/
}

void UFunction_Thermometer::Tick(float DeltaTime, class AItem_Thermometer* Thermometer)
{
	FVector Start = Thermometer->GetActorLocation();

	FVector End = Start;


	FCollisionShape Sphere = FCollisionShape::MakeSphere(Thermometer->DetectionRadius);
	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Thermometer);

	bool bHit = Thermometer->GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_WorldDynamic,
		Sphere,
		Params
	);

	bool bGhostDetected = false;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AGhostBase* Ghost = Cast<AGhostBase>(Hit.GetActor()))
			{
				if (Ghost->GhostData->Weakness.Find("Thermometer") && Ghost->GhostData->Weakness["Thermometer"] == true) {
					bGhostDetected = true;
					FVector GhostLocation = Ghost->GetActorLocation();
				}
			}
		}
	}

	DrawDebugSphere(Thermometer->GetWorld(), Start, Thermometer->DetectionRadius, 32, FColor::Green, false, 0.5f);
}
