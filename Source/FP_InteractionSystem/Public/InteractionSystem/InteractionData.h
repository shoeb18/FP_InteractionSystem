#pragma once

#include "CoreMinimal.h"
#include "InteractionData.generated.h"

// Interaction Types
UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Press UMETA(DisplayName = "Press"),
	Hold  UMETA(DisplayName = "Hold")
};