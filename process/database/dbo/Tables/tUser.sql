CREATE TABLE [dbo].[tUser]
(
	[userID] INT NOT NULL PRIMARY KEY IDENTITY, 
    [username] NVARCHAR(50) NOT NULL, 
    [email] NVARCHAR(1024) NOT NULL, 
	[apikey] UNIQUEIDENTIFIER NULL,
	[guid] UNIQUEIDENTIFIER NULL,
    [banned] BIT NOT NULL DEFAULT 0, 
    [active] BIT NOT NULL DEFAULT 0, 
    [verified] BIT NOT NULL DEFAULT 0,      
    [created] DATETIME NOT NULL DEFAULT (getdate()), 
    [updated] DATETIME NULL,    
)
