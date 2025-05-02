// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCursorWidget.h"
#include "Components/Image.h"

void UDefaultCursorWidget::ShowDefaultCursor()
{
	img_defaultCursor->SetVisibility(ESlateVisibility::Visible);
	img_handCursor->SetVisibility(ESlateVisibility::Hidden);
}

void UDefaultCursorWidget::ShowHandCursor()
{
	img_defaultCursor->SetVisibility(ESlateVisibility::Hidden);
	img_handCursor->SetVisibility(ESlateVisibility::Visible);
}
