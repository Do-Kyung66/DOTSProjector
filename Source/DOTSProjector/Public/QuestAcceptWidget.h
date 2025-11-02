#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DT_Quest.h"
#include "QuestAcceptWidget.generated.h"

UCLASS()
class DOTSPROJECTOR_API UQuestAcceptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* button_accept;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* questdescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestID = 0;

	class UQuestManager* QM = nullptr;
	FQuestData* Data;

	UFUNCTION()
	void OnButtonAcceptClicked();


};
