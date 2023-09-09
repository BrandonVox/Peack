// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CharacterWidget.h"

#include "Components/ProgressBar.h"

void UCharacterWidget::UpdateBar_Health(float Health, float MaxHealth)
{
	if (Bar_Health)
	{
		Bar_Health->SetPercent(Health / MaxHealth);
	}
}
