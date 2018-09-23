CREATE TABLE [dbo].[tResponse] (
    [ResponseID]  UNIQUEIDENTIFIER NOT NULL,
    
    [Guid] UNIQUEIDENTIFIER NOT NULL,
    [User] UNIQUEIDENTIFIER NOT NULL, 
    [Status] INT NOT NULL, 
 [Created] DATETIME        CONSTRAINT [DF_tResponse_Created] DEFAULT (getdate()) NOT NULL,
    [Available] BIT NOT NULL, 
    CONSTRAINT [PK_tResponse] PRIMARY KEY CLUSTERED ([ResponseID] ASC)
);

