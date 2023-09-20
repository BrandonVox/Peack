// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StartupWidget.h"

bool UStartupWidget::Initialize()
{
    if (Super::Initialize() == false)
    {
        return false;
    }

    // Input Mode UI
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetShowMouseCursor(true);

        FInputModeUIOnly InputMode_UI;
        InputMode_UI.SetWidgetToFocus(TakeWidget());

        PC->SetInputMode(InputMode_UI);
    }

    return true;
}
