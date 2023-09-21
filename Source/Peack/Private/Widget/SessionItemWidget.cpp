// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SessionItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Object/SessionItemObject.h"

#include "Subsystem/MultiplayerSubsystem.h"

bool USessionItemWidget::Initialize()
{
    if (Super::Initialize() == false)
    {
        return false;
    }

    if (UGameInstance* MyGameInstance = GetGameInstance())
    {
        MultiplayerSubsystem = MyGameInstance->GetSubsystem<UMultiplayerSubsystem>();
    }

    if (Button_Join)
    {
        Button_Join->OnClicked.AddDynamic(
            this,
            &USessionItemWidget::OnClickButton_Join
        );
    }
   
    return true;
}

// function nay duoc goi, khi list view add item 
void USessionItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    if (SessionItemObject = Cast<USessionItemObject>(ListItemObject))
    {
        if (Text_SessionId)
        {
            Text_SessionId->SetText(FText::FromString(SessionItemObject->SessionId));
        }

        if (Text_CreatedBy)
        {
            Text_CreatedBy->SetText(FText::FromString(SessionItemObject->CreatedBy));
        }
    }
}

void USessionItemWidget::OnClickButton_Join()
{
    if (Button_Join)
    {
        Button_Join->SetIsEnabled(false);
    }

    if (MultiplayerSubsystem && SessionItemObject)
    {
        MultiplayerSubsystem->JoinSession(SessionItemObject->SearchResult);
    }
}
