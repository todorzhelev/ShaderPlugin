// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FShaderEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	FString GetShaderContent(FString shaderFilename);
	FString GetListOfAllShaders();
	FString GetShaderPath(FString shaderFilename);

	TArray<FString> GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesWithExtension);

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:

	TSharedPtr<class FUICommandList> m_PluginCommands;
	TSharedPtr<SWidget> m_viewUtility;
	TSharedPtr<class SScrollBox> m_view;
	TSharedPtr<SMultiLineEditableTextBox> m_multiLineTextbox;
	//TSharedPtr<SListView<TSharedPtr<SButton>>> m_listView;

	FString m_textboxContent;
	FString m_pathToCurrentShader;
};