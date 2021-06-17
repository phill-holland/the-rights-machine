CREATE TABLE [dbo].[tLine] (
    [LineID]        UNIQUEIDENTIFIER NOT NULL,
    [ItemID]        UNIQUEIDENTIFIER      NULL,
    [StartDate]     DATETIME NULL,
    [EndDate]       DATETIME NULL,
    [ExclusivityID] INT      NULL,
    [TypeID]        INT      NULL,
    CONSTRAINT [PK_tLine] PRIMARY KEY CLUSTERED ([LineID] ASC)
);

