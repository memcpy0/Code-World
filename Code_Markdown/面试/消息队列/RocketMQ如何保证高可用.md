RocketMQ 分布式集群是通过 Master 和 Slave 的配合达到高可用性的。

Master 和 Slave 的区别：在 Broker 的配置文件中，参数 brokerId 的值为 0 表明这个 Broker 是 Master，大于 0 表明这个 Broker 是 Slave，同时 brokerRole 参数也会说明这个 Broker 是 Master 还是 Slave。

Master 角色的 Broker 支持读和写，Slave 角色的 Broker 仅支持读，也就是 Producer 只能和Master 角色的 Broker 连接写入消息；Consumer 可以连接 Master 角色的 Broker，也可以连接Slave 角色的 Broker 来读取消息。

消息消费的高可用性
在 Consumer 配置文件中，不需要设置从 Master 或 Slave 读取的数据。当主服务器不可用或繁忙时，消费服务器将自动切换到从服务器。使用 Consumer 的自动切换机制，当 Master 的机器出现故障时，Consumer 仍然可以从 Slave 读取消息，而不影响Consumer程序。这是消费者端的高可用性。

消息发送的高可用性
当创建一个Topic时，将在多个 Broker 组上创建该 Topic 的消息队列(具有相同Broker 名称和不同 Brokerid 的机器组成一个 Broker 组)。这样，当一个 Broker 组的 Master 不可用时，另一个组的 Master 仍然可用，Producer 仍然可以发送消息。RocketMQ 目前不支持从服务器到主服务器的自动转换。如果由于机器资源不足需要将 Slave 转换为 Master，请手动停止 Slave Broker，更改配置文件，并使用新的配置文件启动 Broker。

消息主从复制
如果 Broker 组有一个 Master 和 Slave，则需要将消息从 Master 复制到 Slave(同步和异步)。

作者：LeetCode
链接：https://leetcode.cn/leetbook/read/7-day-interview-hou-duan/d07exv/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。