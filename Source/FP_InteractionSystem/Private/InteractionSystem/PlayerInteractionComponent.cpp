// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionSystem/PlayerInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "InteractionSystem/Interactable.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "InteractionSystem/InteractionWidget.h"

// Sets default values for this component's properties
UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ACharacter>(GetOwner());

	if (OwningCharacter)
	{
		OwningCharacter->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnOverlapBegin);
		OwningCharacter->GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &UPlayerInteractionComponent::OnOverlapEnd);

		// interact widget
		InteractionWidgetComponent = Cast<UWidgetComponent>(OwningCharacter->AddComponentByClass(UWidgetComponent::StaticClass(), true, FTransform::Identity, false));

		if (InteractionWidgetComponent)
		{
			if (InteractionWidgetClass)
			{
				InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Widget class for interaction is not assigned in interaction component"));
			}
			InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			InteractionWidgetComponent->SetDrawSize(FVector2D(250.0f, 30.0f));
			InteractionWidgetComponent->SetVisibility(false);

		}
	}
	
}

void UPlayerInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetOwner())
	{		
		if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			InteractablesInRange.AddUnique(OtherActor);
			SetItemHightlight(OtherActor, true);
			DisplayInteractionWidget();
		}
	}
}

void UPlayerInteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			InteractablesInRange.Remove(OtherActor);
			SetItemHightlight(OtherActor, false);
			DisplayInteractionWidget();
		}
	}
}

AActor* UPlayerInteractionComponent::GetActiveInteractable() const
{
	if (InteractablesInRange.Num() > 0)
	{
		return InteractablesInRange[0];
	}
	return nullptr;
}

void UPlayerInteractionComponent::InteractBegin()
{
	AActor* ActiveInteractable = GetActiveInteractable();
	if (!ActiveInteractable) return;

	EInteractionType InteractionType = IInteractable::Execute_GetInteractionType(ActiveInteractable);

	switch (InteractionType)
	{
	case EInteractionType::Press:
		InteractWithActiveInteractable();
		break;

	case EInteractionType::Hold:
		evt_OnInteractPressOngoing.AddDynamic(this, &UPlayerInteractionComponent::OnInteractionInputPress);
		break;
	}
}

void UPlayerInteractionComponent::InteractWithActiveInteractable()
{
	if (!InteractablesInRange.IsEmpty())
	{
		IInteractable::Execute_Interact(GetActiveInteractable(), GetOwner());
	}
}

void UPlayerInteractionComponent::OnInteractionInputPress(float ElapsedSeconds)
{
	AActor* ActiveInteractable = GetActiveInteractable();
	if (!ActiveInteractable) return;

	const float HoldDuration = IInteractable::Execute_GetHoldDuration(ActiveInteractable);

	UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());

	if (WidgetInstance && WidgetInstance->InteractionProgressBar)
	{
		WidgetInstance->InteractionProgressBar->SetPercent(ElapsedSeconds / HoldDuration);
	}

	if (ElapsedSeconds > HoldDuration)
	{
		InteractWithActiveInteractable();
		UnBindInteractPressDelegate();
	}
}

void UPlayerInteractionComponent::SetItemHightlight(AActor* Item, bool Show)
{
	if (!Item) return;

	TArray<UStaticMeshComponent*> MeshComps;
	Item->GetComponents<UStaticMeshComponent>(MeshComps);

	if (Show)
	{
		for (UStaticMeshComponent* MeshComp : MeshComps)
		{
			if (MeshComp)
			{
				MeshComp->SetOverlayMaterial(ObjectHighlightMaterial);
			}
		}
	}
	else 
	{
		for (UStaticMeshComponent* MeshComp : MeshComps)
		{
			if (MeshComp)
			{
				MeshComp->SetOverlayMaterial(nullptr);
			}
		}
	}
}

void UPlayerInteractionComponent::UnBindInteractPressDelegate()
{
	evt_OnInteractPressOngoing.RemoveDynamic(this, &UPlayerInteractionComponent::OnInteractionInputPress);

	UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());

	if (WidgetInstance && WidgetInstance->InteractionProgressBar)
	{
		WidgetInstance->InteractionProgressBar->SetPercent(0); // reset progress bar
	}
}

void UPlayerInteractionComponent::OnInteractPressOngoing(const FInputActionInstance& Instance)
{
	float ElapsedSeconds = Instance.GetElapsedTime();

	UE_LOG(LogTemp, Log, TEXT("Elapsed Time: %f"), ElapsedSeconds);

	// call event 
	evt_OnInteractPressOngoing.Broadcast(ElapsedSeconds);
}

void UPlayerInteractionComponent::InteractInput()
{
	InteractBegin();
}

void UPlayerInteractionComponent::DisplayInteractionWidget()
{
	AActor* ActiveInteractable = GetActiveInteractable();

	if (ActiveInteractable)
	{
		if (InteractionWidgetComponent)
		{
			UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());
			if (WidgetInstance)
			{
				WidgetInstance->CurrentInteractionType = IInteractable::Execute_GetInteractionType(ActiveInteractable);
				WidgetInstance->SetInteractionText();
			}

			const FVector InteractWidgetSpawnLocation = IInteractable::Execute_GetWidgetSpawnLocation(ActiveInteractable);
			InteractionWidgetComponent->SetWorldLocation(InteractWidgetSpawnLocation);
			InteractionWidgetComponent->SetVisibility(true);
		}
	}
	else
	{
		if (InteractionWidgetComponent)
		{
			InteractionWidgetComponent->SetVisibility(false);
		}
	}
}

void UPlayerInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OwningCharacter)
	{
		if (UCapsuleComponent* Capsule = OwningCharacter->GetCapsuleComponent())
		{
			Capsule->OnComponentBeginOverlap.RemoveDynamic(this, &UPlayerInteractionComponent::OnOverlapBegin);
			Capsule->OnComponentEndOverlap.RemoveDynamic(this, &UPlayerInteractionComponent::OnOverlapEnd);
		}
	}

	UnBindInteractPressDelegate();

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

