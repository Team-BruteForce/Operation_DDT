// Component to display enemy health bar in world

#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/UI/CEnemyHealthBarWidget.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
// No polling dependencies; event-driven visibility

UCEnemyHealthBarComponent::UCEnemyHealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCEnemyHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner == nullptr)
		return;

	StatusComponent = Owner->FindComponentByClass<UCEnemyStatusComponent>();

	// Create WidgetComponent dynamically if not already present
	WidgetComponent = Owner->FindComponentByClass<UWidgetComponent>();
	if (WidgetComponent == nullptr)
	{
		WidgetComponent = NewObject<UWidgetComponent>(Owner, UWidgetComponent::StaticClass(), TEXT("EnemyHealthBarWidget"));
		if (WidgetComponent)
		{
			WidgetComponent->RegisterComponent();
			WidgetComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			WidgetComponent->SetRelativeLocation(RelativeOffset);
			WidgetComponent->SetDrawSize(DrawSize);
		}
	}

	if (HealthBarWidgetClass == nullptr)
	{
		HealthBarWidgetClass = UCEnemyHealthBarWidget::StaticClass();
	}

	if (WidgetComponent)
	{
		WidgetComponent->SetWidgetClass(HealthBarWidgetClass);
		WidgetComponent->InitWidget();
		// 처음에는 항상 비가시화
		WidgetComponent->SetVisibility(false);
	}

	if (StatusComponent)
	{
		StatusComponent->OnHealthChanged.AddDynamic(this, &UCEnemyHealthBarComponent::HandleHealthChanged);
		StatusComponent->OnMaxHealthChanged.AddDynamic(this, &UCEnemyHealthBarComponent::HandleMaxHealthChanged);
	}

	UpdateWidgetPercent();

	// 초기는 숨김 유지
	HideHealthBar();
}

void UCEnemyHealthBarComponent::HandleHealthChanged(float NewHealth)
{
	UpdateWidgetPercent();
}

void UCEnemyHealthBarComponent::HandleMaxHealthChanged(float NewMaxHealth)
{
	UpdateWidgetPercent();
}

void UCEnemyHealthBarComponent::UpdateWidgetPercent()
{
	if (WidgetComponent == nullptr)
		return;

	UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject();
	UCEnemyHealthBarWidget* HealthWidget = Cast<UCEnemyHealthBarWidget>(UserWidget);
	if (HealthWidget == nullptr)
		return;

	float Percent = 1.0f;
	if (StatusComponent)
	{
		Percent = StatusComponent->GetHealthPercent();
	}

	HealthWidget->SetHealthPercent(Percent, FrontDecreaseSpeed, GhostDecreaseSpeed);
}

void UCEnemyHealthBarComponent::ShowHealthBar()
{
	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(true);
	}
}

void UCEnemyHealthBarComponent::HideHealthBar()
{
	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(false);
	}
}


