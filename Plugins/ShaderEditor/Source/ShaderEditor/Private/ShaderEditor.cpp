// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ShaderEditorPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "ShaderEditorStyle.h"
#include "ShaderEditorCommands.h"

#include "LevelEditor.h"

#include "Utils.h"
#include "ShaderCompiler.h"

static const FName ShaderEditorTabName("ShaderEditor");

#define LOCTEXT_NAMESPACE "FShaderEditorModule"

void FShaderEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FShaderEditorStyle::Initialize();
	FShaderEditorStyle::ReloadTextures();

	FShaderEditorCommands::Register();

	m_PluginCommands = MakeShareable(new FUICommandList);

	m_PluginCommands->MapAction(
		FShaderEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FShaderEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, m_PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FShaderEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, m_PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FShaderEditorModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ShaderEditorTabName, FOnSpawnTab::CreateRaw(this, &FShaderEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestPluginTabTitle", "TestPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FShaderEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FShaderEditorStyle::Shutdown();

	FShaderEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ShaderEditorTabName);
}

TSharedRef<SDockTab> FShaderEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	m_textboxContent = GetListOfAllShaders();

	m_multiLineTextbox = SNew(SMultiLineEditableTextBox);
	m_multiLineTextbox->SetText(FText::FromString(m_textboxContent));

	m_multiLineTextbox->SetForegroundColor(Utils::Black);
	m_multiLineTextbox->SetTextBoxBackgroundColor(Utils::White);


	m_viewUtility = SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(2.0f, 0.0f)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(SButton)
							.Text(LOCTEXT("List", "List all shaders"))
							.ToolTipText(LOCTEXT("ListToolTip", "Shows list of all shaders in Unreal"))
							.ContentPadding(3)
							.OnClicked_Lambda([&]()
							{
								m_textboxContent = GetListOfAllShaders();
								m_multiLineTextbox->SetText(FText::FromString(m_textboxContent));
								return FReply::Handled();
							})
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(2.0f, 0.0f)
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Left)
						[
							SNew(SButton)
							.Text(LOCTEXT("Edit", "Edit shader"))
							.ToolTipText(LOCTEXT("EditToolTip", "Edit Shader"))
							.ContentPadding(3)
							.OnClicked_Lambda([&]()
							{
								m_pathToCurrentShader = GetShaderPath(TEXT("BRDF.usf"));

								m_textboxContent = GetShaderContent(TEXT("BRDF.usf"));
								m_multiLineTextbox->SetText(FText::FromString(m_textboxContent));
								return FReply::Handled();
							})
						]
						+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f, 0.0f)
							.VAlign(VAlign_Center)
							.HAlign(HAlign_Left)
							[
								SNew(SButton)
								.Text(LOCTEXT("Save", "Save shader"))
								.ToolTipText(LOCTEXT("SaveShaderToolTip", "Save shader"))
								.ContentPadding(3)
								.OnClicked_Lambda([&]()
								{
									m_textboxContent = m_multiLineTextbox->GetText().ToString();
									FFileHelper::SaveStringToFile(m_textboxContent, *m_pathToCurrentShader);
									
									GUnrealEd->Exec(GUnrealEd->GetWorld(), TEXT("RECOMPILESHADERS CHANGED"));
									return FReply::Handled();
								})
							]
					];


	//TArray<TSharedPtr<SButton>> buttons;
	//TSharedPtr<SButton> button = SNew(SButton)
	//	.Text(LOCTEXT("omg", "omg"));
	//
	//buttons.Add(button);

	//m_listView = SNew(SListView<TSharedPtr<SButton>>)
	//	.ListItemsSource(&buttons);
	//	.OnGenerateRow_Lambda([&]() {return FReply::Handled();});

	m_view = SNew(SScrollBox)
			+ SScrollBox::Slot().Padding(5)
			[
				m_multiLineTextbox.ToSharedRef()
				//m_listView.ToSharedRef()
			];


	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
									.Label(LOCTEXT("ShaderCodeTitle", "Shader code"))
									[
										SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.AutoHeight()
										[
											m_viewUtility.ToSharedRef()
										]
										+ SVerticalBox::Slot()
										.FillHeight(1)
										[
											m_view.ToSharedRef()
										]
									];

	return SpawnedTab;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FShaderEditorModule::GetShaderContent(FString shaderFilename)
{
	FString CompleteFilePath = FPaths::EngineDir() + "Shaders/" + shaderFilename;

	FString FileData = "";
	FFileHelper::LoadFileToString(FileData, *CompleteFilePath);
	return  FileData;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FShaderEditorModule::GetShaderPath(FString shaderFilename)
{
	return FPaths::EngineDir() + "Shaders/" + shaderFilename;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FShaderEditorModule::GetListOfAllShaders()
{
	FString directoryToSearch = FPaths::EngineDir() + "Shaders";
	FString fileExtensions = TEXT("usf");

	TArray<FString> filesInDirectory = GetAllFilesInDirectory(directoryToSearch, false, fileExtensions);

	FString allShaders;

	for (auto& file : filesInDirectory)
	{
		allShaders += file + FString("\n");
	}

	return allShaders;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TArray<FString> FShaderEditorModule::GetAllFilesInDirectory(const FString directory, const bool fullPath, const FString onlyFilesWithExtension)
{
	// Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*directory, Visitor);
	TArray<FString> files;

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if file extension is required characters
		if (!onlyFilesWithExtension.IsEmpty())
		{
			if (!(FPaths::GetExtension(fileName, false).Equals(onlyFilesWithExtension, ESearchCase::IgnoreCase)))
				shouldAddFile = false;
		}

		// Add full path to results
		if (shouldAddFile)
		{
			files.Add(fullPath ? filePath : fileName);
		}
	}

	return files;
}

void FShaderEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(ShaderEditorTabName);
}

void FShaderEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FShaderEditorCommands::Get().OpenPluginWindow);
}

void FShaderEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FShaderEditorCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FShaderEditorModule, ShaderEditor)