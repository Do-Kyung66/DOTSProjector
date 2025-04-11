// Fill out your copyright notice in the Description page of Project Settings.


#include "DetachItemBehavior.h"

UDetachItemBehavior::UDetachItemBehavior()
{

}

void UDetachItemBehavior::ExecuteBehavior(AActor* Owner, const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Detach!"));
}
