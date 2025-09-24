// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/CPauseWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/DDTPlayer.h"

void UCPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UnhoverBtn();
	if (btn_Yes)
	{
		btn_Yes->OnHovered.AddDynamic(this, &UCPauseWidget::OnHoverYesBtn);
		btn_Yes->OnUnhovered.AddDynamic(this, &UCPauseWidget::UnhoverBtn);
		btn_Yes->OnClicked.AddDynamic(this, &UCPauseWidget::OnClickYesBtn);
	}
	if (btn_No)
	{
		btn_No->OnHovered.AddDynamic(this, &UCPauseWidget::OnHoverNoBtn);
		btn_No->OnUnhovered.AddDynamic(this, &UCPauseWidget::UnhoverBtn);
		btn_No->OnClicked.AddDynamic(this, &UCPauseWidget::OnClickNoBtn);
	}

	Player = Cast<ADDTPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PC = Cast<APlayerController>(Player->GetController());
	
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
	
	SetKeyboardFocus();
	SetIsFocusable(true);
}

void UCPauseWidget::OnHoverYesBtn()
{
	img_Normal->SetVisibility(ESlateVisibility::Hidden);
	img_Yes->SetVisibility(ESlateVisibility::Visible);
	img_No->SetVisibility(ESlateVisibility::Hidden);
}

void UCPauseWidget::OnHoverNoBtn()
{
	img_Normal->SetVisibility(ESlateVisibility::Hidden);
	img_Yes->SetVisibility(ESlateVisibility::Hidden);
	img_No->SetVisibility(ESlateVisibility::Visible);
}

void UCPauseWidget::UnhoverBtn()
{
	img_Normal->SetVisibility(ESlateVisibility::Visible);
	img_Yes->SetVisibility(ESlateVisibility::Hidden);
	img_No->SetVisibility(ESlateVisibility::Hidden);
}
void UCPauseWidget::OnClickYesBtn()
{
	RemoveFromParent();
	
	UKismetSystemLibrary::QuitGame(Player->GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UCPauseWidget::OnClickNoBtn()
{
	SetIsFocusable(false);
	
	RemoveFromParent();
	
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = false;
	PC->SetPause(false);
}

FReply UCPauseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		OnClickNoBtn();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
