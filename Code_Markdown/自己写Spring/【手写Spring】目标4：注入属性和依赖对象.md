> 超卖、掉单、幂等，你的程序总是不抗揍！
> 想想，运营已经对外宣传了七八天的活动，满心欢喜的等着最后一天页面上线对外了，突然出现了一堆异常、资损、闪退，而用户流量稍纵即逝，最后想死的心都有！
> 就程序 Bug 来讲，会包括产品 PRD 流程上的 Bug、运营配置活动时候的 Bug、研发开发时功能实现的 Bug、测试验证时漏掉流程的 Bug、上线过程中运维服务相关配置的 Bug，而这些其实都可以通过制定的流程规范和一定的研发经验积累，慢慢尽可能减少。
> 而另外一类是沟通留下的 Bug，通常情况下业务提需求、产品定方案、研发做实现，最终还要有 UI、测试、运营、架构等等各个环节的人员参与到一个项目的承接、开发到上线运行，而在这一群人需要保持一个统一的信息传播其实是很难的。比如在项目开发中期，运营给产品说了一个新增的需求，产品觉得功能也不大，随即找到对应的前端研发加个逻辑，但没想到可能也影响到了后端的开发和测试的用例。最后功能虽然是上线了，可并不在整个产研测的需求覆盖度范围里，也就隐形的埋下了一个坑。
> 所以，如果你想让你的程序很抗揍，接的住农夫三拳，那么你要做的就不只是一个单纯的搬砖码农！

### 1. 目标
这几章节都完成了什么，包括：
- 实现一个容器、
- 定义和注册Bean、
- 实例化 Bean，
- **按照是否包含构造函数实现不同的实例化策略**，

那么在创建对象实例化这我们还缺少什么？其实还缺少一个关于类中是否有属性的问题，==如果有类中包含属性，那么在实例化的时候就需要把属性信息填充上，这样才是一个完整的对象创建==。
- 对于属性的填充不只是 int、Long、String，
- 还包括还没有实例化的对象属性，都需要在 Bean 创建时进行填充操作。
- **不过这里暂时不会考虑 Bean的循环依赖，否则会把整个功能实现撑大**，这样学习时就把握不住了，待后续陆续先把核心功能实现后，再逐步完善

### 2. 设计
鉴于**属性填充是在 Bean 使用 newInstance 或者 Cglib 创建后，开始补全属性信息**，那么就可以在类 **AbstractAutowireCapableBeanFactory 的 createBean 方法中添加补全属性方法**。这部分在过程中也可以对照Spring 源码学习，这里的实现也是 Spring 的简化版，后续对照学习更易于理解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241026544.png)

属性填充要在类实例化创建之后，也就是**需要在AbstractAutowireCapableBeanFactory 的 createBean 方法中添加applyPropertyValues 操作**。

由于我们需要在创建 Bean 时填充属性操作，那么就**需要在 bean 定义BeanDefinition 类中，添加 PropertyValues 信息**。——不能通过反射获取吗？

另外==填充属性信息还**包括了 Bean 的对象类型**，也就是需要再定义一个BeanReference，里面其实就是一个**简单的 Bean 名称**，在具体的实例化操作时进行递归创建和填充==，与 Spring 源码实现一样。
Spring 源码中 BeanReference 是一个接口。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241128722.png)

本章节中需要新增加 3 个类，BeanReference(类引用)、PropertyValue(属性值)、PropertyValues(属性集合)，分别用于类和其他类型属性填充操作。

另外**改动的类主要是 AbstractAutowireCapableBeanFactory，在 createBean 中补全属性填充部分**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241130755.png)
### 定义属性
```java
package com.memcpy0.springframework.beans;  
  
/**  
 * 属性信息  
 */  
public class PropertyValue {  
    private final String name;  
    private final Object value;  
    public PropertyValue(String name, Object value) {  
        this.name = name;  
        this.value = value;  
    }  
  
    public String getName() { return name; }  
    public Object getValue() { return value; }  
}
```
### 定义属性集合
这两个类的作用就是创建出一个用于传递类中属性信息的类，因为属性可能会有很多，所以还需要定义一个集合包装下
```java
package com.memcpy0.springframework.beans;  
  
import java.util.ArrayList;  
import java.util.List;  
  
public class PropertyValues {  
    private final List<PropertyValue> propertyValueList = new ArrayList<>();  
    public void addPropertyValue(PropertyValue pv) {  
        this.propertyValueList.add(pv);  
    }  
  
    public PropertyValue[] getPropertyValues() {  
        return this.propertyValueList.toArray(new PropertyValue[0]);  
    }  
  
    public PropertyValue getPropertyValue(String propertyName) {  
        for (PropertyValue pv : this.propertyValueList) {  
            if (pv.getName().equals(propertyName)) {  
                return pv;  
            }  
        }  
        return null;  
    }  
}
```
### 定义BeanReference
```java
package com.memcpy0.springframework.beans.factory.config;  
  
/**  
 * Bean的引用  
 */  
public class BeanReference {  
    private final String beanName;  
    public BeanReference(String beanName) {  
        this.beanName = beanName;  
    }  
    public String getBeanName() { return beanName; }  
}
```
### 3. Bean定义补全
在Bean注册的过程中，需要传递Bean的信息，前几个章节的测试中都有体现：new BeanDefinition(UserService.class, propertyValues); 
所以，==为了把属性都交给Bean定义，这里填充了PropertyValues属性，同时把两个构造函数做了一些简单的优化，避免后面for循环时还得判断属性填充是否为空==：
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.PropertyValues;  
  
/**  
 * 存储Bean的类信息  
 */  
public class BeanDefinition {  
    private Class beanClass;  
    private PropertyValues propertyValues;  
  
