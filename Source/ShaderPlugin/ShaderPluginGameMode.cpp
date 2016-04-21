// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ShaderPlugin.h"
#include "ShaderPluginGameMode.h"
#include "ShaderPluginHUD.h"
#include "ShaderPluginCharacter.h"

AShaderPluginGameMode::AShaderPluginGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AShaderPluginHUD::StaticClass();
}
