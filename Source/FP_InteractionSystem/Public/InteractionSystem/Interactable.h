// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FP_INTERACTIONSYSTEM_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction System")
	void Interact(AActor* Interactor);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction System")
	const FVector GetWidgetSpawnLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction System")
	EInteractionType GetInteractionType() const;
};
