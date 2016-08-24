// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ShaderEditorPrivatePCH.h"
#include "ShaderEditorCommands.h"

#define LOCTEXT_NAMESPACE "FShaderEditorModule"

void FShaderEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ShaderEditor", "Bring up ShaderEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
