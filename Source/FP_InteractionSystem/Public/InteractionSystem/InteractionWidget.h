// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "InteractionSystem/InteractionData.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class FP_INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction System")
	EInteractionType CurrentInteractionType = EInteractionType::Press;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction System", meta = (BindWidget))
	UProgressBar* InteractionProgressBar;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interaction System")
	void SetInteractionText();

};
