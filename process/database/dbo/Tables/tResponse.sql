CREATE TABLE [dbo].[tResponse] (
    [ResponseID]  INT             IDENTITY (1, 1) NOT NULL,
    [ItemID]      INT             NULL,
    [Code]        INT             NULL,
    [Description] NVARCHAR (1024) NULL,
    [CreatedDate] DATETIME        CONSTRAINT [DF_tResponse_CreatedDate] DEFAULT (getdate()) NULL,
    CONSTRAINT [PK_tResponse] PRIMARY KEY CLUSTERED ([ResponseID] ASC)
);

