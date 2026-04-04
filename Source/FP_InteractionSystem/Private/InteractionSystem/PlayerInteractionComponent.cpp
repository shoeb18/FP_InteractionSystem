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
			InteractionWidgetComponent->SetWidgetClass(InteractionWidgetClass);
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
			DisplayInteractionWidget();
		}
	}
}

AActor* UPlayerInteractionComponent::GetActiveInteractable() const
{
	return InteractablesInRange[0];
}

void UPlayerInteractionComponent::InteractBegin()
{
	if (!InteractablesInRange.IsEmpty())
	{
		EInteractionType InteractionType = IInteractable::Execute_GetInteractionType(GetActiveInteractable());

		switch (InteractionType)
		{
		case EInteractionType::Press:
			InteractWithActiveInteractable();
			break;

		case EInteractionType::Hold:
			// Handle Hold interaction
			evt_OnInteractPressOngoing.AddDynamic(this, &UPlayerInteractionComponent::OnInteractionInputPress);
			break;
		}
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
	const float HoldDuration = IInteractable::Execute_GetHoldDuration(GetActiveInteractable());

	UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());

	if (WidgetInstance)
	{
		// set percent
		WidgetInstance->InteractionProgressBar->SetPercent(ElapsedSeconds / HoldDuration);
	}

	if (ElapsedSeconds > HoldDuration)
	{
		InteractWithActiveInteractable();
		UnBindInteractPressDelegate();
	}
}

void UPlayerInteractionComponent::UnBindInteractPressDelegate()
{
	evt_OnInteractPressOngoing.RemoveDynamic(this, &UPlayerInteractionComponent::OnInteractionInputPress);

	UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());
	if (WidgetInstance)
	{
		// reset percent
		WidgetInstance->InteractionProgressBar->SetPercent(0);
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
	if (!InteractablesInRange.IsEmpty())
	{
		if (InteractionWidgetComponent)
		{
			UInteractionWidget* WidgetInstance = Cast<UInteractionWidget>(InteractionWidgetComponent->GetUserWidgetObject());
			if (WidgetInstance)
			{
				WidgetInstance->SetInteractionText();
			}

			const FVector InteractWidgetSpawnLocation = IInteractable::Execute_GetWidgetSpawnLocation(GetActiveInteractable());
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


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

