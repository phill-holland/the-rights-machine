CREATE TABLE `tResponse` (
  `ResponseID` NVARCHAR(37) NOT NULL,
  `User` NVARCHAR(37) NOT NULL,
  `Status` INT NOT NULL,
  `Created` DATETIME NOT NULL, 
  `Available` BOOLEAN NOT NULL
   PRIMARY KEY (`ResponseID`)
);

