CREATE TABLE [dbo].[tElement]
(
	[ElementID] INT NOT NULL PRIMARY KEY IDENTITY, 
    [ComponentID] INT NOT NULL, 
    [Value] NVARCHAR(50) NOT NULL
)
