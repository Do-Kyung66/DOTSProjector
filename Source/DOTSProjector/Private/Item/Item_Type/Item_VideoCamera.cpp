// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_VideoCamera.h"
#include "Function_VideoCamera.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetRenderingLibrary.h"

AItem_VideoCamera::AItem_VideoCamera()
{
	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("VideoCamera", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);

	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComponent"));
	CaptureComponent->SetupAttachment(MeshComp);

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RT(TEXT("/Game/UP/Item/ItemTool/RT_VideoCamera.RT_VideoCamera"));
	if (RT.Succeeded())
	{
		RenderTarget = RT.Object;
		CaptureComponent->TextureTarget = RenderTarget;
	}

}

void AItem_VideoCamera::BeginPlay()
{
	Super::BeginPlay();

	CaptureComponent->SetVisibility(bIsOn);
	CaptureComponent->bCaptureEveryFrame = bIsOn;
	ItemStrategy = NewObject<UFunction_VideoCamera>(this);
}

void AItem_VideoCamera::OnRep_VideoState()
{
	if (!CaptureComponent) return;

	CaptureComponent->SetVisibility(bIsOn);
	CaptureComponent->bCaptureEveryFrame = bIsOn;

}

void AItem_VideoCamera::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem_VideoCamera, bIsOn);
}

