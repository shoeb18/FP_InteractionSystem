// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystem/InteractionData.h"
#include "PlayerInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionPressOngoing, float, ElapsedSeconds);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FP_INTERACTIONSYSTEM_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// owning character
	UPROPERTY(BlueprintReadOnly, Category="InteractionComponent")
	ACharacter* OwningCharacter;	

	UPROPERTY(BlueprintReadOnly, Category = "InteractionComponent")
	TArray<AActor*> InteractablesInRange;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly, Category="InteractionComponent")
	class UWidgetComponent* InteractionWidgetComponent;

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent")
	void DisplayInteractionWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionComponent")
	TSubclassOf<class UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditAnywhere, Category = "InteractionComponent")
	UMaterialInterface* ObjectHighlightMaterial;

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent")
	AActor* GetActiveInteractable() const;

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent")
	void InteractBegin();

	UFUNCTION(BlueprintCallable, Category = "InteractionComponent")
	void InteractWithActiveInteractable();

	UFUNCTION()
	void OnInteractionInputPress(float ElapsedSeconds);

	UFUNCTION()
	void SetItemHightlight(AActor* Item, bool Show);

	UPROPERTY(BlueprintAssignable, Category = "InteractionComponent")
	FOnInteractionPressOngoing evt_OnInteractPressOngoing;
	


public:

	/** IMPORTANT - BIND THESE INPUTS IN PLAYER CLASS */

	UFUNCTION()
	void InteractInput();

	UFUNCTION()
	void OnInteractPressOngoing(const FInputActionInstance& Instance);

	UFUNCTION()
	void UnBindInteractPressDelegate();

};
