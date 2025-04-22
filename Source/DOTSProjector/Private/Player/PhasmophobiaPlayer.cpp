// Fill out your copyright notice in the Description page of Project Settings.


#include "PhasmophobiaPlayer.h"
#include "MoveBehavior.h"
#include "LookBehavior.h"
#include "CrouchBehavior.h"
#include "RunBehavior.h"
#include "EquipItemBehavior.h"
#include "SwitchItemBehavior.h"
#include "DetachItemBehavior.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "PhasmophobiaPlayerController.h"
#include "ItemStrategy.h"
#include "GhostBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerAnimInstance.h"

#include "Net/UnrealNetwork.h"

#include "DefaultCursorWidget.h"
#include "Components/Image.h"






// Sets default values
APhasmophobiaPlayer::APhasmophobiaPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 메시 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Player/Assets/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/rp_manuel_rigged_001_ue4.rp_manuel_rigged_001_ue4'"));

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTemp(TEXT("/Script/Engine.SkeletalMesh'/Game/Player/Assets/Scanned3DPeoplePack/RP_Character/rp_manuel_rigged_001_ue4/PlayerHandMesh2.PlayerHandMesh2'"));

	if (MeshTemp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshTemp.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -88.0), FRotator(0.0, -90, 0.0));
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 27.0f, 153.0f));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->TargetArmLength = 0.0f;

	CamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	CamComp->SetupAttachment(SpringArmComp);
	CamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 계단 오르기
	GetCharacterMovement()->MaxStepHeight = 45.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60.f);
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// 아이템 붙이기
	/*ItemComp = CreateDefaultSubobject<USceneComponent>(TEXT("ItemComp"));
	ItemComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));*/

	// 아이템 슬롯 초기화
	ItemActors.SetNum(4);
	ItemActors[0] = nullptr;
	
}

// Called when the game starts or when spawned
void APhasmophobiaPlayer::BeginPlay()
{
	Super::BeginPlay();
	PC = Cast<APhasmophobiaPlayerController>(GetController());
	// input mapping
	if (PC)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0); // 우선순위 0
			Subsystem->AddMappingContext(ItemMappingContext, 1);


		}
	}

	/*if (CenterWidget)
	{
		CenterUI = CreateWidget<UUserWidget>(GetWorld(), CenterWidget);
		if (CenterUI)
		{
			CenterUI->AddToViewport();
		}
	}*/

	if (CenterWidget)
	{
		CenterUI = CreateWidget<UDefaultCursorWidget>(GetWorld(), CenterWidget);
		CenterUI->AddToViewport();
		
		CenterUI->ShowDefaultCursor();
	}

	

	// 런타임에서만 사용
	// 기본 이동 전략 설정
	CurrentMoveStrategy = NewObject<UMoveBehavior>(this);
	CurrentLookStrategy = NewObject<ULookBehavior>(this);
	CurrentCrouchStrategy = NewObject<UCrouchBehavior>(this);
	CurrentRunStrategy = NewObject<URunBehavior>(this);

	CurrentEquipStrategy = NewObject<UEquipItemBehavior>(this);
	CurrentSwitchStrategy = NewObject<USwitchItemBehavior>(this);
	CurrentDetachStrategy = NewObject<UDetachItemBehavior>(this);
}

// Called every frame
void APhasmophobiaPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ItemTrace();
	// 문 상호작용
	/*FHitResult DoorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, DoorHit);

	if (DoorHit.bBlockingHit && DoorHit.GetActor() && DoorHit.GetActor()->GetName().Contains(TEXT("Door")))
	{
		PC->CurrentMouseCursor = EMouseCursor::Hand;
		PC->TargetDoor = DoorHit.GetActor();
		UE_LOG(LogTemp, Warning, TEXT("Door Open"));
	}

	else
	{
		PC->CurrentMouseCursor = EMouseCursor::Default;
		PC->TargetDoor = nullptr;
	}*/

	

	// 스태미나 채우기
	if (!bIsRunning && CurrentStamina < MaxStamina)
	{
		CurrentStamina += StaminaRegenRate * DeltaTime;
		CurrentStamina = FMath::Clamp(CurrentStamina, 0.0f, MaxStamina);
	}
	CheckGhostOnScreen(DeltaTime);

	if (GEngine)
	{

		GEngine->AddOnScreenDebugMessage(4, 1.5f, FColor::Green, FString::Printf(TEXT("Current Stamina: %f"), CurrentStamina));
	}
	if (PC)
	{
		if (PC->bCanInteract)
		{
			CenterUI->ShowHandCursor();
		}
		else
		{
			CenterUI->ShowDefaultCursor();
		}
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

		EnhancedInput->BindAction(ItemAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::ActivateItem);
		EnhancedInput->BindAction(UseAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::UseItem);

		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::PlayerCrouch);

		EnhancedInput->BindAction(RunAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::Run);
		EnhancedInput->BindAction(RunAction, ETriggerEvent::Completed, this, &APhasmophobiaPlayer::OnRunReleased);

		EnhancedInput->BindAction(EquipItemAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Equip);
		EnhancedInput->BindAction(SwitchItemAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::Switch);
		EnhancedInput->BindAction(DetachItemAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Detach);



		//EnhancedInput->BindAction(JournalAction, ETriggerEvent::Started, this, &APhasmophobiaPlayer::Journal);

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


