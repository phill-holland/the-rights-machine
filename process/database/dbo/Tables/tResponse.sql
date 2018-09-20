CREATE TABLE [dbo].[tResponse] (
    [ResponseID]  UNIQUEIDENTIFIER NOT NULL,
    [ItemID]      UNIQUEIDENTIFIER             NULL,
    [Code]        INT             NULL,
    [Description] NVARCHAR (128) NULL,
    [CreatedDate] DATETIME        CONSTRAINT [DF_tResponse_CreatedDate] DEFAULT (getdate()) NULL,
    CONSTRAINT [PK_tResponse] PRIMARY KEY CLUSTERED ([ResponseID] ASC)
);

