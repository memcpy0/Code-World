## 1. Spring refresh 流程
**要求**
* 掌握 refresh 的 12 个步骤

**Spring refresh 概述**
refresh 是 AbstractApplicationContext 中的一个方法，**负责初始化 ApplicationContext 容器**，容器必须调用 refresh 才能正常工作。
> BeaFactory父接口（Bean的创建、依赖注入、初始化等） -》ApplicationContext子接口（核心功能交给BeanFactory完成）  -》实现的抽象类 AbstractApplicationContext

该抽象类的refresh方法内部主要会调用 12 个方法（不包括catch/finally），我们把它们称为 refresh 的 12 个步骤：
1. prepareRefresh
2. obtainFreshBeanFactory
3. prepareBeanFactory
4. postProcessBeanFactory
5. invokeBeanFactoryPostProcessors
6. registerBeanPostProcessors
7. initMessageSource
8. initApplicationEventMulticaster
9. onRefresh
10. registerListener
11. finishBeanFactoryInitialization
12. finishRefresh

> ***功能分类***
> * 1 为准备环境
> * 2 3 4 5 6 为准备 BeanFactory
> * 7 8 9 10 12 为准备 ApplicationContext
> * 11 为初始化 BeanFactory 中**非延迟单例 bean**

**1. prepareRefresh**
* **这一步创建和准备了 Environment 对象，它作为 ApplicationContext 的一个成员变量**
* Environment 对象的作用之一是**为后续 @Value，值注入时提供键值**
* Environment 分成三个主要部分
	* systemProperties - 保存 java 环境键值
	* systemEnvironment - 保存系统环境键值
	* 自定义 PropertySource - **保存自定义键值**，例如来自于 `*.properties` 文件的键值

一开始ApplicationContext空空如也，而prepareRefresh创建和准备了Environment对象，赋值给了ApplicationContext的一个成员变量
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303260035937.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303260043612.png)
下面用Environment解析@Value中 `{}` 内的值：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303260047193.png)

如果是自定义键值（比如jdbc.username），则StandardEnvironment不知道。我们需要告知它相关properties文件（读Spring配置文件）的信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303260051667.png)

这里 `$` 用在key上做拼接 ，还可以用在配置文件的value上做拼接 `#{'class version:' + '${java.class.version}'}`
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303260055739.png)

**2. obtainFreshBeanFactory**
* **这一步获取（或创建） BeanFactory，它也是作为 ApplicationContext 的一个成员变量**
* BeanFactory 的作用是**真正负责 bean 的创建、依赖注入和初始化**，**bean 的各项特征由 BeanDefinition 定义**
	* BeanDefinition 作为 **bean** 的设计蓝图，**规定了 bean 的特征**，如单例多例、依赖关系、初始销毁方法等
	* BeanDefinition 的来源有多种多样，可以是**通过 xml 获得、配置类获得、组件扫描**获得，也可以是编程添加
* 所有的 BeanDefinition 会存入 BeanFactory 中的 **beanDefinitionMap 集合**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261008340.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261013896.png)
来源于XML，如静态内部类：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261014566.png)
XMLBeanDefinitionReader负责解析XML：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261015165.png)
来源是配置类，如Config1，用@Bean修饰会生成一个Bean2：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261017742.png)
这里是registerBeanDifinition，还需一个**ConfigurationClassPostProcessor解析配置类**、增强beanFactory的功能（Spring标准功能中不能识别@Bean注解，解析为Bean；包括@ImportSource和@Import、@Component、@Service等注解都需要额外代码来解析）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261019246.png)
来源于扫描：用ClassPathBeanDefinitionScanner：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261027362.png)
**3. prepareBeanFactory**
* 这一步会**进一步完善 BeanFactory**，为它的各项成员变量赋值
* **beanExpressionResolver 用来解析 SpEL**，常见实现为 StandardBeanExpressionResolver
* **propertyEditorRegistrars 会注册一些类型转换器**（尤其是值注入时，需要把字符串转换为其他类型）
	* 它在这里使用了 ResourceEditorRegistrar 实现类
	* 并==应用 ApplicationContext 提供的 Environment 完成 ${ } 解析==
* **registerResolvableDependency 来注册 beanFactory 以及 ApplicationContext**（它们不是真正的Bean，不在BeanDefinitionMap，是一些特殊的对象），**让它们也能用于依赖注入**（特殊的用来依赖注入的对象）
* ==beanPostProcessors 是 **bean 后处理器集合**，会工作在 **bean 的生命周期各个阶段**做一些功能增强==，此处会**添加两个**：
  * ApplicationContextAwareProcessor 用来解析 Aware 接口
  * ApplicationListenerDetector 用来识别容器中 ApplicationListener 类型的 bean
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261029684.png)

**4. postProcessBeanFactory**
* 这一步是空实现，**留给子类扩展**。
	* 一般 Web 环境的 ApplicationContext 都要利用它注册新的 Scope，完善 Web 下的 BeanFactory（singleton prototype session request response）
* 这里体现的是**模板方法设计模式**（模板方法的主要代码和调用顺序都固定死了，但功能留给子类扩展）

**5. invokeBeanFactoryPostProcessors**（BeanFactory的另一种扩展）
* 这一步会==调用 beanFactory 后处理器==
* beanFactory 后处理器，充当 beanFactory 的扩展点，可以用来==**补充或修改 BeanDefinition**==
* 常见的 beanFactory 后处理器有
  * `ConfigurationClassPostProcessor` – 解析 @Configuration、@Bean、@Import、@PropertySource 等
  * `PropertySourcesPlaceHolderConfigurer` – 替换 BeanDefinition 中的 ${ } ，现在用到很少了（借助Environment来解析）
  * `MapperScannerConfigurer` – **补充 Mapper 接口对应的 BeanDefinition**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261040953.png)

**6. registerBeanPostProcessors**（是Bean的后处理器，不是BeanFactory的后处理器，**区分开！**）
* 这些后处理器从哪里来？==都是在beanDefinitionMap中搜索，看有没有实现了BeanPostProcessor接口、是个特殊的Bean、是BeanPostProcessor，就把这样的Bean创建出来==（这一步是继续从 beanFactory 的BeanDefinitionMap 中找出 bean 后处理器），添加至 beanPostProcessors 集合中。==将来Bean真正创建时就可用到这些后处理器了==！
* bean 后处理器，充当 bean 的扩展点，**可以工作在 bean 的实例化、依赖注入、初始化阶段**，常见的有：
  * AutowiredAnnotationBeanPostProcessor 功能有：解析 @Autowired，@Value 注解
  * CommonAnnotationBeanPostProcessor 功能有：解析 @Resource，@PostConstruct，@PreDestroy
  * AnnotationAwareAspectJAutoProxyCreator 功能有：为**符合切点的目标 bean** 自动创建代理
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261043884.png)

下面是几个Bean，用@Autowired和@Resource写在方法上，给参数注入依赖，还可做一下打印；还要对foo方法做一下增强，执行foo方法前打印一些信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261053504.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261055049.png)
下面用getDefaultListableBeanFactory()获取默认的BeanFactory，并用编程方式往BeanFactory中注册BeanDefinition，接着准备环境context.refresh，最后从beanFactory获取Bean1对象、调用其foo方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261057698.png)
现在结果如下，获取到了Bean1调用了foo方法，但@Autowired、@Resource依赖注入和@Aspect前置通知都没执行：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261103439.png)
> 讲后置处理器对bean的增强，比如解析Autowired，resource注解

说白了，**标准的DefaultListableBeanFactory容器中没有这些功能、需要扩展**。beanPostProcessor功能不同，有的识别Autowired注解、有的做功能增强……如果给BeanFactory添加一个BeanDefinition（其实就是把BeanPostProcessor当成一个普通的Bean、添加其定义），下面的BeanPostProcessor解析Autowired注解：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261115023.png)
==将来在第6步registerBeanPostProcessors会把这个后处理器识别出来，添加至 beanPostProcessors 集合中==。创建Bean1时就会调用它识别Autowired注解，完成依赖注入。现在就发生了依赖注入：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261118409.png)
解析Resource注解，注入Bean3：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261119709.png)
> 一直没弄明白什么时候用autowired什么时候用resource？Autowired是默认按bean类型，resource是默认按名字，resource=autowired+quailfier 
> bean后处理开发中相当常用，认真听
> 有的招聘需求上写的有了解spring源码。spring的核心就是这12个方法

下面是解析@AspectJ注解和@Before，作用包括匹配表达式，看有没有方法需要功能增强，匹配到了后创建Bean1后不会创建原始Bean、而是创建代理对象，代理对象的方法中先调用增强的功能、再调用原始方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261123742.png)

**7. initMessageSource**（ApplicationContext的独特功能）
* 这一步是为 ApplicationContext **添加 messageSource 成员**，**实现国际化功能**
* 去 beanFactory 内找名为 messageSource 的 bean，如果没有，则**提供空的 MessageSource 实现**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261233792.png)
**8. initApplicationContextEventMulticaster**
* 这一步为 ApplicationContext **添加事件广播器成员，即 applicationContextEventMulticaster**
* 它的作用是**发布事件给监听器**
* 去 beanFactory 找名为 applicationEventMulticaster 的 bean 作为**事件广播器**。**若没有，会创建默认的事件广播器**
* 之后就可以调用 ApplicationContext.publishEvent(事件对象) 来发布事件
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261234484.png)

