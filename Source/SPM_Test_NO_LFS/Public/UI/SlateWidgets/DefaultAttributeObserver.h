#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "DefaultAttributeObserver.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttributeChanged);

UCLASS()
class UAttributeObserver : public UObject
{
	GENERATED_BODY()

public:
	void StartObserving();
	void StopObserving();
	float GetValue() const;
	void SetObservedAttribute(const TAttribute<float>& Attribute);

protected:
	void Update();
	void SetUpdateInterval(const float NewInterval);

public:
	FOnAttributeChanged OnAttributeChanged;

protected:
	float UpdateInterval = 1.f;
	FTimerHandle TimerHandle;
	FTimerDelegate UpdateDelegate;
	TAttribute<float> ObservedAttribute;
	float LastAttributeValue;
};

class SProgressBar;

class SDefaultAttributeObserver : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDefaultAttributeObserver){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);	
	void StartObserver(UWorld* World, const TAttribute<float>& InAttribute);

protected:
	    
	TSharedPtr<SStackBox> Container;
	TSharedPtr<STextBlock> TextBlockWidget;
	TSharedPtr<SProgressBar> ProgressBarWidget;
	TSharedPtr<FSlateBrush> ImageBrush;

	TObjectPtr<UAttributeObserver>  ObservedAttribute;
};