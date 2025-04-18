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
#include "IItemBehavior.h"
#include "ItemStrategy.h"
#include "GhostBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerAnimInstance.h"





// Sets default values
APhasmophobiaPlayer::APhasmophobiaPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���� �޽� �ε�
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

	// ��� ������
	GetCharacterMovement()->MaxStepHeight = 45.f;
	GetCharacterMovement()->SetWalkableFloorAngle(60.f);
	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	// ������ ���̱�
	/*ItemComp = CreateDefaultSubobject<USceneComponent>(TEXT("ItemComp"));
	ItemComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));*/

	// ������ ���� �ʱ�ȭ
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
			Subsystem->AddMappingContext(PlayerMappingContext, 0); // �켱���� 0
			Subsystem->AddMappingContext(ItemMappingContext, 1);


		}
	}

	if (CenterWidget)
	{
		CenterUI = CreateWidget<UUserWidget>(GetWorld(), CenterWidget);
		if (CenterUI)
		{
			CenterUI->AddToViewport();
		}
	}

	// ��Ÿ�ӿ����� ���
	// �⺻ �̵� ���� ����
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

	// �� ��ȣ�ۿ�
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

	

	// ���¹̳� ä���
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
}

// Called to bind functionality to input
void APhasmophobiaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhasmophobiaPlayer::LookAround);


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
	// if (ItemActors.Num() >= 4) return;
	/*APhasmophobiaPlayerController* PC = Cast<APhasmophobiaPlayerController>(GetController());*/
	
	if (PC && PC->TargetItem && !ItemActors.Contains(PC->TargetItem))
	{
		ownedItem = PC->TargetItem;
		ownedItem->SetOwner(this);

		if (CurrentEquipStrategy && CurrentEquipStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
		{
			IItemBehavior* EquipStrategy = Cast<IItemBehavior>(CurrentEquipStrategy);
			if (EquipStrategy)
			{
				EquipStrategy->ExecuteBehavior(this, Value);
			}
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
	if (CurrentSwitchStrategy && CurrentSwitchStrategy->GetClass()->ImplementsInterface(UItemBehavior::StaticClass()))
	{
		IItemBehavior* SwitchStrategy = Cast<IItemBehavior>(CurrentSwitchStrategy);
		if (SwitchStrategy)
		{
			SwitchStrategy->ExecuteBehavior(this, Value);
		}
	}
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

	for (AActor* Item : ItemActors)
	{
		if (Item == nullptr)
		{
			bHasItem = false;
			break;
		}
	}

	/*if (!bHasItem)
	{
		UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance)
		{
			AnimInstance->bHasItem = bHasItem;
		}
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

// ���� ���� ����
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
	NotifySanityChanged();

	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, FString::Printf(TEXT("%.2f"), Sanity));
}

void APhasmophobiaPlayer::UseItem()
{
	if (currentItem != nullptr) {
		AItem_Base* HoldingItem = Cast<AItem_Base>(currentItem);
		if (HoldingItem)
		{
			HoldingItem->SetItemStrategy(HoldingItem->ItemStrategy);
			HoldingItem->UseItem();
		}
	}
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