**9. onRefresh**
* 这一步是空实现，留给子类扩展
  * **SpringBoot 中的子类在这里准备了 WebServer，即内嵌 web 容器**
* 体现的是**模板方法设计模式**

**10. registerListeners**
* 这一步会**从多种途径找到事件监听器，并添加至 applicationEventMulticaster**
* ==事件监听器顾名思义，用来接收事件广播器发布的事件==，有如下来源
	* 事先编程添加的
	* 来自**容器中的 bean**
	* 来自于 **@EventListener 的解析**
* **要实现事件监听器，只需要实现 ApplicationListener 接口**，重写其中 `onApplicationEvent(E e)` 方法即可。将来applicationEventMulticaster事件广播器发事件时，就会通过E e把事件传递过来：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261238128.png)

**11. finishBeanFactoryInitialization**
* 这一步会将 beanFactory 的成员补充完毕，并初始化所有非延迟单例 bean
* conversionService 也是一套转换机制，作为对 PropertyEditorRegistrars（这套接口实现的不完整）的补充，**两套并存**！
* embeddedValueResolvers 即内嵌值解析器，用来解析 @Value 中的 ${ }，**借用的是 Environment 的功能**
* singletonObjects 即单例池，缓存所有单例对象。BeanFactory找到beanDefinitionMap中的所有非延迟单例对象创建Bean出来（延迟的后面用到再创建；懒汉式？）
	* 对象的创建过程都分三个阶段（Bean的创建、依赖注入、初始化），**每一阶段都有不同的 bean 后处理器参与进来，扩展功能**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261240841.png)
**12. finishRefresh**
* 这一步会为 ApplicationContext 添加 **lifecycleProcessor 成员，用来控制容器内需要生命周期管理的 bean**
* 如果容器中有名称为 lifecycleProcessor 的 bean 就用它，否则创建默认的生命周期管理器
* 准备好生命周期管理器，就可以实现
	* 调用 context 的 start，即可触发所有实现 LifeCycle 接口 bean 的 start（带头大哥？）
	* 调用 context 的 stop，即可触发所有实现 LifeCycle 接口 bean 的 stop
* 发布 ContextRefreshed 事件，整个 refresh 执行完成
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261243318.png)
### 总结
1. prepareRefresh：准备一个Environment对象，准备一些键值信息
2. obtainFreshBeanFactory：创建或获取BeanFactory；
3. prepareBeanFactory：准备BeanFactory；
4. postProcessBeanFactory：子类扩展BeanFactory
5. invokeBeanFactoryPostProcessors：后处理器扩展BeanFactory
6. registerBeanPostProcessors：准备Bean后处理器
7. initMessgeSource：为ApplicationContext提供国际化功能
8. initApplicationContextEventMulticaster：为ApplicationContext提供事件发布器；
9. onRefresh：留给子类扩展
10. registerListener**s**：为ApplicationConetxt准备事件监听器
11. finishBeanFactoryInitialization：初始化单例Bean，执行Bean后处理器扩展
12. finishRefresh：**准备生命周期管理器**，发布ContextRefreshed事件

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261250375.png)


---
## 2. Spring bean 生命周期
> 源码时spring注解那个。声音太小了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261258810.png)

**要求**
* 掌握 Spring bean 的生命周期

**bean 生命周期 概述**（一开始并不会创建这些Bean，而是懒加载，因此是从doGetBean开始）
bean 的生命周期从调用 beanFactory 的 doGetBean方法 开始，到这个 bean 被销毁，可以总结为以下七个阶段：
1. 处理名称，检查缓存
2. 处理父子容器
3. 处理 dependsOn
4. 选择 scope 策略
5. 创建 bean
6. 类型转换处理
7. 销毁 bean

> ***注意***
> * 划分的阶段和名称并不重要，重要的是理解整个过程中做了哪些事情

**1. 处理名称，检查缓存**
* 这一步会**处理别名**（一个Bean可能有很多别名），将别名**解析为实际名称**
* 一些特殊的Bean名字也特殊。**对 FactoryBean 也会特殊处理**，如果以 & 开头表示要获取 FactoryBean 本身，否则表示要获取其产品（bean的名字前面加&符号获取beanFactory）
* 这里**去缓存中，针对单例对象会检查一级、二级、三级缓存**
  * singletonFactories 三级缓存，存放单例工厂对象（能解决循环依赖）
  * earlySingletonObjects 二级缓存，存放单例工厂的产品对象
	- 如果发生循环依赖，产品是代理；无循环依赖，产品是原始对象（发生代理时解决循环依赖）
  * singletonObjects 一级缓存，存放单例成品对象（有就不用创建，同时保证单例对象唯一性）

**2. 处理父子容器**（子容器的缓存中没有这个Bean）
* 如果当前容器根据名字找不到这个 bean，此时若父容器存在，则执行父容器的 getBean 流程
* **父子容器的 bean 名称可以重复**，优先找子容器的Bean，找到了直接返回，找不到继续到父容器找

**3. 处理 dependsOn**
* 如果当前 bean 有通过 dependsOn 指定了**非显式依赖的 bean**，这一步会提前创建这些 dependsOn 的 bean 
* 所谓非显式依赖，就是指==两个 bean 之间不存在直接依赖关系，但需要控制它们的创建先后顺序==
**4. 选择 scope 策略，按照不同Scope创建bean**（不同Scope的生命周期不同；scope理解为从xxx范围中找这个bean更贴切）
* 对于 singleton scope，首先到单例池去获取 bean，如果有则直接返回，**没有再进入创建流程、创建并放入BeanFactory单例池**
* 对于 prototype scope，表示从不缓存bean，每次都会进入创建流程、创建新的，但**哪也不放、自己管理**
* 对于**自定义 scope**，例如 request，首先到 request 域获取 bean，如果有则直接返回，没有再进入创建流程、创建并放入**request域**

下面分别举单例、多例和request scope的例子：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261320942.png)

==Singleton Scope中从refresh创建到close被销毁，BeanFactory会记录哪些bean要调用销毁方法==，（非延迟）单例bean从ApplicationContext的refresh方法第十一步finishBeanFactoryInitialization中创建并放入singletonObjects单例池，**创建时还是调用每个单例bean的getBean方法**（其中调用了每个Bean的初始化方法），当我们调用context.close()时，Bean1注册了销毁方法，所以最终会调用到它。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261324903.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261322241.png)

==Prototype Scope从首次getBean被创建到调用BeanFactory的destroyBean方法被销毁==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261327997.png)
request scope中的bean从首次getBean时被创建、放到request作用域，到request对象结束前被销毁
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261330134.png)

**5.1 创建 bean - 创建 bean 实例**
还有一个小阶段，注册可销毁Bean阶段。每个阶段都有BeanPostProcessor参与进来做功能增强和扩展。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261339640.png)

| **要点**                             | **总结**                                                     |
| ------------------------------------ | ------------------------------------------------------------ |
| 有自定义 TargetSource 的情况         | 由 AnnotationAwareAspectJAutoProxyCreator 创建代理返回       |
| Supplier 方式创建 bean 实例          | 为 Spring 5.0 新增功能，方便编程方式创建  bean  实例         |
| FactoryMethod 方式  创建 bean  实例  | ① **分成静态工厂与实例工厂**；② 工厂方法若有参数，需要对工厂方法参数进行解析，利用  resolveDependency；③ 如果有多个工厂方法候选者，还要进一步按权重筛选 |
| AutowiredAnnotationBeanPostProcessor | ① **优先选择带  @Autowired  注解的构造**；② 若有唯一的带参构造，也会入选 |
| mbd.getPreferredConstructors         | 选择所有公共构造，这些构造之间按权重筛选                     |
| 采用默认构造                         | ==如果上面的后处理器和 BeanDefiniation 都没找到构造，采用默认构造，即使是私有的== |

**5.2 创建 bean - 依赖注入**
前两个是后处理器，扩展了依赖注入的功能；
| **要点**                             | **总结**                                                     |
| ------------------------------------ | ------------------------------------------------------------ |
| AutowiredAnnotationBeanPostProcessor(注解匹配) | 识别   @Autowired  及 @Value  标注的成员，封装为  InjectionMetadata 进行依赖注入 |
| CommonAnnotationBeanPostProcessor(注解匹配)    | 识别   @Resource  标注的成员，封装为  InjectionMetadata 进行依赖注入 |
| resolveDependency                    | 用来查找要装配的值，可以识别：① Optional；② ObjectFactory 及 ObjectProvider；③ @Lazy  注解；④ @Value  注解（${  }, #{ }, 类型转换）；⑤ 集合类型（Collection，Map，数组等）；⑥ 泛型和  @Qualifier（用来区分类型歧义）；⑦ primary  及名字匹配（用来区分类型歧义） |
| AUTOWIRE_BY_NAME(根据名字匹配)        | 根据成员名字找 bean 对象，修改 mbd 的 propertyValues，**不会考虑简单类型的成员** |
| AUTOWIRE_BY_TYPE(根据类型匹配)        | 根据成员类型执行 resolveDependency 找到依赖注入的值，修改  mbd 的 propertyValues |
| applyPropertyValues(精确指定) | 根据 mbd 的 propertyValues 进行依赖注入（即xml中 `<property name ref|value/>`） |

