CREATE TABLE [dbo].[tComponent] (
    [ComponentID] UNIQUEIDENTIFIER NOT NULL,
    [LineID]      UNIQUEIDENTIFIER             NULL,
    [Name]        NVARCHAR (1024) NULL,
    CONSTRAINT [PK_tComponent] PRIMARY KEY CLUSTERED ([ComponentID] ASC)
);

