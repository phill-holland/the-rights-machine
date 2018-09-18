CREATE TABLE [dbo].[tMessage] (
    [MessageID]   INT              IDENTITY (1, 1) NOT NULL,
    [User]      NVARCHAR(50)              NOT NULL,    
    [GUID]        UNIQUEIDENTIFIER NOT NULL,
    [APIKey] NVARCHAR(1024) NOT NULL, 
    [Created] DATETIME         CONSTRAINT [DF_tMessage_CreatedDate] DEFAULT (getdate()) NOT NULL,
	[Finished] DATETIME NULL, 
    CONSTRAINT [PK_tMessage] PRIMARY KEY CLUSTERED ([MessageID] ASC)
);