如果有多个依赖注入，哪个依赖注入优先级更高？
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261644034.png)

通过编程方式，给Bean添加一种装配方式：Autowired_by_name是根据set方法的后面类名匹配
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261646212.png)

如果使用精确匹配，优先级最高！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261647103.png)


**5.3 创建 bean - 初始化**
如果Bean实现了一些Aware接口，则初始化会先处理一些Aware接口，Spring的容器回调相关接口中的方法。
接下来调用初始化方法。有三种不同的初始化方法，①是PostConstruct标注是初始化方法；② 是实现InitializingBean接口，回调其中方法来初始化。③是给出初始化方法执行初始化。
如果Bean跟切点表达式匹配到了，则通过BeanPostProcessor创建AOP代理，在初始化最后一步执行。

| **要点**              | **总结**                                                     |
| --------------------- | ------------------------------------------------------------ |
| 内置 Aware 接口的装配 | 包括 BeanNameAware，BeanFactoryAware 等                      |
| 扩展 Aware 接口的装配 | 由 ApplicationContextAwareProcessor 解析，执行时机在  postProcessBeforeInitialization |
| @PostConstruct        | 由 CommonAnnotationBeanPostProcessor 解析，执行时机在  postProcessBeforeInitialization |
| InitializingBean      | 通过接口回调执行初始化                                       |
| initMethod            | 根据 BeanDefinition 得到的初始化方法执行初始化，即 `<bean init-method>` 或 @Bean(initMethod) |
| 创建 aop 代理         | 由 AnnotationAwareAspectJAutoProxyCreator 创建，执行时机在  postProcessAfterInitialization |

Aware接口方法的调用和初始化三种方法的优先级：4 2 1 3
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261654179.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261657883.png)

**5.4 创建 bean - 注册可销毁 bean**
在这一步判断并**登记可销毁 bean**
* 判断依据
  * 如果实现了 DisposableBean（跟InitializingBean接口相对应）或 AutoCloseable 接口，则为**可销毁 bean**
  * 如果自定义了 destroyMethod，则为可销毁 bean
  * ==如果采用 @Bean 没有指定 destroyMethod，则采用自动推断方式获取销毁方法名==（close，shutdown）
  * 如果有 @PreDestroy 标注的方法
* 存储位置
  * singleton scope 的可销毁 bean 会存储于 **beanFactory 的成员**当中
  * 自定义 scope 的可销毁 bean 会存储于**对应的域对象**当中
  * **prototype scope 不会存储，需要自己找到此对象销毁**
* 存储时都会封装为 DisposableBeanAdapter 类型，对销毁方法的调用进行适配，不管有多少种销毁方法，最后都统一为对该销毁方法的调用

**6. 类型转换处理**
* 如果 getBean 的 **requiredType 参数**与我们经过上面步骤实际得到的对象类型不同，会尝试进行类型转换
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261707267.png)

**7. 销毁 bean**
* 销毁时机
	* singleton bean 的销毁在 ApplicationContext.close 时，此时会找到**所有 DisposableBean 的名字**，逐一销毁
	* 自定义 scope bean 的销毁在**作用域对象生命周期结束**时
	* prototype bean 的销毁可以通过自己手动调用 AutowireCapableBeanFactory.destroyBean 方法执行销毁
* 同一 bean 中**不同形式销毁方法**的调用次序
  * **优先后处理器销毁**，即 @PreDestroy
  * 其次 DisposableBean 接口销毁
  * 最后 destroyMethod 销毁（包括自定义名称，推断名称，AutoCloseable 接口 多选一）

### 总结：Spring Bean的生命周期
阶段1：**处理名称的别名，检查缓存，有就直接返回，没有就执行后续流程**。
阶段2：**检查父工厂的Bean，父工厂有就返回**。
阶段3：检查DependsOn，**如果有创建顺序要求，就把依赖的Bean创建了**，再创建需要的Bean。
阶段4：**按Scope创建Bean**。
① 创建Singleton；
② 创建Prototype；
③ 创建其他Scope。
阶段5：**创建Bean**
① 创建Bean实例 - 构造方法有@Autowired修饰的优先，没有的就选择唯一的带参构造，都没提供就采用默认构造创建实例。
② 依赖注入：@Autowired @Value（AutoWiredAnnotationPostProcessor）@Resource，ByName ByType ，精确指定
③ 初始化：Aware接口处理，调用初始化方法@PostConstruct、InitializingBean接口、initMethod，创建AOP代理对象
④ 登记可销毁的Bean：实现了DisposableBean或AutoCloseable接口的Bean、指定了destroyMethod的Bean、有@PreDestroy的方法
阶段6：**类型转换**
阶段7：**销毁Bean**

---
## 3. Spring bean 循环依赖

### 代理对象的创建
要完全理解循环依赖，需要理解代理对象的创建时机。掌握ProxyFactory创建代理的过程，理解Advisor、Advice、PointCut、Aspect。掌握AnnotationAwareAspectJAutoProxyCreator筛选Advisor合格者，创建代理的过程。
> aspect切面=advice通知+pointcut切点，一个切面类中可能有一到多个通知方法。
> advisor=更细粒度的切面，只包含一个通知和切点
> 以advisor方式添加通知和切点进行功能增强，不会拿aspect

proxyFactory.setTarget不是直接关联到目标对象，而是：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271316779.png)
通知：最重要的一个MethodInterceptor（类似环绕通知） extends Interceptor extends Advice
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271319494.png)
结果如下（没有用到切点）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271347484.png)
看起来上面没有用到切点实际还是用到了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271400926.png)

下面当foo()匹配到了切点就增强，bar()没匹配到就不增强：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271353091.png)

当然，可以有多个切面advisor：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271355492.png)

打印其代理的class对象，注意到**子类代理技术用的是CGLIB**（spring默认用的是JDK的动态代理，而SPRINGBOOT默认用的是CGLIB动态代理模式）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271404039.png)

换成JDK的动态代理（只能针对接口类型创建代理），==用了接口、此时代理对象和目标对象是平级类型，不能被强制转换，只能转换为共同的父类型（即接口）==：
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271409592.png)

不管三七二十一，不管有没有实现接口，我都想用CGLIB生成代理对象：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271412457.png)

### 代理对象和Advisor的关系
CGLIB下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271414192.png)
JDK动态代理下，advised里面有advisor这些切面对象：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271415590.png)

### Aspect和Advisor关系
实际开发肯定不会和上面一样，而是用注解：用@Aspect标注的是切面，@Before等标注的是通知，里面的式子是切点（都会转为一个Advisor切面类，方法会转换为实现了……）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271418006.png)
在上图基础上，证明@Aspect会被转换Spring内置的Advisor，其中的通知都会转换为MethodInterceptor：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271423246.png)
结果如下：
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271425914.png)
调试一下，发现里面有
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271429739.png)

### 学习AnnotationAwareAspectJAutoProxyCreator  
wrapIfNecessary（protected方法——反射调用；或Debug；或者把测试方法放到该方法的类的包中）如果需要就包装Bean（包装就是创建代理，检查后条件满足就创建代理）——首先，@Aspect修饰的类不能创建代理：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271440664.png)
而Target目标对象没有匹配到切点时也不创建：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271449446.png)

下面调试wrapIfNecessary：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271456413.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271458165.png)
接着创建代理：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271502416.png)
进入createProxy：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271508626.png)
### AnnotationAwareAspectJAutoProxyCreator代理创建时机
创建Bean的实例之前，如果Bean创建了TargetSource，则调用AnnotationAwareAspectJAutoProxyCreator自动代理处理器创建代理对象。小众用法，没谁用。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271514269.png)
在初始化阶段，在初始化方法init-method执行完后，调用后处理器来创建代理
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271516478.png)
第三个地方发生在依赖注入阶段。产生循环依赖后，在单例工厂池中通过一个工厂对象，通过自动代理处理器创建代理：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271515119.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271513630.png)

### 总结
Spring中最基本的切面是Advisor，一个@Aspect切面对应1到多个Advisor。AOP中最基本的Advice是MethodInterceptor，其他Advice最终都将适配为MethodInterceptor。
创建代理的方式：
- 实现了用户自定义接口，采用JDK动态代理
- 没有实现用户自定义接口，采用CGLIB代理
- 设置了setProxyTargetClass(true)，统一采用CGLIB代理

AnnotationAwareAspectJAutoProxyCreator：切面、切点、通知等不会被代理。调用时机：创建阶段、依赖注入阶段、**初始化阶段**（主要创建代理的地方）

---
**要求**
* 掌握单例set方式循环依赖的原理
* 掌握其它循环依赖的解决方法

> Spring不支持原型Bean的循环依赖，一旦出现，直接抛异常
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272343049.png)


**循环依赖的产生**
* 首先要明白，bean 的创建要遵循一定的步骤，必须是**创建、依赖注入、初始化**三步，这些顺序不能乱
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261349832.png)
* set 方法（包括成员变量）的循环依赖如图所示
  * 可以在【a 创建】和【a set 注入 b】之间加入 b 的整个流程来解决
  * 【b set 注入 a】 时可以成功，因为之前 a 的实例已经创建完毕
  * a 的顺序，及 b 的顺序都能得到保障
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261350521.png)

