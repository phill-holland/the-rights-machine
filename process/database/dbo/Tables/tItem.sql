CREATE TABLE [dbo].[tItem] (
    [ItemID]    INT             IDENTITY (1, 1) NOT NULL,
    [MessageID] INT             NULL,
    [Name]      NVARCHAR (1024) NULL,
    CONSTRAINT [PK_tItem] PRIMARY KEY CLUSTERED ([ItemID] ASC)
);

