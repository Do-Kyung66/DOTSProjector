// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_Thermometer.h"
#include "Item_Thermometer.h"
#include "GhostBase.h"
#include "Net/UnrealNetwork.h"

void UFunction_Thermometer::Use(AItem_Base* Item)
{
	if (AItem_Thermometer* Thermometer = Cast<AItem_Thermometer>(Item)) {
		GEngine->AddOnScreenDebugMessage(30, 1.f, FColor::Orange, FString::Printf(TEXT("Temperature %.2f"), Temperature));
	}
}

void UFunction_Thermometer::Tick(float DeltaTime, class AItem_Thermometer* Thermometer)
{
	FVector Start = Thermometer->MeshComp->GetComponentLocation();

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
					
				}
			}
		}
	}

	if (bGhostDetected && Temperature > 5) {
		Temperature -= 0.3;
	}
	else if (!bGhostDetected && Temperature < 20) {
		Temperature += 0.3;
	}

	// DrawDebugSphere(Thermometer->GetWorld(), Start, Thermometer->DetectionRadius, 32, FColor::Green, false, 0.5f);
}

void UFunction_Thermometer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFunction_Thermometer, Temperature);
}