    public BeanDefinition(Class beanClass) {  
        this.beanClass = beanClass;  
        this.propertyValues = new PropertyValues();  
    }  
    public BeanDefinition(Class beanClass, PropertyValues propertyValues) {  
        this.beanClass = beanClass;  
        this.propertyValues = propertyValues != null ? propertyValues : new PropertyValues();  
    }  
    public Class getBeanClass() {return beanClass; }  
    public void setBeanClass(Class beanClass) {  
        this.beanClass = beanClass;  
    }  
    public PropertyValues getPropertyValues() { return propertyValues; }  
    public void setPropertyValues(PropertyValues propertyValues) { this.propertyValues = propertyValues; }  
}
```
### 4. Bean属性填充
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.PropertyValues;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanReference;  
import cn.hutool.core.bean.BeanUtil;  
  
import java.lang.reflect.Constructor;  
  
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory {  
//    private InstantiationStrategy instantiationStrategy = new CglibSubclassingInstantiationStrategy();  
    private InstantiationStrategy instantiationStrategy = new SimpleInstantiationStrategy();  
  
    @Override  
    protected Object createBean(String beanName, BeanDefinition beanDefinition, Object[] args) throws BeansException {  
        Object bean = null;  
        try {  
//            bean = beanDefinition.getBeanClass().newInstance();  
            bean = createBeanInstance(beanDefinition, beanName, args);  
            // 给Bean填充属性  
            applyPropertyValues(beanName, bean, beanDefinition);  
        } catch (Exception e) {  
            throw new BeansException("Instantiation of bean failed", e);  
        }  
        addSingleton(beanName, bean); // 第一次创建Bean时要加入单例池  
        return bean;  
    }  
  
    /**  
     * 找到合适的构造函数，并调用实例化策略的instantiate方法创建Bean实例  
     * @param beanDefinition  
     * @param beanName  
     * @param args  
     * @return  
     */  
    protected Object createBeanInstance(BeanDefinition beanDefinition, String beanName, Object[] args) {  
        Constructor constructorToUse = null;  
        Class<?> beanClass = beanDefinition.getBeanClass();  
        Constructor<?>[] declaredConstructors = beanClass.getDeclaredConstructors(); // 从Class对象获取所有构造函数  
        for (Constructor ctor : declaredConstructors) { // 找到合适的构造函数，传给实例化策略  
            if (null != args && ctor.getParameterTypes().length == args.length) { // 这里对比起来简化了  
                constructorToUse = ctor;  
                break;            }  
        }  
        return getInstantiationStrategy().instantiate(beanDefinition, beanName, constructorToUse, args);  
    }  
  
    /**  
     * Bean属性填充  
     * @return  
     */  
    protected void applyPropertyValues(String beanName, Object bean, BeanDefinition beanDefinition) {  
        try {  
            PropertyValues propertyValues = beanDefinition.getPropertyValues();  
            for (PropertyValue propertyValue : propertyValues.getPropertyValues()) {  
                String name = propertyValue.getName();  
                Object value = propertyValue.getValue();  
                if (value instanceof BeanReference) { // Bean A依赖于Bean B, 获取B的实例化  
                    BeanReference beanReference = (BeanReference) value;  
                    value = getBean(beanReference.getBeanName()); // 通过Bean B的名字递归获取实例  
                }  
                // 属性填充  
                BeanUtil.setFieldValue(bean, name, value);  
            }  
        } catch (Exception e) {  
            throw new BeansException("Error setting property values: " + beanName);  
        }  
    }  
    public InstantiationStrategy getInstantiationStrategy() {  
        return instantiationStrategy;  
    }  
  
    public void setInstantiationStrategy(InstantiationStrategy instantiationStrategy) {  
        this.instantiationStrategy = instantiationStrategy;  
    }  
}
```
这个类的内容稍微有点长，主要包括三个方法：createBean、createBeanInstance、applyPropertyValues，这里我们主要关注 createBean 的方法中调用的applyPropertyValues 方法。

**在 applyPropertyValues 中，通过对 beanDefinition.getPropertyValues() 循环进行属性填充操作**，==如果遇到的是 BeanReference，那么就需要递归获取 Bean 实例，调用 getBean 方法==。

当把依赖的 Bean 对象创建完成后，会递归回现在属性填充中。这里需要注意我们并没有去处理循环依赖的问题，这部分内容较大，后续补充。==BeanUtil.setFieldValue(bean, name, value) 是 hutool-all 工具类中的方法，你也可以自己实现==。

