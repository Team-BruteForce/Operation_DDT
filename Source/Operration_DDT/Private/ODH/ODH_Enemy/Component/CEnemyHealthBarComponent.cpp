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

	// Create/Find dedicated WidgetComponent named "EnemyHealthBarWidget"
	WidgetComponent = nullptr;
	{
		TInlineComponentArray<UWidgetComponent*> WidgetComponents;
		Owner->GetComponents<UWidgetComponent>(WidgetComponents);
		for (UWidgetComponent* Comp : WidgetComponents)
		{
			if (Comp && Comp->GetName() == TEXT("EnemyHealthBarWidget"))
			{
				WidgetComponent = Comp;
				break;
			}
		}
	}
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
		// 처음에는 항상 비가시화 (컴포넌트+위젯 동기화)
		if (UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject())
		{
			UserWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		WidgetComponent->SetHiddenInGame(true);
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
	
	// 에너미가 죽었다면 HP바를 숨김
	if (StatusComponent && StatusComponent->IsDead())
	{
		HideHealthBar();
	}
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
		// 컴포넌트와 위젯 양쪽 가시성 동기화
		if (UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject())
		{
			UserWidget->SetVisibility(ESlateVisibility::Visible);
		}
		WidgetComponent->SetHiddenInGame(false);
		WidgetComponent->SetVisibility(true);
	}
}

void UCEnemyHealthBarComponent::HideHealthBar()
{
	if (WidgetComponent)
	{
		// 컴포넌트와 위젯 양쪽 가시성 동기화
		if (UUserWidget* UserWidget = WidgetComponent->GetUserWidgetObject())
		{
			UserWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		WidgetComponent->SetHiddenInGame(true);
		WidgetComponent->SetVisibility(false);
	}
}


