RocketMq 支持什么级别的延迟消息？如何实现的？
RocketMQ 提供了延迟消息类型，这意味着生产者指定消息发送的延迟时间，然后才能将消息传递给消费者。

Broker 检查收到的消息是否延迟。如果它们是延迟消息，代理将使用 SCHEDULE_TOPIC_XXXX作为 持久主题名。实现方法在 CommitLog的putMessage 中。如果 msg 延迟标准大于 0，则重新设置消息的主题名和队列 id。之后，将消息的 SCHEDULE_TOPIC_XXXX 作为主题，并将延迟时间的 queueId 持久化到提交日志文件中。

Broker 持久化一个名称为 SCHEDULE_TOPIC_XXXX 的延迟消息作为主题。让我们看看代理如何在延迟消息到达后恢复消息。

ScheduleMessageService RocketMQ 提供 ScheduleMessageService 服务。SCHEDULE_TOPIC_XXXX 和 queueId 从 SCHEDULE_TOPIC_XXXX 中读取，用于消息恢复。消息恢复后，使用者可以提取消息。

一个定时任务 DeliverDelayedMessageTimerTask 每个消费级别。在 DeliverDelayedMessageTimerTask 中的名称和时间延迟根据 SCHEDULE_TOPIC_XXXX 级别对应的 queueId 访问消息队列，然后从提交日志中读取消息，恢复消息的原始信息(消息的原始主题信息)并将消息持久化到提交日志文件中，以便消费者可以提取消息。
 