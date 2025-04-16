// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_EMFReader.h"
#include "Item_EMFReader.h"
#include "GhostBase.h"

void UFunction_EMFReader::Use(AItem_Base* Item)
{
	if (AItem_EMFReader* EMFReader = Cast<AItem_EMFReader>(Item)) {
		bOn = !bOn;
	}
}

void UFunction_EMFReader::Tick(float DeltaTime, AItem_EMFReader* EMFReader)
{
	if (!bOn || !EMFReader) return;

	FVector Start = EMFReader->GetActorLocation();
	FVector End = Start;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(EMFReader->DetectionRadius);
	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(EMFReader);

	bool bHit = EMFReader->GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_WorldDynamic,
		Sphere,
		Params
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AGhostBase* Ghost = Cast<AGhostBase>(Hit.GetActor()))
			{
				FVector GhostLocation = Ghost->GetActorLocation();
				DrawDebugSphere(EMFReader->GetWorld(), GhostLocation, 25.f, 12, FColor::Red, false, 1.f);
			}
		}
	}

	DrawDebugSphere(EMFReader->GetWorld(), Start, EMFReader->DetectionRadius, 32, FColor::Green, false, 0.5f);
}
