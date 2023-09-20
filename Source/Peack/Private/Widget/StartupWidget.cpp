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

    if (UGameInstance* MyGameInstance = GetGameInstance())
    {
        MultiplayerSubsystem = MyGameInstance->GetSubsystem<UMultiplayerSubsystem>();
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

    /*
    * Bind Multiplayer Subsystem's Delegates
    */
    if (MultiplayerSubsystem)
    {
        MultiplayerSubsystem->CreateSessionDoneDelegate.AddUObject(
            this,
            &UStartupWidget::OnCreateSessionDone
        );

        MultiplayerSubsystem->DestroySessionDoneDelegate.AddUObject(
            this,
            &UStartupWidget::OnDestroySessionDone
        );

    }



    return true;
}

void UStartupWidget::OnCreateSessionDone(bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        ShowNotify(TEXT("Create Session Succecced!"), FLinearColor::Green);
        InputMode_Game();
    }
    else
    {
        if (Button_CreateSession)
        {
            Button_CreateSession->SetIsEnabled(true);
        }

        ShowNotify(TEXT("Create Session Failed!"), FLinearColor::Red);
    }
}

void UStartupWidget::OnDestroySessionDone(bool bWasSuccessful)
{
    if (Button_CreateSession)
    {
        Button_CreateSession->SetIsEnabled(true);
    }

    if (bWasSuccessful)
    {
        ShowNotify(TEXT("Destroy Existed Session Succecced!"), FLinearColor::Green);
    }
    else
    {
        ShowNotify(TEXT("Destroy Existed Session Failed!"), FLinearColor::Red);
    }
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

void UStartupWidget::InputMode_Game()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        bIsFocusable = false;

        PC->SetShowMouseCursor(false);

        FInputModeGameOnly InputMode_Game;
        PC->SetInputMode(InputMode_Game);
    }
}

void UStartupWidget::OnClickButton_FindSessions()
{
    ShowNotify(TEXT("OnClickButton_FindSessions"), FLinearColor::Gray);
}

void UStartupWidget::OnClickButton_CreateSession()
{
    if (Button_CreateSession)
    {
        Button_CreateSession->SetIsEnabled(false);
    }

    ShowNotify(TEXT("OnClickButton_CreateSession"), FLinearColor::Blue);

    if (MultiplayerSubsystem)
    {
        MultiplayerSubsystem->CreateSession();
    }
}
