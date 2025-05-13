// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPM_Test_NO_LFS.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, SPM_Test_NO_LFS, "SPM_Test_NO_LFS");

//--------- LOG  -----------//

DEFINE_LOG_CATEGORY(CLogEmma);
DEFINE_LOG_CATEGORY(CLogBasir);
DEFINE_LOG_CATEGORY(CLogCalle);
DEFINE_LOG_CATEGORY(CLogJoel);

//-------- PRINT TO SCREEN -----------//

TAutoConsoleVariable<bool> CVarDebugEmma(TEXT("Debug.Emma.Enabled"), true, TEXT("Enable/disable debug messages for Emma"), ECVF_Default);
TAutoConsoleVariable<bool> CVarDebugBasir(TEXT("Debug.Basir.Enabled"), true, TEXT("Enable/disable debug messages for Basir"), ECVF_Default);
TAutoConsoleVariable<bool> CVarDebugCalle(TEXT("Debug.Calle.Enabled"), true, TEXT("Enable/disable debug messages for Calle"), ECVF_Default);
TAutoConsoleVariable<bool> CVarDebugJoel(TEXT("Debug.Joel.Enabled"), true, TEXT("Enable/disable debug messages for Joel"), ECVF_Default);

#define REGISTER_DEBUG_COMMAND(Category) \
static FAutoConsoleCommand Debug##Category##Cmd( \
    TEXT("Debug." #Category ".Message"), \
    TEXT("Display a debug message in the " #Category " category: Debug." #Category ".Message \"Your message\" [optional: red|green|blue|yellow|white]"), \
    FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args) \
    { \
        if (Args.Num() >= 1) \
        { \
            FString Message = Args[0]; \
            FColor Color = FColor::White; \
            if (Args.Num() >= 2) \
            { \
				FString ColorName = Args[1].ToLower(); \
				if (ColorName == "red") Color = FColor::Red; \
				else if (ColorName == "green") Color = FColor::Green; \
				else if (ColorName == "blue") Color = FColor::Blue; \
				else if (ColorName == "yellow") Color = FColor::Yellow; \
				else if (ColorName == "black") Color = FColor::Black; \
				else if (ColorName == "cyan") Color = FColor::Cyan; \
				else if (ColorName == "magenta") Color = FColor::Magenta; \
				else if (ColorName == "orange") Color = FColor::Orange; \
				else if (ColorName == "turquoise") Color = FColor::Turquoise; \
				else if (ColorName == "emerald") Color = FColor::Emerald; \
            } \
            DebugMessage<bool>(CVarDebug##Category, TEXT("CLog" #Category), Message, Color, 5.0f); \
        } \
    }));

REGISTER_DEBUG_COMMAND(Emma);
REGISTER_DEBUG_COMMAND(Basir);
REGISTER_DEBUG_COMMAND(Calle);
REGISTER_DEBUG_COMMAND(Joel);
