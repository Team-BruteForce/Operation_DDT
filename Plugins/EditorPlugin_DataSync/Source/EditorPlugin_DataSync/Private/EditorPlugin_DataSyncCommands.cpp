// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorPlugin_DataSyncCommands.h"

#define LOCTEXT_NAMESPACE "FEditorPlugin_DataSyncModule"

void FEditorPlugin_DataSyncCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorPlugin_DataSync", "Bring up EditorPlugin_DataSync window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
