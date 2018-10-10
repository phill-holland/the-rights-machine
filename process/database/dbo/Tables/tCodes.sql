CREATE TABLE [dbo].[tCode]
(
	[CodeID] INT NOT NULL PRIMARY KEY IDENTITY, 
    [Name] NVARCHAR(50) NOT NULL, 
    [Description] NVARCHAR(200) NOT NULL, 
    [Created] DATETIME NOT NULL DEFAULT (getdate())
)
