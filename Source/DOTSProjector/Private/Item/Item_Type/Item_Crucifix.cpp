// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Crucifix.h"
#include "Function_Crucifix.h"


AItem_Crucifix::AItem_Crucifix()
{
	static const FString ContextString(TEXT("ItemDataTable"));
	FItemData* ItemData = ItemDataTable->FindRow<FItemData>("Crucifix", ContextString);
	MeshComp->SetStaticMesh(ItemData->ItemMesh);
}

void AItem_Crucifix::BeginPlay()
{	
	ItemStrategy = NewObject<UFunction_Crucifix>(this);
}
