### 1. 易扩展性
有什么方式，能给代码留条活路？
其实学会写代码并不难，但学会写好代码却很难。
- 从易阅读上来说你的代码要有准确的命名和清晰的注释、
- 从易使用上来说你的代码要具备设计模式的包装让对外的服务调用更简单、
- 从易扩展上来说你的代码要做好业务和功能的实现分层。

在**易阅读、易使用、易扩展**以及更多编码规范的约束下，还需要在开发完成上线后的交付结果上满足；**高可用、高性能、高并发**，与此同时你还会接到现有项目中层出不穷来自产品经理新增的需求
> 新加的需求如果是以破坏了你原有的封装了非常完美 500 行的 ifelse 咋办，拆了重盖吗？
> 兄嘚，给代码留条活路吧！你的代码用上了定义接口吗、接口继承接口吗、接口由抽象类实现吗、类继承的类实现了接口方法吗，而==这些操作都是为了让你的程序逻辑做到分层、分区、分块，把核心逻辑层和业务封装层做好隔离==，当有业务变化时候，只需要做在业务层完成装配，而底层的核心逻辑服务并不需要频繁变化，它们所增加的接口也更原子化，不具备业务语意

如果还不是太理解，可以多看看《重学 Java设计模式》和现在编写的《手撸 Spring》，这里面都有大量的设计模式应用实践

---
### 2. 目标
当我们的类创建的 Bean 对象、交给 Spring 容器管理以后，这个类对象就可
以被赋予更多的使用能力。就像已经给类对象添加了修改注册
Bean 定义**未实例化前的属性信息**、和修改实例化过程中的前置和后置处理，这些额外能力的实现，都可以让我们对现有工程中的类对象做相应的扩展处理。

那么除此之外，还希望可以在 Bean 初始化过程执行一些操作。比如帮我
们**做一些数据的加载执行**，链接注册中心暴漏 RPC 接口以及**在 Web 程序关闭时执行链接断开，内存销毁等操作**。如果说==没有 Spring 我们也可以通过构造函数、静态方法以及手动调用的方式实现==，但这样的处理方式终究不如==把诸如此类的操作都交给 Spring 容器来管理更加合适==。 因此你会看到 spring.xml
中有如下操作：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261508166.png)
需要满足
- 用户可以在 xml 中配置初始化和销毁的方法，
- 也可以通过实现类的方式处理，比如我们在使用 Spring 时用到的 InitializingBean, DisposableBean 两个接口。
- 其实还可以有一种是注解的方式处理初始化操作，不过目前还没有实现到注解的逻辑，后续再完善此类功能。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261514532.png)

- 在 spring.xml 配置中**添加 init-method、destroy-method 两个注解**，==在配置文件加载的过程中，把注解配置一并定义到 BeanDefinition 的属性当中==。这样在 initializeBean 初始化操作的工程中，就可以**通过反射的方式**来调用配置在 Bean 定义属性当中的**方法信息**了。
- 另外如果是接口实现的方式，那么直接可以**通过 Bean 对象调用对应接口定义的方法**即可，((InitializingBean) bean).afterPropertiesSet()，两种方式达到的效果是一样的。
- 除了在初始化做的操作外，==destroy-method 和 DisposableBean 接口的定义，都会在 Bean 对象初始化完成阶段，注册销毁方法的信息到DefaultSingletonBeanRegistry 类中的 disposableBeans 属性里==，这是为了后续统一进行操作。
- ***这里还有一段适配器的使用，因为反射调用和接口直接调用，是两种方式***。所以*需要使用适配器进行包装*，下文代码讲解中参考DisposableBeanAdapter 的具体实现 
- 关于销毁方法需要在虚拟机执行关闭之前进行操作，所以这里需要用到一个注册钩子的操作，如Runtime.getRuntime().addShutdownHook(new Thread(() -> System.out.println("close！"))); 这段代码你可以执行测试，另外你**可以使用手动调用 ApplicationContext.close 方法关闭容器**。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261524686.png)
整个类图结构描述出来的就是本次新增 Bean 实例化过程中的初始化方法和销
毁方法。==因为我们一共实现了两种方式的初始化和销毁方法，xml 配置和定义接口==，所以这里既有 InitializingBean、DisposableBean 也有需要 XmlBeanDefinitionReader 加载 spring.xml 配置信息到 BeanDefinition 中。

另外接口 ConfigurableBeanFactory 定义了 destroySingletons 销毁方法，并由
AbstractBeanFactory 继承的父类 DefaultSingletonBeanRegistry 实现
ConfigurableBeanFactory 接口定义的 destroySingletons 方法。这种方式的设计可能多数程序员是没有用过的，都是用的谁实现接口谁完成实现类，而不是把实现接口的操作又交给继承的父类处理。所以这块还是蛮有意思的，是一种不错的隔离分层服务的设计方式
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261531139.png)
最后就是**关于向虚拟机注册钩子，保证在虚拟机关闭之前，执行销毁操作**。
Runtime.getRuntime().addShutdownHook(new Thread(() ->
System.out.println("close！")));
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261528812.png)

### 2. 定义初始化和销毁方法的接口
springframework.beans.factory.InitializingBean
```java
package com.memcpy0.springframework.beans.factory;  
  
/**  
 * Interface to be implemented by beans that need to react once all their * properties have been set by a BeanFactory: for example, to perform custom * initialization, or merely to check that all mandatory properties have been set. * * 实现此接口的 Bean 对象，会在 BeanFactory 设置属性后作出相应的处理，如：执行自定义初始化，或者仅仅检查是否设置了所有强制属性。  
 */  
public interface InitializingBean {  
    /**  
     * Bean处理了属性填充后调用  
     *   
* @throws Exception  
     */    void afterPropertiesSet() throws Exception;  
}
```

```java
package com.memcpy0.springframework.beans.factory;  
  
/**  
 * Interface to be implemented by beans that want to release resources * on destruction. A BeanFactory is supposed to invoke the destroy * method if it disposes a cached singleton. An application context * is supposed to dispose all of its singletons on close. * 要由希望在销毁时释放资源的bean实现的接口  
 * 如果BeanFactory处理了一个缓存的singleton, 那么它应该调用destroy方法  
 * 应用程序上下文应该在关闭时处理其所有singleton。  
 */  
public interface DisposableBean {  
    void destroy() throws Exception;  
}
```

### 3. Bean 属性定义新增初始化和销毁