### 测试
```java
package com.memcpy0.bean;  
  
import java.util.HashMap;  
import java.util.Map;  
  
public class UserDao {  
  
    private static Map<String, String> hashMap = new HashMap<>();  
  
	static {  
	    hashMap.put("10001", "张三");  
	    hashMap.put("10002", "李四");  
	    hashMap.put("10003", "王五");  
	}
    public String queryUserName(String uId) {  
        return hashMap.get(uId);  
    }  
}
```
Dao、Service，是我们平常开发经常使用的场景。在 UserService （属性有uId和UserDAO）中注入UserDao，这样就能体现出 Bean 属性的依赖了。
```java
package com.memcpy0.bean;  
  
public class UserService {  
    private String uId;  
    private UserDao userDao;  
  
    public UserService() {}  
  
    public String getuId() { return uId; }  
  
    public void setuId(String uId) { this.uId = uId; }  
  
    public UserDao getUserDao() { return userDao; }  
  
    public void setUserDao(UserDao userDao) { this.userDao = userDao; }  
    public void queryUserInfo() {  
        System.out.println("查询用户信息：" + userDao.queryUserName(uId));  
    }  
}
```
测试用例如下：
```java
package com.memcpy0;  
  
import com.memcpy0.bean.UserDao;  
import com.memcpy0.bean.UserService;  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.PropertyValues;  
import com.memcpy0.springframework.beans.factory.BeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanReference;  
import com.memcpy0.springframework.beans.factory.support.DefaultListableBeanFactory;  
import org.junit.jupiter.api.Test;  
import org.springframework.boot.test.context.SpringBootTest;  
  
@SpringBootTest  
class ApiTest {  
  
   @Test  
   void contextLoads() {  
  
   }  
  
   @Test  
   public void testBeanFactory() {  
      // 1. 初始化BeanFactory  
      DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();  
      // 2. 注册Bean  
      beanFactory.registerBeanDefinition("userDao", new BeanDefinition(UserDao.class)); // 这里先不管UserDao的hashMap属性  
  
      // UserService设置属性[uId, userDao]  
      PropertyValues propertyValues = new PropertyValues();  
      propertyValues.addPropertyValue(new PropertyValue("uId", "10001"));  
      propertyValues.addPropertyValue(new PropertyValue("userDao", new BeanReference("userDao"))); // 依赖另外一个Bean  
  
      // UserService 注入Bean  
      BeanDefinition beanDefinition = new BeanDefinition(UserService.class, propertyValues); // 不用自己实例化Bean去传入了  
      beanFactory.registerBeanDefinition("userService", beanDefinition); // 注册Bean定义  
  
      // 4. 获取bean from Singleton  
      UserService userService = (UserService) beanFactory.getBean("userService"); // 无参构造初始化  
      userService.queryUserInfo();  
  
//    if (userService == userService_singleton) System.out.println("same instance");  
   }  
}
```
与直接获取 Bean 对象不同，这次我们**还需要先把 userDao 注入到 Bean 容器中**。`beanFactory.registerBeanDefinition("userDao", new BeanDefinition(UserDao.class));`

接下来就是属性填充的操作了，**一种是普通属性** new PropertyValue("uId", "10001")，**另外一种是对象属性** new PropertyValue("userDao",new BeanReference("userDao"))

接下来的操作就简单了，只不过是正常获取 userService 对象，调用方法即可。

从测试结果看我们的属性填充已经起作用了，因为只有属性填充后，才能调用到 Dao 方法，如：userDao.queryUserName(uId)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241613935.png)

那么我们在看看 Debug 调试的情况下，有没有进入到实现的 Bean 属性填充中，如下：我们看到已经开始进行属性填充操作了，当发现属性是 BeanReference 时，则需要获取创建 Bean 实例。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241615803.png)

### 总结 
在本章节中把 AbstractAutowireCapableBeanFactory 类中的创建对象功能又做了扩充，**依赖于是否有构造函数的实例化策略**完成后，**开始补充 Bean 属性信息**。==当遇到 Bean 属性为 Bean 对象时，需要递归处理。最后在属性填充时需要用到反射操作，也可以使用一些工具类处理==。

每一个章节的功能点我们都在循序渐进的实现，这样可以更好的接受关于Spring 中的设计思路。尤其是在一些已经开发好的类上，怎么扩充新的功能时的设计更为重要。学习编程有的时候学习思路设计要比仅仅是做简单实现，更能提升编程思维。

到这一章节**关于 Bean 的创建操作**就开发完成了，接下来需要整个框架的基础上完成资源属性的加载，就是我们需要**去动 Xml 配置了**，让我们这小框架越来越像 Spring。另外在框架实现的过程中所有的类名都会参考 Spring 源码，以及相应的设计实现步骤也是与 Spring 源码中对应，只不过会简化一些流程，==但你可以拿相同的类名，去搜到每一个功能在 Spring 源码中的实现==。

---
## 资源加载器解析文件、注册对象
> 你写的代码，能接的住产品加需求吗？接，是能接的，接几次也行，哪怕就一个类一片的 if...else 也可以！但接完成什么样可就不一定了，会不会出事故也不是能控制住的。那出事故时，你说因为我写 if...else 多了导致代码烂了，但可是你先动的手啊：你说的需求还得加、你说的老板让上线、你说的合同都签了，搬砖码农的我没办法，才以堆代码平需求，需求太多不好搞，我才以搬砖平需求！诸侯不服，我才以兵服诸侯，你不服，我就打到你服！

但代码烂了有时候并不是因为需求加的快、也不是着急上线。==因为往往在承接产品需求的前几次，一个功能逻辑的设计并不会太复杂，也不会有多急迫，甚至会留出让你做设计、做评审、做开发的时间==，如果这个时候仍不能把以后可能会发生的事情评估到需求里，那么导致代码的混乱从一开始就已经埋下了，以后只能越来越乱！

承接需求并能把它做好，这来自于对需求的理解，产品场景开发的经验以及对代码实践落地的把控能力等综合多方面因素的结果。就像你现在做的开发中，你的代码有哪些是经常变化的，有哪些是固定通用的，有哪些是负责逻辑拼装的、有哪些是来做核心实现的。那么现在如果你的核心共用层做了频繁变化的业务层包装，那么肯定的说，你的代码即将越来越乱，甚至可能埋下事故的风险！

在我们实现的 Spring 框架中，每一个章节都会结合上一章节继续扩展功能，就像每一次产品都在加需求一样，那么在学习的过程中可以承上启下的对照和参考，==看看每一个模块的添加都是用什么逻辑和技术细节实现的==。这些内容的学习，会非常有利于你以后在设计和实现，自己承接产品需求时做的具体开发，代码的质量也会越来越高，越来越有扩展性和可维护性。

### 2. 目标
在完成 Spring 的框架雏形后，现在我们可以通过单元测试进行手动操作 Bean对象的定义、注册和属性填充，以及最终获取对象调用方法。但这里会有一个问题，就是如果实际使用这个 Spring 框架，是**不太可能让用户通过手动方式创建的**，而是最好能通过配置文件的方式简化创建过程。需要完成如下操作：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241953911.png)
- 如图中我们需要把步骤：2、3、4 整合到 Spring 框架中，**通过 Spring 配置文件的方式将 Bean 对象实例化**。
- 接下来我们就需要在现有的 Spring 框架中，添加**能解决 「Spring 配置的读取、解析、注册 Bean 」的操作**。

