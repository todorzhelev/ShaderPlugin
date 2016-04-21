// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "ShaderPluginStyle.h"

class FShaderPluginCommands : public TCommands<FShaderPluginCommands>
{
public:

	FShaderPluginCommands()
		: TCommands<FShaderPluginCommands>(TEXT("ShaderPlugin"), NSLOCTEXT("Contexts", "ShaderPlugin", "ShaderPlugin Plugin"), NAME_None, FShaderPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};