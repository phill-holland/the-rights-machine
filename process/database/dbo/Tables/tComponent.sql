CREATE TABLE [dbo].[tComponent] (
    [ComponentID] UNIQUEIDENTIFIER NOT NULL,
    [ID]      UNIQUEIDENTIFIER             NOT NULL,
    [Type] INT NOT NULL DEFAULT 1, 
	[Name]        NVARCHAR (128) NOT NULL
    
    CONSTRAINT [PK_tComponent] PRIMARY KEY CLUSTERED ([ComponentID] ASC)
);