### 解决 set 循环依赖的原理
一级缓存SingletonObjects作用：限制bean在beanFactory中只存一份，即实现singleton scope： 
**一级缓存**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261352814.png)
作用是保证单例对象仅被创建一次
* 第一次走 `getBean("a")` 流程后，**最后会将成品 a 放入 singletonObjects 一级缓存**
* 后续再走 `getBean("a")` 流程时，先从一级缓存中找，这时已经有成品 a，就无需再次创建

 **一级缓存与循环依赖**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261352903.png)

一级缓存无法解决循环依赖问题，分析如下
* 无论是获取 bean a 还是获取 bean b，走的方法都是同一个 getBean 方法，假设先走 `getBean("a")`
* 当 a 的实例对象创建，接下来执行 `a.setB()` 时，需要走 `getBean("b")` 流程，红色箭头 1
* 当 b 的实例对象创建，接下来执行 `b.setA()` 时，又回到了 `getBean("a")` 的流程，红色箭头 2
* 但此时 singletonObjects 一级缓存内没有成品的 a，陷入了死循环

一级缓存无法解决下面的循环依赖：假设先执行A a = singletons.get。依赖注入阶段，就遇到了麻烦，a.setB(?)会间接调用b的getBean，b = singletons.get也返回null，因此b = new B()，接着b.setA(?)就要间接调用a的getBean。这就重复循环了…… 

**二级缓存**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261352235.png)
解决思路如下：
* 再增加一个 singletonFactories 缓存
* **在依赖注入前**，即 `a.setB()` 以及 `b.setA()` **将 a 及 b 的半成品对象（未完成依赖注入和初始化）放入此缓存**
* ==执行依赖注入时，先看看 singletonFactories 缓存中是否有半成品的对象，如果有拿来注入，顺利走完流程==

二级缓存SingletonFactories（Spring中它叫三级缓存）：作用：解决循环依赖。（不一定和源码一致，只是讲解原理）从a开始，首先去一级缓存SingletonObjects看有没有A，没有成品则自己创建，创建出a = A()后，调用factories.put(a)将a（后续的依赖注入和初始化都没完成，是半成品对象）加入二级缓存。接着对a进行setB(?)依赖注入，它先factories.get从二级缓存看有没有半成品的Bean，发现没有返回null，才会调用b的getBean。

b这边也类似，发现没有成品Bean，就自己创建，并factories.put(b)把半成品的b放入二级缓存，接着对b进行setA(?)依赖注入，factories.get从二级缓存看有没有半成品的A的Bean，有就先用来完成依赖注入，发现是有的、就接着执行下去完成b的init()、把成品b对象通过singletons.put(b)放入一级缓存，而二级缓存的半成品的b是个临时引用，清理掉就行。完整的b对象返回去完成a的依赖注入，然后完成a的初始化，并将成品a放入一级缓存，同样清理之前的临时引用。
  
对于上面的图
* `a = new A()` 执行之后就会把这个半成品的 a 放入 singletonFactories 缓存，即 `factories.put(a)`
* 接下来执行 `a.setB()`，走入 `getBean("b")` 流程，红色箭头 3
* 这回再执行到 `b.setA()` 时，需要一个 a 对象，有没有呢？有！
* `factories.get()` 在 singletonFactories  缓存中就可以找到，红色箭头 4 和 5
* b 的流程能够顺利走完，将 b 成品放入 singletonObject 一级缓存，返回到 a 的依赖注入流程，红色箭头 6

**二级缓存与创建代理**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261353909.png)
二级缓存无法正确处理**循环依赖**并且包含有**代理创建**的场景，分析如下
* **spring 默认要求，在 `a.init` 完成之后才能创建代理 `pa = proxy(a)`**
* 由于 a 的代理创建时机靠后，在执行 `factories.put(a)` 向 singletonFactories 中**放入的还是原始对象**
* 接下来箭头 3、4、5 这几步 b 对象拿到和注入的都是原始对象

> 不应拿未增强的a对象去创建b，而应该去拿增强后的a去创建b；

**三级缓存**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261353398.png)
简单分析的话，只需要将代理的创建时机放在依赖注入之前即可，但 **spring 仍然希望代理的创建时机在 init 之后，只有出现循环依赖时，才会将代理的创建时机提前**。所以解决思路稍显复杂：
* **图中 `factories.put(fa)` 放入的既不是原始对象，也不是代理对象而是工厂对象 fa**
* ==当检查出发生循环依赖时，fa 的产品就是代理 pa（只有发生循环依赖才提前创建代理），没有发生循环依赖，fa 的产品是原始对象 a
* 假设出现了循环依赖，拿到了 singletonFactories 中的**工厂对象**，**通过在依赖注入前获得了 pa**，红色箭头 5
* **这回 `b.setA()` 注入的就是代理对象**，保证了正确性，红色箭头 7
* **还需要把 pa 存入新加的 earlySingletonObjects 缓存**，红色箭头 6
* `a.init` 完成后，无需二次创建代理，从哪儿找到 pa 呢？**earlySingletonObjects 已经缓存，蓝色箭头 9**
* 当成品对象产生，放入 singletonObject 后，singletonFactories 和 earlySingletonObjects 就中的对象就没有用处，清除即可

断点：doGetBean方法的getSingleton(beanName)（访问一级、二级、三级缓存看是否有a对象，beanName.equals("a")。第五阶段流程：doCreateBean方法的getSingleton(beanName, allowEarlyReference: false) 访问到一级和二级缓存（发生到循环依赖时进入这个if）。下面创建a的Bean，而a的set方法需要依赖注入b：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272132636.png)
为a进行依赖注入：调用getBean(b)，b又要进行依赖注入a，于是进入doGetBean的getSingleton(beanName)：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272143136.png)
添加工厂对象：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272146907.png)
可以看一下BeanFactory：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272150210.png)
继续跟进：
> 性能效率吧，又不是有很多循环依赖的，大部分还是单个bean的吧
> 双检是为了此刻其他线程已经创建了A，当前线程拿到锁后，需要先到一级二级缓存中捞一边有没有A
> 加锁是为了保证获取singletonFactory并创建A这些操作的原子性

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272208858.png)
获取工厂对象，并用其创建代理，工厂用完就没用了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272219100.png)
现在b的流程结束了，它在singleObjects单例池中有了对象，还依赖注入了a：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272223472.png)
然后，a的创建和依赖注入（注入b）都完成了，还要返回a的Bean。但还要去二级缓存中找getSingleton，==防止遗漏二级缓存中的代理对象==（就是之前工厂生产的A可能会是代理对象）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272229463.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272232776.png)

下面准备一个代理的例子，看一下上面的流程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272234578.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272237887.png)

断点加的都不变，直接Debug运行：
getBean 获取a -> a要依赖注入b -> getBean获取b -> b要populate依赖注入a -> 第三次getBean，走到缓存，getSingleton方法 -> 到singletonObjects和earlySingletonObjects找、没有 -> 去三级缓存SingletonFactory找，拿到a的工厂对象
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272254640.png)
-> 调用工厂对象的getObject()获得的是a的代理对象（提前创建的），并放入了earlySingletonObjects中，以后的初始化阶段就不需要重复创建了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272256676.png)
-> b已经创建完成了-> 现在回到a -> 由于创建了a的代理，现在initializingBean方法不会重复执行代理创建，返回的是原始目标对象a
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272306737.png)
### 如何避免代理重复创建
工厂对象中lambda表达式调用getEarlyBeanReference
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272313127.png)
b依赖注入a要提前创建代理，从上面走到下图的方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272311325.png)
由于b没有提前创建代理，所以它在postProcessAfterInitialization中创建代理

> 三级缓存思想是A创建完了之后，但是没有初始化，先把A的半成品放入SingletonFactory中

**构造方法的循环依赖**如图所示，显然无法用前面的方法解决（A的实例都无法创建成功！）
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261351746.png)
**构造循环依赖的解决**
* 思路1
  * **a 注入 b 的代理对象**（不代表要先创建B的对象，这里只是代理），这样能够保证 a 的流程走通，a完成初始化；b依赖注入时直接取singletonObjects中的a
  * 后续需要用到 b 的真实对象时，可以**通过代理间接访问**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261351302.png)
* 思路2
  * **a 注入 b 的工厂对象**，让 b 的实例创建被推迟，这样能够保证 a 的流程先走通
  * 后续需要用到 b 的真实对象时，再**通过 ObjectFactory 工厂间接访问**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261351338.png)
示例1：用 @Lazy 为构造方法参数生成代理
```java
public class App60_1 {
    static class A {
        private static final Logger log = LoggerFactory.getLogger("A");
        private B b;

        public A(@Lazy B b) { // 找到B的代理对象
            log.debug("A(B b) {}", b.getClass());
            this.b = b;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    static class B {
        private static final Logger log = LoggerFactory.getLogger("B");
        private A a;

        public B(A a) {
            log.debug("B({})", a);
            this.a = a;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    public static void main(String[] args) {
        GenericApplicationContext context = new GenericApplicationContext();
        context.registerBean("a", A.class);
        context.registerBean("b", B.class);
        AnnotationConfigUtils.registerAnnotationConfigProcessors(context.getDefaultListableBeanFactory());
        context.refresh();
        System.out.println();
    }
}
```
两个Bean都成功创建：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272322546.png)
resolveDependency：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272328891.png)

