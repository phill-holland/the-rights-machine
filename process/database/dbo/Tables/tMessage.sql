CREATE TABLE [dbo].[tMessage] (
    [MessageID]   INT              IDENTITY (1, 1) NOT NULL,
    [UserID]      INT              NULL,
    [CreatedDate] DATETIME         CONSTRAINT [DF_tMessage_CreatedDate] DEFAULT (getdate()) NULL,
    [GUID]        UNIQUEIDENTIFIER NULL,
    CONSTRAINT [PK_tMessage] PRIMARY KEY CLUSTERED ([MessageID] ASC)
);

