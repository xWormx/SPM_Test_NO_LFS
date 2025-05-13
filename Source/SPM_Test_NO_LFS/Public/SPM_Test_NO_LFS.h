// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//--------- LOG  -----------//

DECLARE_LOG_CATEGORY_EXTERN(CLogEmma, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(CLogBasir, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(CLogCalle, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(CLogJoel, Log, All);

#define EMMA_LOG(Severity, Text, ...) UE_LOG(CLogEmma, Severity, Text, ##__VA_ARGS__)
#define BASIR_LOG(Severity, Text, ...) UE_LOG(CLogBasir, Severity, Text, ##__VA_ARGS__)
#define CALLE_LOG(Severity, Text, ...) UE_LOG(CLogCalle, Severity, Text, ##__VA_ARGS__)
#define JOEL_LOG(Severity, Text, ...) UE_LOG(CLogJoel, Severity, Text, ##__VA_ARGS__)

//-------- PRINT TO SCREEN -----------//

extern TAutoConsoleVariable<bool> CVarDebugEmma;
extern TAutoConsoleVariable<bool> CVarDebugBasir;
extern TAutoConsoleVariable<bool> CVarDebugCalle;
extern TAutoConsoleVariable<bool> CVarDebugJoel;

template <typename T>
void DebugMessage(const TAutoConsoleVariable<T>& CVar, const FString& Prefix, const FString& Message,const FColor& Color, float Duration)
{
	if (CVar.GetValueOnGameThread() && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(TEXT("[%s] %s"), *Prefix, *Message));
	}
}

#define EMMA_DEBUG(Message, Color, Duration) DebugMessage(CVarDebugEmma, TEXT("LogEmma"), Message, Color, Duration);
#define BASIR_DEBUG(Message, Color, Duration) DebugMessage(CVarDebugBasir, TEXT("LogBasir"), Message, Color, Duration);
#define CALLE_DEBUG(Message, Color, Duration) DebugMessage(CVarDebugCalle, TEXT("LogCalle"), Message, Color, Duration);
#define JOEL_DEBUG(Message, Color, Duration) DebugMessage(CVarDebugJoel, TEXT("LogJoel"), Message, Color, Duration);

