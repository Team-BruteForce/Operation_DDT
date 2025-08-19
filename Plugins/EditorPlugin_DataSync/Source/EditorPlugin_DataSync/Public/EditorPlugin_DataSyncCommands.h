// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "EditorPlugin_DataSyncStyle.h"

class FEditorPlugin_DataSyncCommands : public TCommands<FEditorPlugin_DataSyncCommands>
{
public:

	FEditorPlugin_DataSyncCommands()
		: TCommands<FEditorPlugin_DataSyncCommands>(TEXT("EditorPlugin_DataSync"), NSLOCTEXT("Contexts", "EditorPlugin_DataSync", "EditorPlugin_DataSync Plugin"), NAME_None, FEditorPlugin_DataSyncStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};