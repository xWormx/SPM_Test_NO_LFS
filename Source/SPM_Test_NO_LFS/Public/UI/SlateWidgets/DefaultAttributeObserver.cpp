#include "DefaultAttributeObserver.h"

#include "SPM_Test_NO_LFS.h"
#include "Widgets/Notifications/SProgressBar.h"

void UAttributeObserver::StartObserving()
{
	UpdateDelegate = FTimerDelegate::CreateUObject(this, &UAttributeObserver::Update);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, UpdateDelegate, UpdateInterval, true);
}

void UAttributeObserver::StopObserving()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UAttributeObserver::Update()
{
	if (!ObservedAttribute.IsSet())
	{
		return;
	}
	if (LastAttributeValue != ObservedAttribute.Get())
	{
		LastAttributeValue = ObservedAttribute.Get();
		OnAttributeChanged.Broadcast();
	}
}

void UAttributeObserver::SetUpdateInterval(const float NewInterval)
{
	UpdateInterval = NewInterval;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, UpdateDelegate, UpdateInterval, true);
}

float UAttributeObserver::GetValue() const
{
	return ObservedAttribute.Get();
}

void UAttributeObserver::SetObservedAttribute(const TAttribute<float>& Attribute)
{
	ObservedAttribute = Attribute;
	if (ObservedAttribute.IsSet())
	{
		LastAttributeValue = ObservedAttribute.Get();
	}
	else
	{
		LastAttributeValue = 0;
	}
}


void SDefaultAttributeObserver::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(Container, SStackBox)
		.Orientation(Orient_Horizontal)
		+ SStackBox::Slot()
		.AutoSize()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		.Padding(2)
		[
			SAssignNew(TextBlockWidget, STextBlock)			
			.ColorAndOpacity(FLinearColor::White)
		]
		+ SStackBox::Slot()
		.AutoSize()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		.Padding(2)
		[
			SAssignNew(ProgressBarWidget, SProgressBar)			
			.Style(FCoreStyle::Get(), "ProgressBar")
			.BackgroundImage(FCoreStyle::Get().GetBrush("ProgressBar.Background"))
			.FillImage(FCoreStyle::Get().GetBrush("ProgressBar.Fill"))
		]
	];

	Container->SetOrientation(Orient_Horizontal);	
}

void SDefaultAttributeObserver::StartObserver(UWorld* World, const TAttribute<float>& InAttribute)
{
	if (!World)
	{
		EMMA_LOG(Error, TEXT("SDefaultAttributeObserver::StartObserver: World is null"));
		return;
	}

	ObservedAttribute = NewObject<UAttributeObserver>(World);
	ObservedAttribute->SetObservedAttribute(InAttribute);
	ObservedAttribute->OnAttributeChanged.AddLambda([this]
	{
		float Value = ObservedAttribute->GetValue();
		if (TextBlockWidget.IsValid())
		{
			TextBlockWidget->SetText(FText::FromString(FString::Printf(TEXT("Value: %.2f"), Value)));
		}
		if (ProgressBarWidget.IsValid())
		{
			ProgressBarWidget->SetPercent(Value / 100.f);
		}
	});
	ObservedAttribute->StartObserving();

	TextBlockWidget->SetText( FText::FromString(FString::Printf(TEXT("Value: %.2f"), ObservedAttribute->GetValue())));
	ProgressBarWidget->SetPercent( ObservedAttribute->GetValue() / 100.f);
}