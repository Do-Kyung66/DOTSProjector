// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayer.h"
#include "MoveBehavior.h"
#include "LookBehavior.h"
#include "CrouchBehavior.h"
#include "RunBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"


// Sets default values
APhasmophobiaPlayer::APhasmophobiaPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 메시 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Player/Assets/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/rp_manuel_rigged_001_ue4.rp_manuel_rigged_001_ue4'"));

	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -88.0), FRotator(0.0, -90, 0.0));
	}

	// input 고정 시켜두기
	
}

// Called when the game starts or when spawned
void APhasmophobiaPlayer::BeginPlay()
{
	Super::BeginPlay();

	// input mapping
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0); // 우선순위 0

		}
	}

	// 런타임에서만 사용
	// 기본 이동 전략 설정
	CurrentMoveStrategy = NewObject<UMoveBehavior>(this); 
	CurrentLookStrategy = NewObject<ULookBehavior>(this);
	CurrentCrouchStrategy = NewObject<UCrouchBehavior>(this);
	CurrentRunStrategy = NewObject<URunBehavior>(this);



}

// Called every frame
void APhasmophobiaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APhasmophobiaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::LookAround);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Crouch);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::Run);



	}
}

void APhasmophobiaPlayer::Move(const FInputActionValue& Value)
{
	if (CurrentMoveStrategy && CurrentMoveStrategy->GetClass()->ImplementsInterface(UPlayerBehavior::StaticClass()))
	{
		IPlayerBehavior* MoveStrategy = Cast<IPlayerBehavior>(CurrentMoveStrategy);
		if (MoveStrategy)
		{
			MoveStrategy->ExecuteBehavior(this, Value);
		}
	}
}

void APhasmophobiaPlayer::LookAround(const FInputActionValue& Value)
{
	if (CurrentLookStrategy && CurrentLookStrategy->GetClass()->ImplementsInterface(UPlayerBehavior::StaticClass()))
	{
		IPlayerBehavior* LookStrategy = Cast<IPlayerBehavior>(CurrentLookStrategy);
		if (LookStrategy)
		{
			LookStrategy->ExecuteBehavior(this, Value);
		}
	}
}

void APhasmophobiaPlayer::Crouch(const FInputActionValue& Value)
{
	if (CurrentCrouchStrategy && CurrentCrouchStrategy->GetClass()->ImplementsInterface(UPlayerBehavior::StaticClass()))
	{
		IPlayerBehavior* CrouchStrategy = Cast<IPlayerBehavior>(CurrentCrouchStrategy);
		if (CrouchStrategy)
		{
			CrouchStrategy->ExecuteBehavior(this, Value);
		}
	}
}

void APhasmophobiaPlayer::Run(const FInputActionValue& Value)
{
	if (CurrentRunStrategy && CurrentRunStrategy->GetClass()->ImplementsInterface(UPlayerBehavior::StaticClass()))
	{
		IPlayerBehavior* RunStrategy = Cast<IPlayerBehavior>(CurrentRunStrategy);
		if (RunStrategy)
		{
			RunStrategy->ExecuteBehavior(this, Value);
		}
	}
}

// 전략 동적 변경
void APhasmophobiaPlayer::SetMoveStrategy(TObjectPtr<UObject> NewMoveStrategy)
{
	if (NewMoveStrategy && NewMoveStrategy->Implements<UMoveBehavior>())
	{
		CurrentMoveStrategy = NewMoveStrategy;
	}
}


