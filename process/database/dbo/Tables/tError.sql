CREATE TABLE [dbo].[tError]
(
	[ErrorID] UNIQUEIDENTIFIER NOT NULL PRIMARY KEY, 
    [Code] INT NOT NULL, 
    [Description] NVARCHAR(256) NOT NULL, 
    [Created] DATETIME NOT NULL DEFAULT (getdate())
)
