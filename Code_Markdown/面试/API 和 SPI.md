API 是 应用编程接口(Application Programing Interface) , 是为调用方提供服务或函数的一种方法。 SPI 是 服务提供接口(Service Provider Interface)， 是为第三方扩展功能的一种方法。

API 是系统与外部交流的窗口。SPI 是系统为第三方留的后门。

很多时候一个应用可能既是client 也是 provider，那么API 与SPI 的分界就很模糊，我们需要从 API 与 SPI 的不同属性上来区分。

API 的属性

-   提供给调用方某种能力
-   添加 API 对调用方没有影响
-   修改或删除 API 影响很大
-   用来调用

SPI 的属性

-   提供给服务方扩展系统能力的能力
-   SPI 的删除对调用方没有影响，只会影响服务方
-   SPI 的添加可能会产生问题（多了一步拦截）
-   用来实现