### 三、设计
依照本章节的需求背景，我们需要在现有的 Spring 框架雏形中添加一个**资源解析器**，
- 也就是**能读取 classpath、本地文件和云文件的配置内容**。这些配置内容就是像使用 Spring 时配置的 Spring.xml 一样，里面会包括 **Bean 对象的描述和属性信息**。 
- 在读取配置文件信息后，接下来就是**对配置文件中的Bean 描述信息解析后进行注册操作**，把 Bean 对象注册到 Spring 容器中。

整体设计结构如下图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241956648.png)

- **资源加载器属于相对独立的部分**，它位于 Spring 框架核心包下的 **IO 实现内容**，主要用于处理 Class、本地和云环境中的文件信息。
- 当资源可以加载后，接下来就是**解析**和**注册 Bean 到 Spring 中的操作**，==这部分实现需要和 DefaultListableBeanFactory 核心类结合起来，因为你所有的解析后的**注册动作，都会把 Bean 定义信息放入到这个类中**==。
- 那么在实现的时候就设计好**接口的实现层级关系**，包括我们需要定义出 **Bean 定义的读取接口 BeanDefinitionReader** 以及**做好对应的实现类**，==在实现类中完成对 Bean 对象的解析和注册==。


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304242125929.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304242128034.png)
为了能**把 Bean 的定义、注册和初始化**交给 Spring.xml 配置化处理，那么就需要实现两大块内容，分别是：**资源加载器、xml 资源处理类**，实现过程主要以**对接口 Resource、ResourceLoader 的实现**，而另外**BeanDefinitionReader 接口则是对资源的具体使用**，将配置信息注册到Spring 容器中去。
- 在 Resource 的资源加载器的实现中包括了，ClassPath、系统文件、云配置文件，这三部分与 Spring 源码中的设计和实现保持一致，**最终在DefaultResourceLoader 中做具体的调用**。
- 接口：BeanDefinitionReader、抽象类：AbstractBeanDefinitionReader、实现类：XmlBeanDefinitionReader，这三部分内容主要是合理清晰的处理了**资源读取后的注册 Bean 容器操作**。<b><font color="red">这样我们就不用自己注册BeanDefinition了！读取了资源（配置文件）后就在这里面直接注册，不然让外面或BeanDefinitionRegistry中注册吗？</font></b>
- ==接口管定义，抽象类处理**非接口功能外的注册 Bean 组件填充**，最终实现类即可只关心具体的业务实现

另外还参考 Spring 源码，做了相应接口的集成和实现的关系，虽然这些接口目前还并没有太大的作用，但随着框架的逐步完善，它们也会发挥作用。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304242231080.png)
- ListableBeanFactory，是一个扩展 Bean 工厂接口的接口，新增加了getBeansOfType、getBeanDefinitionNames() 方法，在 Spring 源码中还有其他扩展方法。
- HierarchicalBeanFactory，在 Spring 源码中**它提供了可以获取父类 BeanFactory 的方法**，属于是一种扩展工厂的层次子接口。Sub-interface implemented by bean factories that can be part of a hierarchy. 由bean工厂实现的子接口，可以是层次结构的一部分。

BeanDefinitionRegistry：
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
public interface BeanDefinitionRegistry {  
    /**  
     * 向注册表中注册 BeanDefinition  
     *     
     * @param beanName  
     * @param beanDefinition  
     */  
    void registerBeanDefinition(String beanName, BeanDefinition beanDefinition);  
  
    /**  
     * 使用Bean名称查询BeanDefinition  
     *     
     * @param beanName  
     * @return  
     * @throws BeansException  
     */    
    BeanDefinition getBeanDefinition(String beanName) throws BeansException;  
  
    /**  
     * 判断是否包含指定名称的BeanDefinition  
     * @param beanName  
     * @return  
     */  
    boolean containsBeanDefinition(String beanName);  
  
    /**  
     * Return the names of all beans defined in this registry.    
     *     
     * 返回注册表中所有的Bean名称  
     */  
    String[] getBeanDefinitionNames();  
  
}
```

BeanFactory，已经存在的 Bean 工厂接口用于获取 Bean 对象，这次新增加了按照类型获取 Bean 的方法：`<T> T getBean(String name, Class<T> requiredType)`
```java
package com.memcpy0.springframework.beans.factory;  
  
import com.memcpy0.springframework.beans.BeansException;  
import org.springframework.context.annotation.Bean;  
  
public interface BeanFactory {  
    Object getBean(String beanName) throws BeansException;  
    Object getBean(String beanName, Object... args) throws BeansException;  
    <T> T getBean(String name, Class<T> requiredType) throws BeansException;  
}
```
ListableBeanFactory：可以枚举其所有bean实例的bean工厂实现，而不是根据客户端的请求逐个尝试按名称查找bean。预加载所有bean定义的BeanFactory实现（例如基于XML的工厂）可以实现这个接口。
```java
package com.memcpy0.springframework.beans.factory;  
  
  
import com.memcpy0.springframework.beans.BeansException;  
  
import java.util.Map;  
  
/**  
 * Extension of the {@link BeanFactory} interface to be implemented by bean factories  
 * that can enumerate all their bean instances, rather than attempting bean lookup 
 * by name one by one as requested by clients. BeanFactory implementations that 
 * preload all their bean definitions (such as XML-based factories) may implement 
 * this interface. 
 * BeanFactory接口的扩展，由Bean工厂实现，可以枚举其所有的Bean实例，而不是据客户端的请求逐个尝试按名称查找bean  
 * 预加载所有bean定义的BeanFactory实现（例如基于XML的工厂）可以实现这个接口。  
 * <p>  
 */  
