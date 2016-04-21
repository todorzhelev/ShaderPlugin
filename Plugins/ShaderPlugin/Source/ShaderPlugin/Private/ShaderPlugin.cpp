// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ShaderPluginPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "ShaderPluginStyle.h"
#include "ShaderPluginCommands.h"

#include "LevelEditor.h"

static const FName ShaderPluginTabName("ShaderPlugin");

#define LOCTEXT_NAMESPACE "FShaderPluginModule"

void FShaderPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FShaderPluginStyle::Initialize();
	FShaderPluginStyle::ReloadTextures();

	FShaderPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FShaderPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FShaderPluginModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FShaderPluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FShaderPluginModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ShaderPluginTabName, FOnSpawnTab::CreateRaw(this, &FShaderPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FShaderPluginTabTitle", "ShaderPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FShaderPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FShaderPluginStyle::Shutdown();

	FShaderPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ShaderPluginTabName);
}

TSharedRef<SDockTab> FShaderPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FShaderPluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("ShaderPlugin.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FShaderPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ShaderPluginTabName);
}

void FShaderPluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FShaderPluginCommands::Get().OpenPluginWindow);
}

void FShaderPluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FShaderPluginCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FShaderPluginModule, ShaderPlugin)