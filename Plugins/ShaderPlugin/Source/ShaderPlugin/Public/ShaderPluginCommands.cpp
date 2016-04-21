// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ShaderPluginPrivatePCH.h"
#include "ShaderPluginCommands.h"

#define LOCTEXT_NAMESPACE "FShaderPluginModule"

void FShaderPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ShaderPlugin", "Bring up ShaderPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