public interface ListableBeanFactory extends BeanFactory {  
    /**  
     * 按照类型返回 Bean 实例  
     * @param type  
     * @param <T>  
     * @return  
     * @throws BeansException  
     */    
    <T> Map<String, T> getBeansOfType(Class<T> type) throws BeansException;  
  
    /**  
     * 返回注册表中所有的Bean名称  
     * Return the names of all beans defined in this registry.  
     * @return  
     */  
    String[] getBeanDefinitionNames();  
}
```
AutowireCapableBeanFactory，是一个**自动化处理 Bean 工厂配置**的接口，目前案例工程中还没有做相应的实现，后续逐步完善。
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.factory.BeanFactory;  
  
/**  
 * Extension of the {@link com.memcpy0.springframework.beans.factory.BeanFactory}  
 * interface to be implemented by bean factories that are capable of 
 * autowiring, provided that they want to expose this functionality for * existing bean instances. 
 * 由进行自动装配的Bean工厂实现，前提是他们希望为现有的Bean实例公开此功能  
 */  
public interface AutowireCapableBeanFactory extends BeanFactory {  
}
```
- ConfigurableBeanFactory，可获取 BeanPostProcessor、BeanClassLoader 等的一个**配置化接口**。
- ConfigurableListableBeanFactory，提供**分析和修改Bean定义、以及==提前实例化单例==的操作接口**，不过目前只有一个 getBeanDefinition 方法。
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.factory.HierarchicalBeanFactory;  
  
/**  
 * Configuration interface to be implemented by most bean factories. Provides 
 * facilities to configure a bean factory, in addition to the bean factory 
 * client methods in the {@link com.memcpy0.springframework.beans.factory.BeanFactory}  
 * interface. 
 * 将由大多数Bean工厂实现的配置接口，提供了配置Bean工厂的方法，还有Bean Factory客户端方法（即一系列getBean方法）  
 */  
public interface ConfigurableBeanFactory extends HierarchicalBeanFactory, SingletonBeanRegistry {  
    String SCOPE_SINGLETON = "singleton";  
  
    String SCOPE_PROTOTYPE = "prototype";  
}
```


此外修改的还有：返回的不仅是Object，还可以是T
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.factory.BeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.BeansException;  
  
public abstract class AbstractBeanFactory extends DefaultSingletonBeanRegistry implements BeanFactory {  
    @Override  
    public Object getBean(String beanName) throws BeansException {  
//        Object bean = getSingleton(beanName);  
//        if (bean != null) {  
//            return bean;  
//        }  
//        BeanDefinition beanDefinition = getBeanDefinition(beanName);  
//        return createBean(beanName, beanDefinition);  
        return doGetBean(beanName, null);  
    }  
  
    @Override  
    public Object getBean(String beanName, Object... args) throws BeansException {  
        return doGetBean(beanName, args);  
    }  
  
    @Override  
    public <T> T getBean(String beanName, Class<T> requiredType) throws BeansException {  
        return (T) getBean(beanName);  
    }  
  
    protected <T> T doGetBean(final String beanName, final Object[] args) {  
        Object bean = getSingleton(beanName);  
        if (bean != null) {  
            return (T) bean;  
        }  
        BeanDefinition beanDefinition = getBeanDefinition(beanName);  
        return (T) createBean(beanName, beanDefinition, args);  
    }  
    protected abstract BeanDefinition getBeanDefinition(String beanName) throws BeansException;  
    protected abstract Object createBean(String beanName, BeanDefinition beanDefinition, Object[] args) throws BeansException;  
}
```
DefaultListableBeanFactory：
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
import java.util.HashMap;  
import java.util.Map;  
  
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory  
        implements BeanDefinitionRegistry, ConfigurableListableBeanFactory {  
    private Map<String, BeanDefinition> beanDefinitionMap = new HashMap<>();  
  
    /**  
     * 获取BeanDefinition  
     * @param beanName  
     * @return  
     * @throws BeansException  
     */    
    @Override  
    public BeanDefinition getBeanDefinition(String beanName) throws BeansException {  
        BeanDefinition beanDefinition = beanDefinitionMap.get(beanName);  
        if (beanDefinition == null)  
            throw new BeansException("No bean named '" + beanName + "' is defined");  
        return beanDefinition;  
    }  
  
    @Override  
    public boolean containsBeanDefinition(String beanName) {  
        return beanDefinitionMap.containsKey(beanName);  
    }  
  
    @Override  
    public String[] getBeanDefinitionNames() {  
        return beanDefinitionMap.keySet().toArray(new String[0]);  
    }  
  
    /**  
     * 注册BeanDefinition  
     * @param beanName  
     * @param beanDefinition  
     */  
    @Override  
    public void registerBeanDefinition(String beanName, BeanDefinition beanDefinition) {  
        beanDefinitionMap.put(beanName, beanDefinition);  
    }  
  
    /**  
     * 返回某种类型的全部Bean  
     * @param type  
     * @return  
     * @param <T>  
     * @throws BeansException  
     */    
    @Override  
    public <T> Map<String, T> getBeansOfType(Class<T> type) throws BeansException {  
        Map<String, T> result = new HashMap<>();  
        beanDefinitionMap.forEach((beanName, beanDefinition) -> {  
            Class beanClass = beanDefinition.getBeanClass();  
            if (type.isAssignableFrom(beanClass))  
                result.put(beanName, (T) getBean(beanName));  
        });  
        return result;  
    }  
}
```
### 2. 资源加载接口定义和实现
在 Spring 框架下创建 core.io 核心包，在这个包中主要用于处理资源加载流。
```java
package com.memcpy0.springframework.beans.core.io;  
  
