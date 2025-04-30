// Fill out your copyright notice in the Description page of Project Settings.


#include "Function_EMFReader.h"
#include "Item_EMFReader.h"
#include "GhostBase.h"
#include "Components/PointLightComponent.h"
#include "Net/UnrealNetwork.h"

void UFunction_EMFReader::Use(AItem_Base* Item)
{
	if (AItem_EMFReader* EMFReader = Cast<AItem_EMFReader>(Item)) {
		bOn = !bOn;

		if (bOn)
		{
			EMFReader->PointLight1->SetVisibility(true);
		}
		else {
			EMFReader->PointLight1->SetVisibility(false);
			EMFReader->PointLight2->SetVisibility(false);
			EMFReader->PointLight3->SetVisibility(false);
			EMFReader->PointLight4->SetVisibility(false);
			EMFReader->PointLight5->SetVisibility(false);
		}
	}
}


void UFunction_EMFReader::Tick(float DeltaTime, AItem_EMFReader* EMFReader)
{
	if (!bOn || !EMFReader || !EMFReader->HasAuthority()) return;

	//GEngine->AddOnScreenDebugMessage(25, 1.0f, FColor::Yellow, TEXT("EMFReader Tick Working"));
	//FVector Start = EMFReader->GetActorLocation();

	FVector Start = EMFReader->LEDComp->GetComponentLocation();

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

	bool bGhostDetected = false;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AGhostBase* Ghost = Cast<AGhostBase>(Hit.GetActor()))
			{
				if (Ghost->GhostData->Weakness.Find("EMFReader") && Ghost->GhostData->Weakness["EMFReader"] == true) {
					bGhostDetected = true;
					FVector GhostLocation = Ghost->GetActorLocation();
					//DrawDebugSphere(EMFReader->GetWorld(), GhostLocation, 25.f, 12, FColor::Red, false, 1.f);
				}
			}
		}
	}

	EMFReader->PointLight2->SetVisibility(bGhostDetected);
	EMFReader->PointLight3->SetVisibility(bGhostDetected);
	EMFReader->PointLight4->SetVisibility(bGhostDetected);
	EMFReader->PointLight5->SetVisibility(bGhostDetected);
	


	//DrawDebugSphere(EMFReader->GetWorld(), Start, EMFReader->DetectionRadius, 32, FColor::Green, false, 0.5f);
}

void UFunction_EMFReader::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFunction_EMFReader, bOn);
}
