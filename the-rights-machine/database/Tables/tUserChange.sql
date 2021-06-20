CREATE TABLE [dbo].[tUserChange]
(
	[userChangeID] INT NOT NULL PRIMARY KEY IDENTITY, 
    [userID] INT NOT NULL
)


CREATE TABLE `tUserChange` (
  `UserChangeID` INT NOT NULL AUTO_INCREMENT,
  `UserID` NVARCHAR(37) NOT NULL
   PRIMARY KEY (`UserChangeID`)
);