import java.io.IOException;  
import java.io.InputStream;  
  
public interface Resource {  
    InputStream getInputStream() throws IOException;  
}
```
**定义 Resource 接口，提供获取 InputStream 流的方法**，接下来再分别实现三种不同的流文件操作：classPath、FileSystem、URL

这一部分的实现是用于**通过 ClassLoader 读取 ClassPath 下的文件信息**，具体的读取过程主要是：classLoader.getResourceAsStream(path)
```java
package com.memcpy0.springframework.beans.core.io;  
  
import cn.hutool.core.lang.Assert;    
import com.memcpy0.springframework.util.ClassUtils;
  
import java.io.FileNotFoundException;  
import java.io.IOException;  
import java.io.InputStream;  
  
public class ClassPathResource implements Resource {  
    private final String path;  
    private ClassLoader classLoader;  
  
    public ClassPathResource(String path) {  
        this(path, (ClassLoader) null);  
    }  
  
    public ClassPathResource(String path, ClassLoader classLoader) {  
        Assert.notNull(path, "Path must not be null");  
        this.path = path;  
        this.classLoader = (classLoader != null ? classLoader : ClassUtils.getDefaultClassLoader());  
    }  
  
    @Override  
    public InputStream getInputStream() throws IOException {  
        InputStream is = classLoader.getResourceAsStream(path);  
        if (is == null)   
            throw new FileNotFoundException(this.path + " cannot be opened because it does not exist");  
        return is;  
    }  
}
```
这里的ClassUtils先获取当前线程的ContextClassLoader()，获取不到就拿ClassUtils.class的类加载器：
```java
package com.memcpy0.springframework.util;  
  
public class ClassUtils {  
    public static ClassLoader getDefaultClassLoader() {  
        ClassLoader cl = null;  
        try {  
            cl = Thread.currentThread().getContextClassLoader();  
        } catch (Throwable ex) {  
            // Cannot access thread context ClassLoader - falling back to system class loader...  
        }  
        if (cl == null) {  
            // No thread context class loader -> use class loader of this class.  
            cl = ClassUtils.class.getClassLoader();  
        }  
        return cl;  
    }  
}
```
文件系统：通过制定文件路径的方式读取文件信息：
```java
package com.memcpy0.springframework.beans.core.io;  
  
import java.io.File;  
import java.io.FileInputStream;  
import java.io.IOException;  
import java.io.InputStream;  
  
public class FileSystemResource implements Resource {  
    private final File file;  
    private final String path;  
  
    public FileSystemResource(File file) {  
        this.file = file;  
        this.path = file.getPath();  
    }  
      
    public FileSystemResource(String path) {  
        this.file = new File(path);  
        this.path = path;  
    }  
      
    @Override  
    public InputStream getInputStream() throws IOException {  
        return new FileInputStream(file);  
    }  
      
    public final String getPath() {  
        return this.path;  
    }  
}
```
URL：通过 HTTP 的方式读取云服务的文件，我们也可以把配置文件放到 GitHub 或者Gitee 上。
```java
package com.memcpy0.springframework.beans.core.io;  
  
import cn.hutool.core.lang.Assert;   
import java.io.IOException;  
import java.io.InputStream;  
import java.net.HttpURLConnection;  
import java.net.URL;  
import java.net.URLConnection;  
  
public class UrlResource implements Resource {  
    private final URL url;  
  
    public UrlResource(URL url) {  
        Assert.notNull(url,"URL must not be null");  
        this.url = url;  
    }  
  
    @Override  
    public InputStream getInputStream() throws IOException {  
        URLConnection conn = this.url.openConnection();  
        try {  
            return conn.getInputStream();  
        } catch (IOException ex) {  
            if (conn instanceof HttpURLConnection) {  
                ((HttpURLConnection) conn).disconnect();  
            }  
            throw ex;  
        }  
    }  
}
```
### 3. 包装资源加载器
==按照资源加载的不同方式，资源加载器可以把这些方式集中到统一的类服务下进行处理==，外部用户只需要传递资源地址即可，**简化使用**。

定义**获取资源接口**，里面传递 location 地址即可
```java
package com.memcpy0.springframework.beans.core.io;  
  
public interface ResourceLoader {  
  
    /**  
     * Pseudo URL prefix for loading from the class path: "classpath:"     */    String CLASSPATH_URL_PREFIX = "classpath:";  
  
    Resource getResource(String location);  
}
```
实现接口：**在获取资源的实现中，主要是把三种不同类型的资源处理方式进行了包装**，分为：
判断是否为 ClassPath、URL 以及文件。

虽然 DefaultResourceLoader 类实现的过程简单，但这也是设计模式约定的具体结果，像是这里不会让外部调用放知道过多的细节，而是仅关心具体调用结果即可。
```java
package com.memcpy0.springframework.beans.core.io;  
  
import cn.hutool.core.lang.Assert;  
import org.slf4j.LoggerFactory;  
  
import java.net.MalformedURLException;  
import java.net.URL;  
  
