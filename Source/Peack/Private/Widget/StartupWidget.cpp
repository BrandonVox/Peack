// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StartupWidget.h"

#include "Components/Button.h"

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

void UStartupWidget::InputMode_UI()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(true);

        FInputModeUIOnly InputMode_UI;
        InputMode_UI.SetWidgetToFocus(TakeWidget());

        PC->SetInputMode(InputMode_UI);
    }
}

void UStartupWidget::OnClickButton_FindSessions()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.0f,
            FColor::Purple,
            TEXT("OnClickButton_FindSessions")
        );
    }

}

void UStartupWidget::OnClickButton_CreateSession()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            3.0f,
            FColor::Green,
            TEXT("OnClickButton_CreateSession")
        );
    }
}