void APhasmophobiaPlayer::PlayerCrouch(const FInputActionValue& Value)
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
	if (PC && TargetItem)
	{
		ServerRPC_Equip();
	}
	// if (ItemActors.Num() >= 4) return;
	/*APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetController());*/
	
	

	/*if (bHasItem)
	{	
		UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->bHasItem = bHasItem;
		}
	}*/
	
}

void APhasmophobiaPlayer::Switch(const FInputActionValue& Value)
{
	ServerRPC_Switch(Value.Get<float>());
}

void APhasmophobiaPlayer::Detach(const FInputActionValue& Value)
{
	ServerRPC_Detach();

	/*
	if (!bHasItem)
	{
		UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->bHasItem = bHasItem;
		}b
	}*/
}

//void APhasmophobiaPlayer::Journal(const FInputActionValue& Value)
//{
//	if (Value.Get<bool>()) 
//	{
//		if (APhasmophobiaPlayerController* PC = GetController())
//		{
//			if (APhasmophobiaHUD* HUD = Cast<APhasmophobiaHUD>(PC->GetHUD()))
//			{
//				HUD->ToggleJournalUI();
//				UE_LOG(LogTemp, Warning, TEXT("Journal toggled from PhasmophobiaPlayer"));
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("Failed to cast HUD to AMyPhasmophobiaHUD"));
//			}
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("No PlayerController found"));
//		}
//	}
//}

// 전략 동적 변경
void APhasmophobiaPlayer::SetMoveStrategy(TObjectPtr<UObject> NewMoveStrategy)
{
	if (NewMoveStrategy && NewMoveStrategy->Implements<UMoveBehavior>())
	{
		CurrentMoveStrategy = NewMoveStrategy;
	}
}

void APhasmophobiaPlayer::AddObserver(IObserver* Observer)
{
	Observers.Add(Observer);
}

void APhasmophobiaPlayer::RemoveObserver(IObserver* Observer)
{
	Observers.Remove(Observer);
}

void APhasmophobiaPlayer::NotifySanityChanged()
{
	for (IObserver* Observer : Observers)
	{
		if (Observer)
		{
			Observer->PlayerSanityChanged(Sanity);
		}
	}
}

void APhasmophobiaPlayer::DecreaseSanity(float Amount)
{
	Sanity -= Amount;
	if (Sanity <= 60) {
		NotifySanityChanged();
	}
	
	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%.2f"), Sanity));
}

void APhasmophobiaPlayer::UseItem()
{
	ServerRPC_UseItem();
}

void APhasmophobiaPlayer::ActivateItem()
{
	ServerRPC_ItemAction();
}

void APhasmophobiaPlayer::CheckGhostOnScreen(float DeltaTime)
{
	if (!PC) return;
	FVector CameraLoc;
	FRotator CameraRot;
	PC->GetPlayerViewPoint(CameraLoc, CameraRot);

	int32 ViewX, ViewY;
	PC->GetViewportSize(ViewX, ViewY);

	for (TActorIterator<AGhostBase> It(GetWorld()); It; ++It)
	{
		AGhostBase* Ghost = *It;
		if (!Ghost || Ghost->IsHidden()) continue;

		FVector2D ScreenLocation;
		bool bOnScreen = UGameplayStatics::ProjectWorldToScreen(PC, Ghost->GetActorLocation(), ScreenLocation);

		if (bOnScreen &&
			ScreenLocation.X >= 0 && ScreenLocation.X <= ViewX &&
			ScreenLocation.Y >= 0 && ScreenLocation.Y <= ViewY)
		{
			FHitResult Hit;
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(this);
			TraceParams.AddIgnoredActor(Ghost);

			bool bHit = GetWorld()->LineTraceSingleByChannel(
				Hit,
				CameraLoc,
				Ghost->GetActorLocation() + FVector(0.f, 0.f, 80.f),
				ECollisionChannel::ECC_Visibility,
				TraceParams
			);

			if (!bHit || Hit.GetActor() == Ghost)
			{	
				DecreaseSanity(DeltaTime * Ghost->GetSanityDestoryRate());
				break;
			}
		}
	}
}


void APhasmophobiaPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APhasmophobiaPlayer, CurrentEquipStrategy);
	DOREPLIFETIME(APhasmophobiaPlayer, ownedItem);
	DOREPLIFETIME(APhasmophobiaPlayer, TargetItem);
	DOREPLIFETIME(APhasmophobiaPlayer, currentItem);
	DOREPLIFETIME(APhasmophobiaPlayer, CurrentItemType);
	DOREPLIFETIME(APhasmophobiaPlayer, bHasItem);
	DOREPLIFETIME(APhasmophobiaPlayer, ItemActors);
	DOREPLIFETIME(APhasmophobiaPlayer, CurrentSwitchStrategy);
	DOREPLIFETIME(APhasmophobiaPlayer, SwitchStrategy);
	DOREPLIFETIME(APhasmophobiaPlayer, CurrentItemIndex);
	DOREPLIFETIME(APhasmophobiaPlayer, ScrollValue);
	DOREPLIFETIME(APhasmophobiaPlayer, NextIndex);
	DOREPLIFETIME(APhasmophobiaPlayer, StartIndex);

}

void APhasmophobiaPlayer::ServerRPC_Equip_Implementation()
{
	if (!ItemActors.Contains(TargetItem))
	{
		ownedItem = TargetItem;
		ownedItem->SetOwner(this);

		if (CurrentEquipStrategy && CurrentEquipStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
		{
			EquipStrategy = Cast<IItemBehavior>(CurrentEquipStrategy);
			MulticastRPC_Equip();
		}
	}

	for (AActor* Item : ItemActors)
	{
		if (Item != nullptr)
		{
			bHasItem = true;
			break;
		}
	}

}

void APhasmophobiaPlayer::MulticastRPC_Equip_Implementation()
{
	if (EquipStrategy)
	{
		EquipStrategy->ExecuteBehavior(this, 0);
	}
}

void APhasmophobiaPlayer::ItemTrace()
{
	ServerRPC_ItemTrace();
}


void APhasmophobiaPlayer::ServerRPC_ItemTrace_Implementation()
{
	FVector Start = CamComp->GetComponentLocation();
	FVector End = Start + CamComp->GetForwardVector() * 300.f;

	FHitResult Hitinfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(Hitinfo, Start, End, ECC_Visibility, params))
	{
		// GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *Hitinfo.GetActor()->GetName()));

		AActor* HitActor = Hitinfo.GetActor();
		if (HitActor)
		{
			FString ActorName = HitActor->GetName();

			if (ActorName.Contains(TEXT("item"), ESearchCase::IgnoreCase))
			{
				TargetItem = HitActor;
			}
			else
			{
				TargetItem = nullptr;
			}
		}
	}
}

void APhasmophobiaPlayer::ServerRPC_UseItem_Implementation()
{
	if (currentItem != nullptr) {
		AItem_Base* HoldingItem = Cast<AItem_Base>(currentItem);
		if (HoldingItem)
		{
			HoldingItem->SetItemStrategy(HoldingItem->ItemStrategy);
			HoldingItem->UseItem();
			// MulticastRPC_UseItem(HoldingItem);
		}
	}
}

void APhasmophobiaPlayer::MulticastRPC_UseItem_Implementation(AItem_Base* Item)
{
	// Item->UseItem();
}

void APhasmophobiaPlayer::ServerRPC_Detach_Implementation()
{
	if (CurrentDetachStrategy && CurrentDetachStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
	{
		DetachStrategy = Cast<IItemBehavior>(CurrentDetachStrategy);
		
		MulticastRPC_Detach();
	}

	for (AActor* Item : ItemActors)
	{
		if (Item == nullptr)
		{
			bHasItem = false;
			break;
		}
	}
}

void APhasmophobiaPlayer::MulticastRPC_Detach_Implementation()
{
	if (DetachStrategy)
	{
		DetachStrategy->ExecuteBehavior(this, 0);
	}
}

void APhasmophobiaPlayer::ServerRPC_Switch_Implementation(float ScrollData)
{
	if (CurrentSwitchStrategy && CurrentSwitchStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
	{
		//GEngine->AddOnScreenDebugMessage(22, 1.5f, FColor::Green, TEXT("On Switch Function"));

		SwitchStrategy.SetObject(CurrentSwitchStrategy);
		SwitchStrategy.SetInterface(Cast<IItemBehavior>(CurrentSwitchStrategy));

		if (SwitchStrategy)
		{
			SwitchStrategy->ExecuteBehavior(this, ScrollData);
			// GEngine->AddOnScreenDebugMessage(23, 1.5f, FColor::Green, TEXT("Switch Working?"));
			MulticastRPC_Switch(CurrentItemIndex);
		}
	}
}

void APhasmophobiaPlayer::MulticastRPC_Switch_Implementation(int32 NextItemIndex)
{

	if (ItemActors.IsValidIndex(NextItemIndex))
	{
		if (currentItem)
		{
			currentItem->SetActorHiddenInGame(true);
		}

		currentItem = ItemActors[NextItemIndex];
		CurrentItemIndex = NextItemIndex;

		if (currentItem)
		{
			currentItem->SetActorHiddenInGame(false);
			currentItem->AttachToComponent(ItemComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
	

}

void APhasmophobiaPlayer::ServerRPC_ItemAction_Implementation()
{
	if (TargetItem)
	{
		AItem_Base* Item = Cast<AItem_Base>(TargetItem);
		if (Item)
		{
			Item->UseItem();
		}
		TargetItem = nullptr;
	}
}

