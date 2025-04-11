// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayer.h"
#include "MoveBehavior.h"
#include "LookBehavior.h"
#include "CrouchBehavior.h"
#include "RunBehavior.h"
#include "EquipItemBehavior.h"
#include "DetachItemBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "PhasmophobiaPlayerController.h"
#include "IItemBehavior.h"




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

	// 아이템 붙이기
	ItemComp = CreateDefaultSubobject<USceneComponent>(TEXT("ItemComp"));
	ItemComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	// input 고정 시켜두기
	
}

// Called when the game starts or when spawned
void APhasmophobiaPlayer::BeginPlay()
{
	Super::BeginPlay();

	// input mapping
	if (APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0); // 우선순위 0
			Subsystem->AddMappingContext(ItemMappingContext, 1);


		}
	}

	// 런타임에서만 사용
	// 기본 이동 전략 설정
	CurrentMoveStrategy = NewObject<UMoveBehavior>(this);
	CurrentLookStrategy = NewObject<ULookBehavior>(this);
	CurrentCrouchStrategy = NewObject<UCrouchBehavior>(this);
	CurrentRunStrategy = NewObject<URunBehavior>(this);

	CurrentEquipStrategy = NewObject<UEquipItemBehavior>(this);
	CurrentDetachStrategy = NewObject<UDetachItemBehavior>(this);


}

// Called every frame
void APhasmophobiaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsRunning && CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Current Stamina: %f"), CurrentStamina));
	}
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
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APhasmophobiaPlayer::OnRunReleased);

		EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Equip);

		EnhancedInput->BindAction(DetachItemAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Detach);





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
	if (GetVelocity().Size() <= 0.1f) return;
	bIsRunning = true;
	if (CurrentRunStrategy && CurrentRunStrategy->GetClass()->ImplementsInterface(UPlayerBehavior::StaticClass()))
	{
		IPlayerBehavior* RunStrategy = Cast<IPlayerBehavior>(CurrentRunStrategy);
		if (RunStrategy)
		{
			RunStrategy->ExecuteBehavior(this, Value);
		}
	}
	
}

void APhasmophobiaPlayer::OnRunReleased(const FInputActionValue& Value)
{
	bIsRunning = false;
}

void APhasmophobiaPlayer::Equip(const FInputActionValue& Value)
{
	if (ItemActors.Num() >= 3) return;
	APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetController());
	
	if (PC && PC->TargetItem && !ItemActors.Contains(PC->TargetItem))
	{
		ownedItem = PC->TargetItem;
		ownedItem->SetOwner(this);
		ItemActors.Add(ownedItem);
		UE_LOG(LogTemp, Log, TEXT("Item Add: %s / Item count: %d"), *ownedItem->GetName(), ItemActors.Num());

		if (CurrentEquipStrategy && CurrentEquipStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
		{
			IItemBehavior* EquipStrategy = Cast<IItemBehavior>(CurrentEquipStrategy);
			if (EquipStrategy)
			{
				EquipStrategy->ExecuteBehavior(this, Value);
			}
		}
	}
	if (ItemActors.Num() > 0) {bHasItem = true;};
	
}

void APhasmophobiaPlayer::Detach(const FInputActionValue& Value)
{
	if (CurrentDetachStrategy && CurrentDetachStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
	{
		IItemBehavior* DetachStrategy = Cast<IItemBehavior>(CurrentDetachStrategy);
		if (DetachStrategy)
		{
			DetachStrategy->ExecuteBehavior(this, Value);
		}
	}
	if (ItemActors.Num() == 0) { bHasItem = false; };
}

// 전략 동적 변경
void APhasmophobiaPlayer::SetMoveStrategy(TObjectPtr<UObject> NewMoveStrategy)
{
	if (NewMoveStrategy && NewMoveStrategy->Implements<UMoveBehavior>())
	{
		CurrentMoveStrategy = NewMoveStrategy;
	}
}


