create proc dbo.pEraseMessagesWithTag(@tag uniqueidentifier)
as
begin
begin transaction;

delete from tElement where tElement.ElementID IN
(select tElement.ElementID from tElement
left join tComponent on tElement.ComponentID = tComponent.ComponentID
left join tLine on tComponent.ID = tLine.LineID
left join tItem on tLine.ItemID = tItem.ItemID
left join tMessage on tItem.MessageID = tMessage.MessageID
WHERE tComponent.[Type]=1 AND tMessage.Tag = @tag);

delete from tComponent where tComponent.ComponentID IN
(select tComponent.ComponentID from tComponent
left join tLine on tComponent.ID = tLine.LineID
left join tItem on tLine.ItemID = tItem.ItemID
left join tMessage on tItem.MessageID = tMessage.MessageID
WHERE tComponent.[Type]=1 AND tMessage.Tag = @tag);

delete from tLine where tLine.LineID IN
(select tLine.LineID from tLine
left join tItem on tLine.ItemID = tItem.ItemID
left join tMessage on tItem.MessageID = tMessage.MessageID
WHERE tMessage.Tag = @tag);

delete from tItem where tItem.ItemID IN
(select tItem.ItemID from tItem
left join tMessage on tItem.MessageID = tMessage.MessageID
WHERE tMessage.Tag = @tag);

/* Query Delete */

delete from tElement where tElement.ElementID IN
(select tElement.ElementID from tElement
left join tComponent on tElement.ComponentID = tComponent.ComponentID
left join tQuery on tComponent.ID = tQuery.QueryID
left join tMessage on tQuery.MessageID = tMessage.MessageID
WHERE tComponent.[Type]=2 AND tMessage.Tag = @tag);

delete from tComponent where tComponent.ComponentID IN
(select tComponent.ComponentID from tComponent
left join tQuery on tComponent.ID = tQuery.QueryID
left join tMessage on tQuery.MessageID = tMessage.MessageID
WHERE tComponent.[Type]=2 AND tMessage.Tag = @tag);

delete from tQuery where tQuery.MessageID IN
(select tMessage.MessageID from tMessage
WHERE tMessage.Tag = @tag);

delete from tMessage where tMessage.Tag = @tag;

commit transaction;
end;