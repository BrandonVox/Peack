// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StartupWidget.h"

#include "Components/Button.h"

#include "Components/TextBlock.h"

#include "Subsystem/MultiplayerSubsystem.h"

bool UStartupWidget::Initialize()
{
    if (Super::Initialize() == false)
    {
        return false;
    }

    InputMode_UI();

    // lien ket onclick

    if (Button_FindSessions)
    {
        Button_FindSessions->OnClicked.AddDynamic(
            this,
            &UStartupWidget::OnClickButton_FindSessions
        );
    }

    if (Button_CreateSession)
    {
        Button_CreateSession->OnClicked.AddDynamic(
            this,
            &UStartupWidget::OnClickButton_CreateSession
        );
    }



    return true;
}

void UStartupWidget::ShowNotify(const FString& NotifyString, const FLinearColor& NotifyColor)
{
    PlayAnimation(ShowNotifyAnimation);

    if (Text_Notify)
    {
        Text_Notify->SetText(FText::FromString(NotifyString));
        Text_Notify->SetColorAndOpacity(NotifyColor);
    }
}

void UStartupWidget::InputMode_UI()
{
    if (APlayerController* PC = GetOwningPlayer())
    {

        bIsFocusable = true;

        PC->SetShowMouseCursor(true);

        FInputModeUIOnly InputMode_UI;
        InputMode_UI.SetWidgetToFocus(TakeWidget());

        PC->SetInputMode(InputMode_UI);
    }
}

void UStartupWidget::OnClickButton_FindSessions()
{
    ShowNotify(TEXT("OnClickButton_FindSessions"), FLinearColor::Gray);
}

void UStartupWidget::OnClickButton_CreateSession()
{
    ShowNotify(TEXT("OnClickButton_CreateSession"), FLinearColor::Blue);

    if (UGameInstance* MyGameInstance = GetGameInstance())
    {
        UMultiplayerSubsystem* MS = MyGameInstance->GetSubsystem<UMultiplayerSubsystem>();
        if (MS)
        {
            MS->CreateSession();
        }
    }
}
