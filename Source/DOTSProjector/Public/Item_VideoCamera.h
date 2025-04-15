// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Base.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Item_VideoCamera.generated.h"

/**
 * 
 */
UCLASS()
class DOTSPROJECTOR_API AItem_VideoCamera : public AItem_Base
{
	GENERATED_BODY()
	
public:
	AItem_VideoCamera();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D* CaptureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* RenderTarget;
};
