// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SessionItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool USessionItemWidget::Initialize()
{
    if (Super::Initialize() == false)
    {
        return false;
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

void USessionItemWidget::OnClickButton_Join()
{

}