public class DefaultResourceLoader implements ResourceLoader {  
    @Override  
    public Resource getResource(String location) {  
        Assert.notNull(location, "Location must not be null");  
        if (location.startsWith(CLASSPATH_URL_PREFIX)) // 类路径加载  
            return new ClassPathResource(location.substring(CLASSPATH_URL_PREFIX.length()));  
        else {  
            try {  
                URL url = new URL(location);  
                return new UrlResource(url);  
            } catch (MalformedURLException e) {  
                return new FileSystemResource(location);  
            }  
        }  
    }  
}
```
### 4. Bean定义读取接口
这是一个 Simple interface for bean definition readers. 其实里面无非定义了几个方法，包括：getRegistry()、getResourceLoader()，以及三个加载 Bean 定义的方法。==需要注意 getRegistry()、getResourceLoader()，都是**用于提供给后面三个方法的工具**，加载和注册==，**这两个方法的实现会包装到抽象类中**，以免污染具体的接口实现方法。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;   
import com.memcpy0.springframework.core.io.Resource;  
import com.memcpy0.springframework.core.io.ResourceLoader;  
  
/**  
 * Simple interface for bean definition readers. */public interface BeanDefinitionReader {  
    BeanDefinitionRegistry getRegistry();  
    ResourceLoader getResourceLoader();  
    void loadBeanDefinitions(Resource resource) throws BeansException;  
    void loadBeanDefinitions(Resource... resources) throws BeansException;  
    void loadBeanDefinitions(String location) throws BeansException;  
}
```
### 5. Bean定义读取的抽象类实现
抽象类把 BeanDefinitionReader 接口的前两个方法全部实现完了，并提供了构造函数，让外部的调用使用方，**把 Bean 定义注入类，传递进来**。

这样在接口 BeanDefinitionReader 的具体实现类中，就可以**把解析后的 XML 文件中的 Bean 信息，注册到 Spring 容器去了**。以前我们是通过单元测试使用，**调用 BeanDefinitionRegistry 完成 Bean 的注册**，现在可以放到 XMl 中操作了
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.core.io.DefaultResourceLoader;  
import com.memcpy0.springframework.core.io.ResourceLoader;  
  
  
/**  
 * Abstract base class for bean definition readers which implement * the {@link BeanDefinitionReader} interface.  
 * <p>  
 */  
public abstract class AbstractBeanDefinitionReader implements BeanDefinitionReader {  
    private final BeanDefinitionRegistry registry;  
  
    private ResourceLoader resourceLoader;  
  
    protected AbstractBeanDefinitionReader(BeanDefinitionRegistry registry) {  
        this(registry, new DefaultResourceLoader());  
    }  
  
    public AbstractBeanDefinitionReader(BeanDefinitionRegistry registry, ResourceLoader resourceLoader) {  
        this.registry = registry;  
        this.resourceLoader = resourceLoader;  
    }  
  
    @Override  
    public BeanDefinitionRegistry getRegistry() { return registry; }  
    @Override  
    public ResourceLoader getResourceLoader() { return resourceLoader; }  
}
```
### 6. 解析XML处理Bean注册
XmlBeanDefinitionReader 类最核心的内容就是对 XML 文件的解析，把我们本来在代码中的操作、变成**通过解析 XML 来自动注册**的方式。
```java
package com.memcpy0.springframework.beans.factory.xml;  
  
import cn.hutool.core.util.StrUtil;  
import cn.hutool.core.util.XmlUtil;  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanReference;  
import com.memcpy0.springframework.beans.factory.support.AbstractBeanDefinitionReader;  
import com.memcpy0.springframework.beans.factory.support.BeanDefinitionReader;  
import com.memcpy0.springframework.beans.factory.support.BeanDefinitionRegistry;  
import com.memcpy0.springframework.core.io.Resource;  
import com.memcpy0.springframework.core.io.ResourceLoader;  
import org.w3c.dom.Document;  
import org.w3c.dom.Element;  
import org.w3c.dom.NodeList;  
  
import java.io.IOException;  
import java.io.InputStream;  
  
