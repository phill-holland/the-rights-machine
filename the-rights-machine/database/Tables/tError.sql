CREATE TABLE [dbo].[tError]
(
	[ErrorID] UNIQUEIDENTIFIER NOT NULL PRIMARY KEY, 
    [Code] INT NOT NULL, 
    [Name] NCHAR(50) NOT NULL,
	[Description] NVARCHAR(200) NOT NULL, 
    [Created] DATETIME NOT NULL DEFAULT (getdate())    
)
