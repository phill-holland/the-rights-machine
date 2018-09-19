﻿CREATE TABLE [dbo].[tItem] (
    [ItemID]    UNIQUEIDENTIFIER NOT NULL,
    [MessageID] UNIQUEIDENTIFIER             NULL,
    [Name]      NVARCHAR (1024) NULL,
    CONSTRAINT [PK_tItem] PRIMARY KEY CLUSTERED ([ItemID] ASC)
);