示例2：用 （ObjecFactory或其子类型）ObjectProvider 延迟依赖对象的创建
```java
public class App60_2 {
    static class A {
        private static final Logger log = LoggerFactory.getLogger("A");
        private ObjectProvider<B> b;

        public A(ObjectProvider<B> b) {
            log.debug("A({})", b);
            this.b = b;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    static class B {
        private static final Logger log = LoggerFactory.getLogger("B");
        private A a;

        public B(A a) {
            log.debug("B({})", a);
            this.a = a;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    public static void main(String[] args) {
        GenericApplicationContext context = new GenericApplicationContext();
        context.registerBean("a", A.class);
        context.registerBean("b", B.class);
        AnnotationConfigUtils.registerAnnotationConfigProcessors(context.getDefaultListableBeanFactory());
        context.refresh();

        System.out.println(context.getBean(A.class).b.getObject()); // 调用工厂对象的getObject()获得真正的b
        System.out.println(context.getBean(B.class));
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272328347.png)

示例3：用 @Scope 产生代理（会产生额外的BeanDefinition……）
```java
public class App60_3 {
    public static void main(String[] args) {
        GenericApplicationContext context = new GenericApplicationContext();
        ClassPathBeanDefinitionScanner scanner = new ClassPathBeanDefinitionScanner(context.getDefaultListableBeanFactory());
        scanner.scan("com.itheima.app60.sub");
        context.refresh();
        System.out.println();
    }
}
```

```java
@Component
class A {
    private static final Logger log = LoggerFactory.getLogger("A");
    private B b;

    public A(B b) { // 经过@Scope代理的B对象
        log.debug("A(B b) {}", b.getClass());
        this.b = b;
    }

    @PostConstruct
    public void init() {
        log.debug("init()");
    }
}
```

```java
@Scope(proxyMode = ScopedProxyMode.TARGET_CLASS) // 解决循环依赖,scope注解的解析必须是配置类或组件扫描的方式,因此在A,B类加了Component
@Component
class B {
    private static final Logger log = LoggerFactory.getLogger("B");
    private A a;

    public B(A a) {
        log.debug("B({})", a);
        this.a = a;
    }

    @PostConstruct
    public void init() {
        log.debug("init()");
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272337605.png)

示例4：用 Provider 接口解决，**原理上与 ObjectProvider 一样，Provider 接口是独立的 jar 包，需要加入依赖**
```xml
<dependency>
    <groupId>javax.inject</groupId>
    <artifactId>javax.inject</artifactId>
    <version>1</version>
</dependency>
```

```java
public class App60_4 {

    static class A {
        private static final Logger log = LoggerFactory.getLogger("A");
        private Provider<B> b;

        public A(Provider<B> b) {
            log.debug("A({}})", b);
            this.b = b;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    static class B {
        private static final Logger log = LoggerFactory.getLogger("B");
        private A a;

        public B(A a) {
            log.debug("B({}})", a);
            this.a = a;
        }

        @PostConstruct
        public void init() {
            log.debug("init()");
        }
    }

    public static void main(String[] args) {
        GenericApplicationContext context = new GenericApplicationContext();
        context.registerBean("a", A.class);
        context.registerBean("b", B.class);
        AnnotationConfigUtils.registerAnnotationConfigProcessors(context.getDefaultListableBeanFactory());
        context.refresh();

        System.out.println(context.getBean(A.class).b.get());
        System.out.println(context.getBean(B.class));
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303272332010.png)

---
## 4. Spring 事务失效
**要求**：面试题：事务失效的八种场景及原因、解决方案
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261724791.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261727223.png)

**1. 抛出检查异常导致事务不能正确回滚**
文件未找到是：检查型异常：要么说明抛出，要么要try-catch进行处理，即语法上要求处理！非检查型异常是可处理可不处理
```java
@Service
public class Service1 {
    @Autowired
    private AccountMapper accountMapper;