/**  
 * Bean definition reader and registry for XML bean definitions. 
 */
 public class XmlBeanDefinitionReader extends AbstractBeanDefinitionReader {  
    public XmlBeanDefinitionReader(BeanDefinitionRegistry registry) {  
        super(registry);  
    }  
  
    public XmlBeanDefinitionReader(BeanDefinitionRegistry registry, ResourceLoader resourceLoader) {  
        super(registry, resourceLoader);  
    }  
  
    @Override  
    public void loadBeanDefinitions(Resource resource) throws BeansException {  
        try {  
            try (InputStream inputStream = resource.getInputStream()) {  
                doLoadBeanDefinition(inputStream);  
            }  
        } catch (IOException | ClassNotFoundException e) {  
            throw new BeansException("IOException parsing XML document from " + resource, e);  
        }  
    }  
  
    @Override  
    public void loadBeanDefinitions(Resource... resources) throws BeansException {  
        for (Resource resource : resources) {  
            loadBeanDefinitions(resource);  
        }  
    }  
  
    @Override  
    public void loadBeanDefinitions(String location) throws BeansException {  
        ResourceLoader resourceLoader = getResourceLoader();  
        Resource resource = resourceLoader.getResource(location);  
        loadBeanDefinitions(resource);  
    }  
  
    protected void doLoadBeanDefinition(InputStream inputStream) throws ClassNotFoundException {  
        Document doc = XmlUtil.readXML(inputStream);  
        Element root = doc.getDocumentElement();  
        NodeList nodeList = root.getChildNodes();  
        for (int i = 0; i < nodeList.getLength(); ++i) {  
            // 判断元素  
            if (!(nodeList.item(i) instanceof Element)) continue;  
            // 判断对象  
            if (!"bean".equals(nodeList.item(i).getNodeName())) continue;  
            // 解析标签  
            Element bean = (Element) nodeList.item(i);  
            String id = bean.getAttribute("id");  
            String name = bean.getAttribute("name");  
            String className = bean.getAttribute("class");  
            // 获取Class，方便获取类中的名称  
            Class<?> clazz = Class.forName(className);  
            // 优先级id > name  
            String beanName = StrUtil.isNotEmpty(id) ? id : name;  
            if (StrUtil.isEmpty(beanName))  
                beanName = StrUtil.lowerFirst(clazz.getSimpleName());  
  
            // 定义Bean  
            BeanDefinition beanDefinition = new BeanDefinition(clazz);  
            // 读取属性并填充  
            for (int j = 0; j < bean.getChildNodes().getLength(); ++j) {  
                if (!(bean.getChildNodes().item(j) instanceof Element)) continue;  
                if (!"property".equals(bean.getChildNodes().item(j).getNodeName())) continue; // 不是property的节点名  
                // 解析标签  
                Element property = (Element) bean.getChildNodes().item(j);  
                String attrName = property.getAttribute("name");  
                String attrValue = property.getAttribute("value");  
                String attrRef = property.getAttribute("ref");  
                // 获取属性值：引入对象、值对象  
                Object value = StrUtil.isNotEmpty(attrRef) ? new BeanReference(attrRef) : attrValue;  
                // 创建属性信息  
                PropertyValue propertyValue = new PropertyValue(attrName, value);  
                beanDefinition.getPropertyValues().addPropertyValue(propertyValue);  
            }  
            if (getRegistry().containsBeanDefinition(beanName)) {  
                throw new BeansException("Duplicate beanName[" + beanName + "] is not allowed");  
            }  
            // 注册BeanDefinition  
            getRegistry().registerBeanDefinition(beanName, beanDefinition);  
        }  
    }  
}
```
- loadBeanDefinitions 方法，处理资源加载，这里新增加了一个内部方法：**doLoadBeanDefinitions，它主要负责解析 xml**、创建BeanDefinition、读取属性并填充、注册Bean定义
- 在 doLoadBeanDefinitions 方法中，主要是对 xml 的读取XmlUtil.readXML(inputStream) 和元素 Element 解析。在解析的过程中通过循环操作，以此**获取 Bean 配置以及配置中的 id、name、class、value、ref信息**。
- 最终把读取出来的配置信息，创建成 BeanDefinition 以及 PropertyValue，最终**把完整的 Bean 定义内容注册到 Bean 容器**： getRegistry().registerBeanDefinition(beanName, beanDefinition)


### 配置文件测试
这里有两份配置文件，一份用于测试资源加载器，另外 spring.xml 用于测试整体的 Bean 注册功能。

important.properties
```java
# Config File
system.key=OLpj9823dZ
```
spring.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<beans>  
   <bean id="userDao" class="com.memcpy0.bean.UserDao"/>    
   <bean id="userService" class="com.memcpy0.bean.UserService">  
        <property name="uId" value="10001"/>  
        <property name="userDao" ref="userDao"/>  
    </bean>  
</beans>
```
单元测试(资源加载)
```java
package com.memcpy0.bean;  
  
public class UserService {  
    private String uId;  
    private UserDao userDao;  
  
    public UserService() {}  
  
    public String getuId() { return uId; }  
  
    public void setuId(String uId) { this.uId = uId; }  
  
    public UserDao getUserDao() { return userDao; }  
  
    public void setUserDao(UserDao userDao) { this.userDao = userDao; }  
    public String queryUserInfo() {  
        return userDao.queryUserName(uId);  
    }  
}
```
单元测试（资源加载）：
```java
private static DefaultResourceLoader resourceLoader;  
  
@BeforeAll  
public static void init() {  
   resourceLoader = new DefaultResourceLoader();  
}  
  
@Test  
public void test_classpath() throws IOException {  
   Resource resource = resourceLoader.getResource("classpath:important.properties");  
   InputStream inputStream = resource.getInputStream();  
   String content = IoUtil.readUtf8(inputStream);  
   System.out.println(content);  
}  
  
@Test  
public void test_file() throws IOException {  
   Resource resource = resourceLoader.getResource("src/main/resources/important.properties");  
   InputStream inputStream = resource.getInputStream();  
   String content = IoUtil.readUtf8(inputStream);  
   System.out.println(content);  
}
  
@Test  
public void test_url() throws IOException {  
   Resource resource = resourceLoader.getResource("https://github.com/fuzhengwei/small-spring/important.properties");  
   InputStream inputStream = resource.getInputStream();  
   String content = IoUtil.readUtf8(inputStream);  
   System.out.println(content);  
}
```
这三个方法：test_classpath、test_file、test_url，分别用于测试加载 ClassPath、FileSystem、Url 文件，URL 文件在 Github，可能加载时会慢

单元测试(配置文件注册 Bean)
```java
@Test  
public void test_xml() {  
   // 1.初始化 
   BeanFactory DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();  
  
   // 2. 读取配置文件&注册Bean  
   XmlBeanDefinitionReader reader = new XmlBeanDefinitionReader(beanFactory);  
   reader.loadBeanDefinitions("classpath:spring.xml");  
  
   // 3. 获取Bean对象调用方法  
   UserService userService = beanFactory.getBean("userService", UserService.class);  
   String result = userService.queryUserInfo();  
   System.out.println("测试结果：" + result);  
}
```
在上面的测试案例中可以看到，我们把以前通过手动注册 Bean 以及配置属性信息的内容，交给了 new XmlBeanDefinitionReader  类读取 Spring.xml 的方式来处理，并通过了测试验证。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304251651150.png)


### 总结
此时的工程结构已经越来越有 Spring 框架的味道了，**以配置文件为入口解析和注册 Bean 信息，最终再通过 Bean 工厂获取 Bean 以及做相应的调用操作**。

关于案例中每一个步骤的实现这里都会尽可能参照 Spring 源码的接口定义、抽象类实现、名称规范、代码结构等，做相应的简化处理。这样大家在学习的过程中也可以通过类名或者接口和整个结构体学习 Spring 源码，这样学习起来就容易多了。

看完绝对不等于会，你只有动起手来从一个小小的工程框架结构，敲到现在以及以后不断的变大、变多、变强时，才能真的掌握这里面的知识。另外每一个章节的功能实现都会涉及到很多的代码设计思路，要认真去领悟。当然实践起来是最好的领悟方式！