CREATE TABLE [dbo].[tComponent] (
    [ComponentID] INT             IDENTITY (1, 1) NOT NULL,
    [LineID]      INT             NULL,
    [Name]        NVARCHAR (1024) NULL,
    CONSTRAINT [PK_tComponent] PRIMARY KEY CLUSTERED ([ComponentID] ASC)
);

