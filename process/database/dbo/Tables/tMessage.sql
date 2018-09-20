CREATE TABLE [dbo].[tMessage] (
    [MessageID]   UNIQUEIDENTIFIER NOT NULL,
    [User]      UNIQUEIDENTIFIER              NOT NULL,    
    [GUID]        UNIQUEIDENTIFIER NOT NULL,
    [APIKey] UNIQUEIDENTIFIER NOT NULL, 
    [Created] DATETIME          NOT NULL,
	[Finished] DATETIME NULL, 
    CONSTRAINT [PK_tMessage] PRIMARY KEY CLUSTERED ([MessageID] ASC)
);

