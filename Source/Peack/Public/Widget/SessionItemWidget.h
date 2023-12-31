// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Blueprint/IUserObjectListEntry.h"

#include "SessionItemWidget.generated.h"

class UButton;
class UTextBlock;

class UMultiplayerSubsystem;

class USessionItemObject;

UCLASS()
class PEACK_API USessionItemWidget: 
	public UUserWidget,
	public IUserObjectListEntry
{
	GENERATED_BODY()
	
public: // Function
	virtual bool Initialize() override;

protected: // Function
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private: // Function

	UFUNCTION()
	void OnClickButton_Join();

private: // Property
	UPROPERTY()
	TObjectPtr<USessionItemObject> SessionItemObject;

	UPROPERTY()
	TObjectPtr<UMultiplayerSubsystem> MultiplayerSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Join;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_SessionId;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_CreatedBy;
};