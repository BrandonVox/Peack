// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionItemObject.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API USessionItemObject : public UObject
{
	GENERATED_BODY()

public: // Property
	FString SessionId;
	FString CreatedBy;
};
