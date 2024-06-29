Dubbo 的底层 I/O 操作都是异步的。当消费者被调用时，得到一个 Future 对象。对于同步调用，服务线程使用 Future#get(timeout) 来阻塞并等待提供者返回结果。timeout 是消费者定义的超时。当返回结果时，设置Future并唤醒阻塞的业务线程。如果在超时时间后没有返回结果，业务线程将返回一个异常。

基于底层 Dubbo 的异步 NIO 来实现异步调用是生产者响应时间较长的场景所必需的，它可以有效地利用消费者端的资源，相比于消费者端使用多线程的成本较小。

无论是同步的还是异步的，将以 exchang# send 方法结束，然后将以HeaderExchangeChannel#request 方法结束，这是一个异步方法并返回 ResponseFuture 对象。

dubbo 中的同步调用也是通过异步调用实现的，但是在启动同步调用之后，直接调用 future#get 方法来同步等待结果的返回，而异步调用只返回 future Response，并在用户需要关心结果时调用 get 方法。



作者：LeetCode
链接：https://leetcode.cn/leetbook/read/7-day-interview-hou-duan/d00p5g/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。