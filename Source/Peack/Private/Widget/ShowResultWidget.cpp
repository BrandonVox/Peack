

#include "Widget/ShowResultWidget.h"

#include "Components/TextBlock.h"

void UShowResultWidget::UpdateText_WinnerName(const FString& WinnerNameString)
{
	if (Text_WinnerName)
	{
		Text_WinnerName->SetText(FText::FromString(WinnerNameString));
	}
}
