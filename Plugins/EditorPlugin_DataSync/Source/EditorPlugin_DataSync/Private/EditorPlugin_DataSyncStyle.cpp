// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorPlugin_DataSyncStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FEditorPlugin_DataSyncStyle::StyleInstance = nullptr;

void FEditorPlugin_DataSyncStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FEditorPlugin_DataSyncStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FEditorPlugin_DataSyncStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("EditorPlugin_DataSyncStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FEditorPlugin_DataSyncStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("EditorPlugin_DataSyncStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("EditorPlugin_DataSync")->GetBaseDir() / TEXT("Resources"));

	Style->Set("EditorPlugin_DataSync.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FEditorPlugin_DataSyncStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FEditorPlugin_DataSyncStyle::Get()
{
	return *StyleInstance;
}
