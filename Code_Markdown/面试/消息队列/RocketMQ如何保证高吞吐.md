RocketMq 如何保证高吞吐的？
路由中心（NameServer）
NameServer 通过集群部署，为追求简单高效 NameServer 彼此之间互不通信。
NameServer 与每台 Broker 服务器保持长连接，并间隔 30s 检测 Broker 是否存活，如果检测到 Broker 宕机，则从路由注册表中将其移除。但是路由变化不会马上通知消息生产者，降低NameServer 实现的复杂性，在消息发送端提供容错机制来保证消息发送的高可用性。
消息生产者（Producer）
消息生产者集群部署。
本地缓存 topic 路由信息，如果本地路由表中未缓存 topic 的路由信息，向 Name­Server 发送获取路由信息请求，更新本地路由信息表，并且消息生产者每隔 30s 从 Name­Server 更新路由表。
消息发送通过负载均衡策略选择 Broker，避免单个服务器压力过大。
消息发送异常机制，消息发送高可用主要通过两个手段：重试与 Broker 规避。Broker 规避就是在一次消息发送过程中发现错误，在某一时间段内，消息生产者不会选择该 Broker 上的消息队列，提高发送消息的成功率。
批量消息发送，支持将同一主题下的多条消息一次性发送到消息服务端。
消息消费者（Consumer）
消息消费者集群部署。
消费者消费消息时负载均衡。由 RebalanceService 线程默认每隔 20s 进行一次消息队列负载，根据当前消费组内消费者个数与主题队列数量按照某一种负载算法进行队列分配，分配原则为同一个消费者可以分配多个消息消费队列，同一个消息消费队列同一时间只会分配给一个消费者。
消息拉取由 PullMessageService 线程根据 RebalanceService 线程创建的拉取任务进行拉取，默认一批拉取 32 条消息，提交给消费者消费线程池后继续下一次的消息拉取。如果消息消费过慢产生消息堆积会触发消息消费拉取流控。
服务器（Broker）
零拷贝技术。
服务端的高并发读写主要利用 Linux 操作系统的 PageCache 特性，通过 Java 的 MappedByteBuffer 直接操作PageCache。MappedByteBuffer 能直接将文件映射到内存，通过顺序写盘（Commitlog），预读数据来尽量命中 PageCahe，从而大大减少磁盘 IO。Commitlog，消息存储文件，RocketMQ 为了保证消息发送的高吞吐量，采用单一文件存储所有主题的消息，保证消息存储是完全的顺序写，但这样给文件读取同样带来了不便，为此 RocketMQ 为了方便消息消费构建了消息消费队列文件（ConsumerQueue），基于主题与队列进行组织，同时 RocketMQ 为消息实现了 Hash 索引文件（IndexFile），可以为消息设置索引键，根据索引能够快速从 Commitog 文件中检索消息。当消息到达 Commitlog 文件后，会通过ReputMessageService 线程接近实时地将消息转发给消息消费队列文件与索引文件。为了安全起见，RocketMQ 引人 abort 文件，记录 Broker 的停机是正常关闭还是异常关闭，在重启 Broker 时为了保证 Commitlog 文件、消息消费队列文件与 Hash 索引文件的正确性，分别采取不同的策略来恢复文件。

 