    @Transactional
    public void transfer(int from, int to, int amount) throws FileNotFoundException {
        int fromBalance = accountMapper.findBalanceBy(from);
        if (fromBalance - amount >= 0) {
            accountMapper.update(from, -1 * amount);
            new FileInputStream("aaa");
            accountMapper.update(to, amount);
        }
    }
}
```
* 原因：**Spring 默认只会回滚非检查（RuntimeException及其子类，Error及其子类）异常**
* 解法：配置 rollbackFor 属性
  * `@Transactional(rollbackFor = Exception.class)`

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261941501.png)
声明式事务的实现原理：此时context.getBean(Service2.class)从容器中拿到的Service2 Bean并不是原始的目标对象，而是一个代理对象，它调用transfer时才会去调用事务通知Advice，在事务通知（环绕通知）内部再会调用原始的目标对象。原始的transfer在调用的最里层，如果它捕获了异常，则外部的事务通知就不知道发生了异常。
**2. 业务方法内自己 try-catch 异常，导致事务不能正确回滚**
```java
@Service
public class Service2 {
    @Autowired
    private AccountMapper accountMapper;
    @Transactional(rollbackFor = Exception.class)
    public void transfer(int from, int to, int amount)  {
        try {
            int fromBalance = accountMapper.findBalanceBy(from);
            if (fromBalance - amount >= 0) {
                accountMapper.update(from, -1 * amount);
                new FileInputStream("aaa");
                accountMapper.update(to, amount);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
```
* 原因：==事务通知只有捉到了目标抛出的异常，才能进行后续的回滚处理，如果目标自己处理掉异常，事务通知无法知悉==
* 解法1：异常原样抛出（**对事务中的异常都要抛出去**！）
  * 在 catch 块添加 `throw new RuntimeException(e);` 抛出非检查型异常！
* 解法2：手动设置 TransactionStatus.setRollbackOnly()
  * 在 catch 块添加 `TransactionInterceptor.currentTransactionStatus().setRollbackOnly();`

**3. aop 切面顺序导致导致事务不能正确回滚**
```java
@Service
public class Service3 {
    @Autowired
    private AccountMapper accountMapper;
    @Transactional(rollbackFor = Exception.class)
    public void transfer(int from, int to, int amount) throws FileNotFoundException {
        int fromBalance = accountMapper.findBalanceBy(from);
        if (fromBalance - amount >= 0) {
            accountMapper.update(from, -1 * amount);
            new FileInputStream("aaa"); // ....
            accountMapper.update(to, amount);
        }
    }
}
```
没有乱加try-catch，抛出检查型异常时用了rollbackFor属性。下面加了一个MyAspect切面类，一个环绕通知，增强transfer方法。看看transfer执行过程中出现异常是否回滚？
```java
@Aspect
public class MyAspect {
    @Around("execution(* transfer(..))")
    public Object around(ProceedingJoinPoint pjp) throws Throwable {
        LoggerUtils.get().debug("log:{}", pjp.getTarget());
        try {
            return pjp.proceed(); // 有就调用下一个通知，或者调用目标方法
        } catch (Throwable e) {
            e.printStackTrace();
            return null;
        }
    }
}
```
结果还是提交Commit了。原因，它们都是环绕通知，先进入事务切面，然后是自定义切面，接着是目标方法，抛出的异常被自定义切面的around通知里面的try-catch捕捉了……
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261956016.png)
* 原因：事务切面优先级最低，但如果自定义的切面优先级和他一样，则还是自定义切面在内层，这时若自定义切面没有正确抛出异常…
* 解法1、2：同情况2 中的解法：1、2（**更推荐**）
* 解法3：调整切面顺序，在 MyAspect 上添加 `@Order(Ordered.LOWEST_PRECEDENCE - 1)` （不推荐），让异常先到事务切面
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262003286.png)

**4. 非 public 方法导致的事务失效**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262012230.png)
```java
@Service
public class Service4 {
    @Autowired
    private AccountMapper accountMapper;
    @Transactional
    void transfer(int from, int to, int amount) throws FileNotFoundException {
        int fromBalance = accountMapper.findBalanceBy(from);
        if (fromBalance - amount >= 0) {
            accountMapper.update(from, -1 * amount);
            accountMapper.update(to, amount);
        }
    }
}
```
* 原因：**Spring 为方法创建代理、添加事务通知，前提条件都是该方法是 public 的**
* 解法1：改为 public 方法
* 解法2：添加 bean 配置如下（不推荐）
```java
@Bean
public TransactionAttributeSource transactionAttributeSource() {
    return new AnnotationTransactionAttributeSource(false);
}
```

**5. 父子容器导致的事务失效**
controller中transfer调用了service中的transfer方法。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262030085.png)
可以把Service、Controller分别放在不同的父子容器（service, mapper等放在父容器，controller和MVC相关的一些配置等放在子容器）中，（子容器没有Service，就去父容器找Service完成依赖注入）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262037623.png)
```java
package day04.tx.app.service;

// ...

@Service
public class Service5 {

    @Autowired
    private AccountMapper accountMapper;

    @Transactional(rollbackFor = Exception.class)
    public void transfer(int from, int to, int amount) throws FileNotFoundException {
        int fromBalance = accountMapper.findBalanceBy(from);
        if (fromBalance - amount >= 0) {
            accountMapper.update(from, -1 * amount);
            accountMapper.update(to, amount);
        }
    }
}
```
控制器类
```java
package day04.tx.app.controller;
// ...
@Controller
public class AccountController {
    @Autowired
    public Service5 service; // 这里注入的是子容器的Service5,而非父容器的Service5
    public void transfer(int from, int to, int amount) throws FileNotFoundException {
        service.transfer(from, to, amount);
    }
}
```
App 配置类
```java
@Configuration
@ComponentScan("day04.tx.app.service")
@EnableTransactionManagement
// ...
public class AppConfig {
    // ... 有事务相关配置
}
```
Web 配置类（子容器没有配置事务管理）
```java
@Configuration
@ComponentScan("day04.tx.app")
// ...
public class WebConfig {
    // ... 无事务配置
}
```
现在配置了父子容器，WebConfig 对应子容器，AppConfig 对应父容器，**发现事务依然失效**
* 原因：子容器扫描范围过大，把未加事务配置的 service 扫描进来
* 解法1：**各扫描各的，不要图简便**，改成 `day04.tx.app.controller` 
* 解法2：不要用父子容器，所有 bean 放在同一容器

**6. 调用本类方法导致传播行为失效**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262111266.png)
执行结果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262112476.png)
```java
@Service
public class Service6 {

    @Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
    public void foo() throws FileNotFoundException { // 有了事务就加入已有的事务
        LoggerUtils.get().debug("foo");
        bar();
    }

    @Transactional(propagation = Propagation.REQUIRES_NEW, rollbackFor = Exception.class)
    public void bar() throws FileNotFoundException { // 不管之前有没有事务总会开启新的事务 // 应该有两个事务
        LoggerUtils.get().debug("bar");
    }
}
```
打印一下对象，发现调用foo方法的是代理对象，调用bar方法时的this.getClass()对象不是代理对象：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262115528.png)
* 原因：本类方法调用不经过代理，因此无法增强。必须通过代理对象取调用bar()
* 解法1：依赖注入自己（代理）来调用
* 解法2：通过 AopContext 拿到代理对象，来调用
* 解法3：通过 CTW，LTW （编译时植入、加载时植入）实现功能增强
* 解法4：方法写在不同的类中

解法1：
```java
@Service
public class Service6 {
	@Autowired
	private Service6 proxy; // 本质上是一种循环依赖,注入的是代理对象

    @Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
    public void foo() throws FileNotFoundException {
        LoggerUtils.get().debug("foo");
		System.out.println(proxy.getClass());
		proxy.bar();
    }

    @Transactional(propagation = Propagation.REQUIRES_NEW, rollbackFor = Exception.class)
    public void bar() throws FileNotFoundException {
        LoggerUtils.get().debug("bar");
    }
}
```
打印出来，看是不是真的代理：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262125968.png)
解法2，还需要在 AppConfig 上添加 `@EnableAspectJAutoProxy(exposeProxy = true)`
```java
@Service
public class Service6 {
    
    @Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
    public void foo() throws FileNotFoundException {
        LoggerUtils.get().debug("foo");
        ((Service6) AopContext.currentProxy()).bar();
    }

    @Transactional(propagation = Propagation.REQUIRES_NEW, rollbackFor = Exception.class)
    public void bar() throws FileNotFoundException {
        LoggerUtils.get().debug("bar");
    }
}
```
**7. @Transactional 没有保证原子行为**
```java
@Service
public class Service7 {
    private static final Logger logger = LoggerFactory.getLogger(Service7.class);

    @Autowired
    private AccountMapper accountMapper;

    @Transactional(rollbackFor = Exception.class)
    public void transfer(int from, int to, int amount) {
        int fromBalance = accountMapper.findBalanceBy(from);
        logger.debug("更新前查询余额为: {}", fromBalance);
        if (fromBalance - amount >= 0) { // 看余额是否充足,多线程下这段代码有问题
            accountMapper.update(from, -1 * amount);
            accountMapper.update(to, amount);
        }
    }
    public int findBalance(int accountNo) {
        return accountMapper.findBalanceBy(accountNo);
    }
}
```
上面的代码实际上是有 bug 的，假设 from 余额为 1000，两个线程都来转账 1000，可能会出现扣减为负数的情况
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262157235.png)
* 原因：**事务的原子性仅涵盖 insert、update、delete、select … for update 语句**，select 方法并不阻塞（保证了原子性，但是由于指令交错出现了问题）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261353206.png)
* 如上图所示，红色线程和蓝色线程的查询都发生在扣减之前，都以为自己有足够的余额做扣减
	这是线程顺序性，语句原子性问题，和幻读有啥关系

**8. @Transactional 方法导致的 synchronized 失效**
针对上面的问题，能否在方法上加 synchronized 锁来解决呢？
```java
@Service
public class Service7 {
    private static final Logger logger = LoggerFactory.getLogger(Service7.class);
    @Autowired
    private AccountMapper accountMapper;

    @Transactional(rollbackFor = Exception.class)
    public synchronized void transfer(int from, int to, int amount) {
        int fromBalance = accountMapper.findBalanceBy(from);
        logger.debug("更新前查询余额为: {}", fromBalance);
        if (fromBalance - amount >= 0) {
            accountMapper.update(from, -1 * amount);
            accountMapper.update(to, amount);
        }
    }

    public int findBalance(int accountNo) {
        return accountMapper.findBalanceBy(accountNo);
    }
}
```
答案是不行，原因如下：
* **synchronized 保证的仅是目标方法的原子性，环绕目标方法的还有 commit 等操作，它们并未处于 sync 块内**（synchronized只加在了目标对象的transfer方法，但代理对象的commit操作没有锁住）就是没锁住代理对象（就是没锁住临界区，这里的临界区包括SQL操作和事务的commit，根本原因就是事务未提交 锁就放开了）
* 可以参考下图发现，蓝色线程的查询只要在红色线程提交之前执行，那么依然会查询到有 1000 足够余额来转账
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261354003.png)
* 解法1：synchronized 范围应扩大至代理方法调用
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262320739.png)
* 解法2：在数据库层面保证事务的原子性，使用 select … for update 替换 select
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262322626.png)
	当前读和快读
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303262328308.png)


---
## 5. Spring MVC 执行流程
**要求**
* 掌握 Spring MVC的执行流程
* 了解 Spring MVC的重要组件的作用

**概要**
我把整个流程分成三个阶段
* 准备阶段
* 匹配阶段
* 执行阶段

**准备阶段**
1. 在 **Web 容器**（如Tomcat，传统的SpringMVC和Spring整合时是由Tomcat等；或者在SpringBoot里，有可能是由Spring容器自身创建的）第一次用到 DispatcherServlet（核心类）的时候，**会创建其对象并执行 init 方法**
2. **init 方法内会创建 Spring Web 容器，并调用容器 refresh 方法**
3. refresh 过程中会创建并**初始化 SpringMVC 中的重要组件**， 例如 MultipartResolver（文件上传时处理Multipart这种格式的表单数据），HandlerMapping（请求映射），HandlerAdapter（负责调用控制器的方法来处理请求），HandlerExceptionResolver（处理调用控制器方法过程中出现的异常）、ViewResolver（控制器方法执行完毕，封装为一个ModelView对象……把名字解析为视图对象，内部根据不同实现跳转到执行引擎等执行渲染工作）（同一个类型可能有多个对象存在于Spring Web容器中） 等
4. ==容器初始化后，会将上一步初始化好的重要组件，赋值给 DispatcherServlet 的成员变量，留待后用==
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261354610.png)

**匹配阶段**
1. **用户发送的请求统一到达前端控制器 DispatcherServlet**，主要职责是作为请求的入口
2. DispatcherServlet 遍历所有 HandlerMapping ，**找到与当前路径匹配的处理器**
   ① HandlerMapping 有多个，每个 HandlerMapping 会返回不同的处理器对象，谁先匹配，**返回谁的处理器**（因为一个请求只能由一个处理器处理！！）。**其中能识别 @RequestMapping 的优先级最高**
   ② ==对应 @RequestMapping 的处理器是 HandlerMethod，它包含了控制器对象和控制器方法信息==
   ③ 其中==路径与处理器的映射关系在 HandlerMapping 初始化时就会建立好==
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261354759.png)
3. 但返回控制器还不够，信息还缺失，请求还可能匹配到一些拦截器对象。将 HandlerMethod 连同匹配到的拦截器（可能多个），作为一个整体**生成调用链对象 HandlerExecutionChain 返回**（现在就包含执行阶段所需的全部信息了）
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261354987.png)
4. 遍历HandlerAdapter 处理器适配器，**找到能处理和执行 HandlerMethod 的适配器对象**，开始调用
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261355630.png)

**调用阶段**
1. 执行HandlerExecutionChain中的拦截器 preHandle，返回值为真说明可以执行后续调用，否则说明被拦截、停下来。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261355349.png)
2. 假设所有preHandle都为真，则由 HandlerAdapter 调用 HandlerMethod
   ① 调用前处理不同类型的参数（多种多样）。把参数都准备好了，就通过Method进行反射调用，把Bean和参数传进去，就完成控制器方法的调用。
   ② 调用后处理不同类型的返回值（多种多样）。返回值处理完了，就分为两种情况。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261355105.png)
3. 第 2 步没有异常
   ① 将返回的结果统一封装为 ModelAndView
   ② 执行拦截器 postHandle 方法，由里向外。全部为true就处理ModelAndView
   ③ **解析视图得到 View 对象，进行视图渲染**。因为前面的调用可能仅仅返回了一个字符串的视图名称，要借助ViewResolver把视图的字符串名字解析为视图对象。接着，把模型数据根据视图的实现不同、先存储到不同的位置（比如JSP的实现，把模型数据取出来放到request作用域，并转发到JSP的视图，JSP的视图从作用域中取出模型数据，渲染生成HTML返回）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261356802.png)
4. 第 2 步有异常，进入 HandlerExceptionResolver 异常处理流程。按照优先级次序，谁先匹配到就用哪个异常处理器处理。处理完后还是执行第5步的afterCompletion方法（当然中间的postHandle方法就执行不了了）。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303261356602.png)
5. **最后都会以由内到外的顺序、执行拦截器的 afterCompletion 方法**
6. 如果控制器方法标注了 @ResponseBody 注解，则**在第 2 步就会（处理返回值时用一个MessageConverter消息转换器）生成 json 结果**，并**标记 ModelAndView 已处理，这样就不会执行第 3 步的视图渲染**

---
## 6. Spring注解
**要求**：掌握 Spring 常见注解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270032446.png)

> ***提示***
> * 注解的详细列表请参考：面试题-spring-注解.xmind
> * 下面列出了视频中重点提及的注解，考虑到大部分注解同学们已经比较熟悉了，仅对个别的**作简要说明**

Spring框架内部使用的，不给我们提供：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270032594.png)

**事务注解**：
* @EnableTransactionManagement：启用事务控制，会额外加载4个bean
  * BeanFactoryTransactionAttributeSourceAdvisor 事务切面类（一个切面由切点和通知组成）
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270044792.png)
  * TransactionAttributeSource 用来解析事务属性（看类和方法上是否加了@Transactional注解，解析其中的属性；即解析切点）
  ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270045969.png)
  * TransactionInterceptor 事务拦截器（相当于通知，切点就是@Transactional标注的方法或类，它们都会被拦截、被功能增强）
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270047765.png)
  * TransactionalEventListenerFactory 事务监听器工厂
* @Transactional：加在类上，表示这个类的所有public方法都受到事务控制；加在方法上也是一样。

遇到Enable注解看源码，真正干活的是上面的@Import注解：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270040902.png)
可以看到它是个配置类，Import加的是配置类就会把配置类连同「@Bean修饰的方法关联的Bean」加入容器。

和监控相关的注解日常没有用到：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270049450.png)

**核心**
- @AliasFor：给注解属性起别名（Spring内部用）
* @Order：控制Bean的执行顺序
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270052620.png)

下面用的较少：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270054040.png)


**切面**
* @EnableAspectJAutoProxy
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270106916.png)
  * 会加载 AnnotationAwareAspectJAutoProxyCreator，它是一个 **bean 后处理器**，用来在Bean创建时看是否与某个切点匹配、从而生成代理对象、把代理对象放入单例池、代替目标对象
  * 如果没有配置 @EnableAspectJAutoProxy，又需要用到代理（如事务）则会使用 InfrastructureAdvisorAutoProxyCreator 这个 bean 后处理器
  ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270104785.png)
- @EnableLoadTimeWeaving：不是基于代理模式生成AOP切面
- @Configurable：不是基于代理模式生成AOP切面
	
**组件扫描与配置类**（多且熟悉）
* @Component
* @Controller
* @Service
* @Repository 上面配合组件扫描，一旦被扫描到就会加入到Spring容器管理
* @ComponentScan：可和@Filter注解组合
* @Conditional：这是Spring注解。一是在组件扫描时，做条件装配，扫描到且符合条件才加入。二是，配置类中解析@Bean时也会同时去判断Conditional条件是否成立。
* @Configuration
		![800](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270247184.png)
	* **配置类其实相当于一个工厂**，**标注 @Bean 注解的方法相当于工厂方法**（静态工厂方法不需要创建工厂实例，只需要拿到工厂.class即可）
		![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270235141.png)
		![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270248510.png)
	* **@Bean 不支持方法重载，如果有多个重载方法，仅有一个能入选为工厂方法**，参数需要注入的值越多，就越能作为入选的工厂方法
		![760](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270251568.png)
	- 有时去掉@Configuration好像也能正常工作，生成Bean 
	* **@Configuration 默认会为标注的类生成代理对象**（其中做些手脚）（其属性proxyBeanMethods=false就不生成代理），其目的是==保证 @Bean 方法相互调用时，仍然能保证其单例特性==。要么手动注入：
	  ![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270255123.png)
	  要么互相调用。没有@Configuration配置时，多次调用会产生多个Bean：
	  ![350](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270258359.png)
	 打印MyConfig.calss发现它就是个代理：
	    ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270300301.png)
	 * ==@Configuration 中如果含有 BeanFactory 后处理器，则实例工厂方法会导致 MyConfig 提前创建，造成其依赖注入失败==，解决方法是改用静态工厂方法（推荐）；或直接为 @Bean 的方法参数依赖注入；或针对 Mapper 扫描，可以改用注解方式@MapperScan，而不用MapperScannerConfigurer
	   ![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270309880.png)
	   MapperScannerConfigurer（扫描器）是个BeanFactoryPostProcessor，在refresh流程的第五步invokeBeanFactoryPostProcessors执行所有BeanFactory后处理器对BeanFactory做功能增强，此时就要创建出如MapperScannerConfigurer的对象，而它是个成员方法，必须先创建MyConfig对象！！！但配置类不应该在这创建，提前创建就没有准备好、解析Autowired这些注解都没准备好！它应该在第11步创建：finishBeanFactoryInitialization，对Bean利用BeanPostProcessor进行功能增强！
	   ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270959586.png)
* @Bean
* @Import 
    * 四种用法
	① 引入单个 bean，在spring容器中名字是它的全路径
	② 引入一个配置类，连同该配置类和其中的所有@Bean都被引入
	③ **通过 Selector 引入一个实现了ImportSelector接口的类xxx，从而引入多个类**，但不会引入xxx
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271008934.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271014862.png)
	④ **通过 beanDefinition 注册器**引入一个实现了ImportBeanDefinitionRegistrar接口的类xxx，从而引入多个类（自由定制其BeanDefinition）（也可以在其中引入配置类……），但不会引入xxx
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271012301.png)
  * 解析规则（假设配置类MyConfig Import(OtherConfig.class)，且两个配置类中都有名为MyBean myBean() {...}的@Bean，则哪个会生效呢？
	* **同一配置类中，@Import 先解析  @Bean 后解析**
	* 同名定义，**默认后面解析的会覆盖前面解析的**
	  ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271022467.png)
	* 假设在不允许覆盖的情况下，如何能够让 MyConfig(主配置类) 的配置优先? (虽然覆盖方式能解决) 替换到自动配置中的默认配置？
	* ==采用 DeferredImportSelector，因为它最后工作，可以简单认为先解析 @Bean，再 Import==
	   ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271031601.png)
		
* @Lazy
	* 加在类上，表示此类延迟实例化（用到时创建）、初始化
	* ==加在方法参数或成员变量上，可解决循环依赖，此参数会以代理方式注入==
* @PropertySource：读取配置文件的信息，将其作为键值信息加入到Environment对象
* @PropertySources

**缓存**：
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270129017.png)

**依赖注入**
* @Autowired
* @Qualifier：同一类型有多个Bean，用@Qualifier根据名字做进一步区分
* @Value

> resource是jdk里面自带的不是spring的

**mvc mapping**
* @RequestMapping，可以派生多个注解如 @GetMapping 等
* @GetMapping
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270132115.png)
* @PutMapping
* @PostMapping
* @DeleteMapping
* @PatchMapping

**mvc rest**
* @RequestBody：处理请求体中的JSON数据，封装为Java对象
* @ResponseBody，把控制器方法返回的Java对象转换为JSON数据，写入到响应体。组合 @Controller =>  @RestController
* @ResponseStatus：控制响应状态。
* @RestController：类中所有方法返回的数据都会被转换为JSON数据写到响应体。

**mvc 统一处理**
* @ControllerAdvice：**统一处理**：异常处理方法、转换器方法，放到ControllerAdvice中。组合 @ResponseBody => @RestControllerAdvice
* @ExceptionHandler：方法上标注，说明是处理异常。放到普通的控制器中，是局部的异常处理；放到ControllerAdvice中是全局的异常处理

**mvc 参数**
- @RequestHeader：获取请求头中的参数信息
- @CookieValue：获取Cookie的值
* @PathVariable：获取请求路径中的参数值
* @RequestParam：获取请求参数的参数值，即 **?后面的键值参数信息**，也可以是表单中的参数。只要请求参数和方法参数对得上，就可以省略；最有用的在于可以设置默认值
* @MatrixVaraible：用的不多
* @RequestPart：用的不多

mvc 转换与格式化
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270218255.png)

mvc validation
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270218446.png)
mvc scope：
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270219921.png)

**mvc ajax**
* @CrossOrigin：解决AJAX请求的跨域问题，JS中出现的？


---
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270220082.png)
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270221615.png)

**boot auto**
* @SpringBootApplication
* @EnableAutoConfiguration
* @SpringBootConfiguration
	![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303270227270.png)

**boot condition**：都是对Spring中Conditional注解的扩展
- @ConditionalOnBean
* @ConditionalOnClass，classpath 下存在某个 class 时，条件才成立
* @ConditionalOnMissingBean，**beanFactory 内不存在某个 bean 时，条件才成立**
* @ConditionalOnCloudPlatform
* @ConditionalOnExpression
* @ConditionalOnJava
* @ConditionalOnJndi
* @ConditionalOnMissingClass
* @ConditionalOnNotWebApplication
* @ConditionalOnResource
* @ConditionalOnProperty，配置文件中存在某个 property（键、值）时，条件才成立

**boot properties**
* @ConfigurationProperties，会**将当前 bean 的属性与配置文件中的键值进行绑定**
* @EnableConfigurationProperties，启用功能，会添加两个较为重要的 bean
  * ConfigurationPropertiesBindingPostProcessor，（无非是添加）bean 后处理器，在 bean 初始化前调用下面的 binder
  * ConfigurationPropertiesBinder，真正执行绑定操作

---
## 7. SpringBoot 自动配置原理
**要求**
* 掌握 SpringBoot 自动配置原理（就是理解SpringBootApplication这个注解）

**自动配置原理**
@SpringBootApplication 是一个组合注解，由 @ComponentScan、@EnableAutoConfiguration 和 @SpringBootConfiguration 组成
1. ==@SpringBootConfiguration 与普通 @Configuration 相比，唯一区别是前者要求其定义的主配置类整个 app 中只出现一次==
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271039340.png)
2. @ComponentScan
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271112233.png)
   * excludeFilters - 用来在组件扫描时进行排除，也会**排除自动配置类**，classes=AutoConfigurationExcludeFilter.class避免在组件扫描时扫到自动配置类（因为要在EnableAutoConfiguration中import进来）
	  ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271117272.png)
	
1. @EnableAutoConfiguration 也是一个**组合注解**，由下面注解组成
	* @AutoConfigurationPackage – 用来记住「被注解的类的包名」作为扫描的起始包
	   ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271219183.png)
	* @Import(AutoConfigurationImportSelector.class) 用来**加载 `META-INF/spring.factories` 中的自动配置类**（应该说是默认配置）

**为什么不使用 @Import 直接引入自动配置类**
有两个原因：
1. 让主配置类和自动配置类变成了**强耦合**，主配置类不应该知道有哪些从属配置
2. 直接用 `@Import(自动配置类.class)` ，引入的配置解析优先级较高，==自动配置类的解析应该在主配置没提供时**作为默认配置**==

因此，采用了 `@Import(AutoConfigurationImportSelector.class)`
* 由 `AutoConfigurationImportSelector.class` 去读取 `META-INF/spring.factories` 中的自动配置类（SpringBoot自带的所有配置类），实现了弱耦合。
* 另外 `AutoConfigurationImportSelector.class` 实现了 DeferredImportSelector 接口，**让自动配置的解析晚于主配置的解析**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303271237120.png)

---
## 8. Spring 中的设计模式
**要求**
* 掌握 Spring 中常见的设计模式

**1. Spring 中的 Singleton**
请大家区分 singleton pattern 与 Spring 中的 singleton bean
* 根据单例模式的目的 ==*Ensure a class only has one instance, and provide a global point of access to it* ==
* 显然 Spring 中的 singleton bean 并非实现了单例模式，**singleton bean 只能保证每个容器内，相同 id 的 bean 单实例**
* 当然 Spring 中也用到了单例模式，例如
  * org.springframework.transaction.TransactionDefinition#withDefaults
  * org.springframework.aop.TruePointcut#INSTANCE
  * org.springframework.aop.interceptor.ExposeInvocationInterceptor#ADVISOR
  * org.springframework.core.annotation.AnnotationAwareOrderComparator#INSTANCE
  * org.springframework.core.OrderComparator#INSTANCE

**2. Spring 中的 Builder**
定义 *Separate the construction of a complex object from its representation so that the same construction process can create different representations* 

它的主要亮点有三处：
1. 较为灵活的构建产品对象
2. 在不执行最后 build 方法前，产品对象都不可用
3. **构建过程采用链式调用，看起来比较爽**

Spring 中体现 Builder 模式的地方（builder结尾）：
* org.springframework.beans.factory.support.BeanDefinitionBuilder
* org.springframework.web.util.UriComponentsBuilder
* org.springframework.http.ResponseEntity.HeadersBuilder
* org.springframework.http.ResponseEntity.BodyBuilder

**3. Spring 中的 Factory Method**
定义 *Define an interface for creating an object, but ==let subclasses decide which class to instantiate==. Factory Method lets a class defer instantiation to subclasses* 

接口和实现分离，推迟到子类取实例化。根据上面的定义，==Spring 中的 ApplicationContext 与 BeanFactory 中的 getBean 都可以视为工厂方法==，它隐藏了 bean （产品）的创建过程和具体实现

Spring 中其它工厂：
* org.springframework.beans.factory.FactoryBean
* **@Bean 标注的静态方法及实例方法**
* ObjectFactory 及 ObjectProvider

前两种工厂主要封装第三方的 bean 的创建过程，==后两种工厂可以推迟 bean 创建，解决循环依赖及单例注入多例等问题==
**4. Spring 中的 Adapter**
定义 *Convert the interface of a class into another interface clients expect. Adapter lets classes work together that couldn't otherwise because of incompatible interfaces* 

典型的实现有两处：
* org.springframework.web.servlet.HandlerAdapter – 因为**控制器实现有各种各样**，比如有
  * 大家熟悉的 **@RequestMapping 标注的控制器实现**
  * 传统的基于 Controller 接口（不是 @Controller注解啊）的实现
  * 较新的基于 RouterFunction 接口的实现
  * 它们的处理方法都不一样，为了**统一调用**，必须适配为 HandlerAdapter 接口
* org.springframework.beans.factory.support.DisposableBeanAdapter – **因为销毁方法多种多样，因此都要适配为 DisposableBean 来统一调用销毁方法** 

**5. Spring 中的 Composite**
定义 *Compose objects into tree structures to represent part-whole hierarchies. Composite lets clients treat individual objects and compositions of objects uniformly* 

典型实现有：
* org.springframework.web.method.support.HandlerMethodArgumentResolverComposite
* org.springframework.web.method.support.HandlerMethodReturnValueHandlerComposite
* org.springframework.web.servlet.handler.HandlerExceptionResolverComposite
* org.springframework.web.servlet.view.ViewResolverComposite

composite 对象的作用是，**将分散的调用集中起来，统一调用入口**，它的特征是，与具体干活的实现实现同一个接口，当调用 composite 对象的接口方法时，其实是委托具体干活的实现来完成

**6. Spring 中的 Decorator** ：对一个对象动态增加功能，灵活替代（扩展功能）的子类（因为它要继承父类的方法，不太灵活？）
定义 *Attach additional responsibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality* 

典型实现：
* org.springframework.web.util.ContentCachingRequestWrapper

**7. Spring 中的 Proxy**
定义 *Provide a surrogate or placeholder for another object to control access to it* 
装饰器模式注重的是功能增强，避免以子类继承方式进行功能扩展，而**代理模式更注重控制对目标的访问**（真正对目标进行增强，Spring中用通知Advice）
静态代理，就是装饰器模式？
典型实现：
* org.springframework.aop.framework.JdkDynamicAopProxy
* org.springframework.aop.framework.ObjenesisCglibAopProxy

**8. Spring 中的 Chain of Responsibility**
定义 *Avoid coupling the sender of a request to its receiver by giving more than one object a chance to handle the request. Chain the receiving objects and pass the request along the chain until an object handles it* 

典型实现：
* org.springframework.web.servlet.HandlerInterceptor

**9. Spring 中的 Observer**
定义 *Define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically* 

典型实现（解耦）：
* org.springframework.context.**ApplicationListener**
* org.springframework.context.event.**ApplicationEventMulticaster**
* org.springframework.context.**ApplicationEvent**

**10. Spring 中的 Strategy**
定义 *Define a family of algorithms, encapsulate each one, and make them interchangeable. Strategy lets the algorithm vary independently from clients that use it* 

典型实现：
* org.springframework.beans.factory.support.InstantiationStrategy
* org.springframework.core.annotation.MergedAnnotations.SearchStrategy
* org.springframework.boot.autoconfigure.condition.SearchStrategy

**11. Spring 中的 Template Method**
定义 *Define the skeleton of an algorithm in an operation, ==deferring some steps to subclasses==. Template Method lets subclasses redefine certain steps of an algorithm without changing the algorithm's structure* 

典型实现：
* **大部分以 Template 命名的类**，如 JdbcTemplate，TransactionTemplate
* 很多以 Abstract 命名的类，如 AbstractApplicationContext
