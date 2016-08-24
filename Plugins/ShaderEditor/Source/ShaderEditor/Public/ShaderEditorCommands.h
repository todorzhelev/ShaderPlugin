// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "ShaderEditorStyle.h"

class FShaderEditorCommands : public TCommands<FShaderEditorCommands>
{
public:

	FShaderEditorCommands()
		: TCommands<FShaderEditorCommands>(TEXT("ShaderEditor"), NSLOCTEXT("Contexts", "ShaderEditor", "ShaderEditor Plugin"), NAME_None, FShaderEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};