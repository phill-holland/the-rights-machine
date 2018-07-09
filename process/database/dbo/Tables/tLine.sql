CREATE TABLE [dbo].[tLine] (
    [LineID]        INT      IDENTITY (1, 1) NOT NULL,
    [ItemID]        INT      NULL,
    [StartDate]     DATETIME NULL,
    [EndDate]       DATETIME NULL,
    [ExclusivityID] INT      NULL,
    [TypeID]        INT      NULL,
    CONSTRAINT [PK_tLine] PRIMARY KEY CLUSTERED ([LineID] ASC)
);

