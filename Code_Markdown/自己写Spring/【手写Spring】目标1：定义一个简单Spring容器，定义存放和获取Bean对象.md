本仓库以 Spring 源码学习为目的，通过手写简化版 Spring 框架，了解 Spring 核心原理。 在手写的过程中会简化 Spring 源码，摘取整体框架中的核心逻辑，简化代码实现过程，保留核心功能，例如：**IOC、AOP、Bean 生命周期**、上下文、**作用域**、资源处理等内容实现，逐步完成整个生命周期。

需要结合每节要**解决的目标**、进行的**思路设计**，带入到编码实操过程。在学习编码的同时，也最好理解关于这部分内容**为什么这样实现**，它用到了哪样的**设计模式**，**采用了什么手段做了什么样的职责分离**。
> 只有通过这样的学习 才能更好的理解和掌握 Spring 源码的实现过程，也能帮助你在以后的 深入学习和实践应用的过程中打下一个扎实的基础。

此专栏内容的学习上结合了设计模式，下对应了SpringBoot中间件设计和开发，所以学习过程中如果遇到不理解的设计模式，可以翻阅相应的资料，在学习完Spring后还可以结合中间件的内容进行练习。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304212206961.png)
因为所有我想写的内容，都希望它是**以理科思维理解为目的的学会**，而不是靠着硬背记住

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222036484.png)


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222038733.png)

从**最简单的 Bean 容器开始**
# 1. 创建简单的Spring Bean容器
> 容器：凡是可以存放数据的**具体数据结构实现**，都可以称之为容器
> ArrayList、LinkedList等都是容器

我们的容器主要用来存放Bean。
Spring容器**包含**并**管理**对象的**配置**和**生命周期**。
- 配置你的每个 Bean 对象是如何被创建的，
- 这些 Bean 可以创建一个单独的实例或者每次需要时都生成一个新的实例
- 以及它们是如 何相互关联构建和使用的。

如果一个Bean对象交给Spring容器管理，那么**这个Bean对象就应以类似零件的方式被拆解后存放到Bean的定义中**，相当于一种**把对象解耦**的操作，可以由 Spring 更加容易的管理，就像处理循环依赖等操作。

当一个Bean对象被定义存放以后，再由Spring统一进行装配，这个过程包括Bean的初始化和属性填充等，最终就能完整使用一个Bean实例化后的对象了。
## 案例目标就是定义一个简单的 Spring 容器，用于**定义**、存放 和获取 Bean 对象
在Spring容器的场景中，要一种可以「用于存放」和「名称索引」式的数据结构，选择 HashMap 是最合 适不过的。

HashMap是一种基于扰动函数、负载因子、红黑树转换等技术内容，形成的**拉链寻址**的数据结构，它能让数据更加散列的分布 在哈希桶以及**碰撞时形成的链表和红黑树**上。经测试，数据会均匀的散列在各个哈希桶索引上，所以 HashMap 非常适 合用在 Spring Bean 的容器实现上。

一个简单的Spring Bean容器实现，还需要Bean的定义、注册和获取三个基本步骤：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222048596.png)
- 定义：BeanDefinition，Spring 源码时经常看到的一个类。例如它会包括 singleton、prototype、BeanClassName 等。但目前
  初步实现会更加简单的处理，**Bean定义中只定义一个Object类型用于存放对象。**
- 注册：这个过程相当于我们把数据存放到HashMap中，不过现在HashMap存放的是**定义了的Bean的对象信息**。
- 获取：最后就是获取对象，Bean的名字就是key，**Spring 容器初始化好Bean以 后，就可以直接获取了**。

按照这个设计，做一个简单的 Spring Bean 容器代码实现。
> 编码 的过程往往并不会有多复杂，但知晓设计过程却更加重要！


---
## 实现
### 1. 工程结构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222059354.png)
Spring Bean容器类关系：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222100125.png)
Spring Bean 容器的整个实现内容非常简单，也仅仅是包括了一个简单的 BeanFactory 和 BeanDefinition，这里的类名称是与 Spring 源码中一致，只 不过现在的类实现会相对来说更简化一些，在后续的实现过程中再不断的添加 内容。 
1. BeanDefinition：用于定义Bean的实例化信息，现在的实现是以一个 Object 存放对象 
2. BeanFactory，代表了 Bean 对象的工厂，可以**存放 Bean 定义到 Map 中**以及**获取Bean对象**。

### 2. Bean定义
目前的Bean定义中，只有一个Object用于存放Bean对象。感兴趣可以参 考 Spring 源码中这个类的信息，名称都是一样的。
```java
package com.memcpy0.springframework;  
  
public class BeanDefinition {  
    private Object bean;  
    public BeanDefinition(Object bean) {  
        this.bean = bean;  
    }   
  
    public Object getBean() {  
        return bean;  
    }  
}
```
在后面陆续的实现中会逐步完善 BeanDefinition 相关属性的填充：
SCOPE_SINGLETON、SCOPE_PROTOTYPE、ROLE_APPLICATION、 ROLE_SUPPORT、ROLE_INFRASTRUCTURE 以及 Bean Class 信息。

### 3. Bean工厂
目前Bean工厂的实现中，包括了Bean定义的注册，这里注册的是**Bean的定义信息**，同时这个类中还包括了获取Bean的操作。
```java
package com.memcpy0.springframework;  
  
import java.util.Map;  
import java.util.concurrent.ConcurrentHashMap;  
import java.util.concurrent.ConcurrentMap;  
  
public class BeanFactory {  
    private Map<String, BeanDefinition> beanDefinitionMap = new ConcurrentHashMap<String, BeanDefinition>();  
  
    /**  
     * 从beanDefinitionMap获取BeanDefinition,再从中Bean对象  
     * @param name  
     * @return  
     */  
    public Object getBean(String name) {  
        return beanDefinitionMap.get(name).getBean();  
    }  
  
    /**  
     * 注册Bean定义  
     * @param name  
     * @param beanDefinition  
     */  
    public void registerBeanDefinition(String name, BeanDefinition beanDefinition) {  
        beanDefinitionMap.put(name, beanDefinition);  
    }  
}
```
目前的 BeanFactory 仍然是**非常简化**的实现，但这种简化的实现内容也是**整个 Spring 容器中关于 Bean 使用的最终体现结果，只不过实现过程只展示出基本的 核心原理**。在后续的补充实现中，这个会不断变得庞大。

### 4. 测试
准备一个UserService对象，方便后续对Spring容器测试。
```java
package com.memcpy0.springframework.test.bean;  
  
public class UserService {  
    public void queryUserInfo() {  
        System.out.println("查询用户信息");  
    }  
}
```
测试用例：
```java
package com.memcpy0.springframework.test;  
  
import com.memcpy0.springframework.BeanDefinition;  
import com.memcpy0.springframework.BeanFactory;  
import com.memcpy0.springframework.test.bean.UserService;  
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
      BeanFactory beanFactory = new BeanFactory();  
      // 2. 注册Bean定义  
      BeanDefinition beanDefinition = new BeanDefinition(new UserService());  
      beanFactory.registerBeanDefinition("userService", beanDefinition);  
      // 3. 获取Bean  
      UserService userService = (UserService) beanFactory.getBean("userService");  
      userService.queryUserInfo();  
   }  
}
```
在单测中主要包括：
- 初始化Bean工厂、注册Bean（定义）、获取Bean三个步骤。使用效果上贴近于Spring，但显得会更简化。
- 在 Bean 的注册中，这里是**直接把 UserService 实例化后作为入参传递给 BeanDefinition 的**，在后续的陆续实现中，我们会**把这部分内容放入 Bean 工厂中**实现。

通过测试结果可以看到，目前的 Spring Bean 容器，已经稍有雏形。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222120020.png)

### 5. 总结
**整篇关于 Spring Bean 容器的一个雏形就已经实现完成了**，相对来说这部分代码 并不会难住任何人，只要你稍加尝试就可以接受这部分内容的实现。 

但对于一个知识的学习来说，写代码只是最后的步骤，往往**整个思路、设计方案**，才更重要，只要你知道了因为什么、所以什么，才能让你有一个真正的理解。 

下一章会在此工程基础上扩容实现，要比现在的类多一些。不过每一篇的实现 上，我都会**以一个需求视角进行目标分析和方案设计**，让大家在学习编码之外更能 注重更多技术价值的学习。

---
## 目标2：实现Bean的定义、注册和获取
### 1. 逻辑设计System Design
你是否能预见复杂内容的设计问题？

再把系统设计的视角聚焦到具体代码实现上，你会有什么手段来实现想要的设计模式呢？其实编码方式主要依托于：接口/抽象类定义、**类实现接口、抽象类实现接口**、继承类、**继承抽象类**，而这些操作方式可以很好的隔离开每个类的 ==基础功能、通用功能和业务功能==，当类的职责清晰后，你的整个设计也会变得 容易扩展和迭代。

在本章节继续完善 Spring Bean 容器框架的功能开发，在这个开发过程 中会用到较多的**接口、类、抽象类**，它们之间会有**类的实现、类的继承**。

可以仔细参考这部分内容的开发实现，**虽然并不会很复杂**，但这种设计思路是完全 **可以复用到我们自己的业务系统开发中的**。

### 2. 目标
上一章节初步依照 Spring Bean 容器的概念，实现了一个粗糙版本的代码实现。

本章节我们需要**结合已实现的 Spring Bean 容器**进行功能完善， **实现 Bean 容器关于 Bean 对象的注册和获取**。
- 这一次在Bean注册时，**要把Bean的创建交给容器**、而不是我们在调用时传递一个实例化好的Bean对象
- 另外还需要考虑单例对象，**在对象的二次获取时是可以从内存中获取对象的**。
- 此外不仅要实现功能，还需要完善基础容器框架的类结构体， 否则将来就很难扩容进去其他的功能了。

### 3. 设计
首先非常重 要的一点是**在Bean注册的时候只注册一个类信息，而不会直接把实例化信息 注册到 Spring 容器中**。那么就需要**修改 BeanDefinition 中的属性 Object 为 Class**。

接下来需要做的就是在获取 Bean 对象时，需要处理 Bean 对象的 实例化操作、
以及判断当前单例对象在容器中是否已经缓存起来了。

整体设计如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222131078.png)

1. 首先我们**需要定义 BeanFactory 这样一个 Bean 工厂，提供 Bean 的获取方法 getBean(String name)**，之后**这个 Bean 工厂接口由抽象类 AbstractBeanFactory 实现**。这样使用**模板模式**的设计方式，可以==统一收口通用核心方法的调用逻辑和标准定义==，也就很好的控制了后续的实现者不用关心调用逻辑，按照统一方式执行。那么**类的继承者只需要关心具体方法的逻辑实现**即可。 
2. 那么在继承抽象类 AbstractBeanFactory 后的 AbstractAutowireCapableBeanFactory 就可以**实现相应的抽象方法了**，因为 AbstractAutowireCapableBeanFactory 本身也是一个抽象类，所以**它只会实现属于 自己的抽象方法**，其他抽象方法由继承 AbstractAutowireCapableBeanFactory 的 类实现。
   这里就体现了类实现过程中的各司其职，你只需要关心属于你的内容，不 是你的内容，不要参与。这一部分内容我们会在代码里有具体的体现
3. 另外这里还有块非常重要的知识点，就是关于**单例 SingletonBeanRegistry 的接口** 定义实现，而 **DefaultSingletonBeanRegistry 对接口实现后，会被抽象类 AbstractBeanFactory 继承**。现在 AbstractBeanFactory 就是一个非常完整且强大的 抽象类了，也能非常好的体现出它对模板模式的抽象定义。

下来我们就带着这些 设计层面的思考，去看代码的具体实现结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222148642.png)
这一章节关于 Spring Bean 容器的功能实现与 Spring 源码中还有不少的差距，但以目前实现结果的类关系图来看，其实已经具备了**一定的设计复杂性**，这些复杂的类关系设计在**各个接口定义和实现**以及在**抽象类继承**中都有所体现
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230047949.png)

- BeanFactory 的定义由 AbstractBeanFactory 抽象类**实现接口的 getBean 方法**
- 而 AbstractBeanFactory 又继承了「**实现了 SingletonBeanRegistry 的DefaultSingletonBeanRegistry 类**」。这样 AbstractBeanFactory 抽象类就**具备了单例Bean 的注册功能**。
- AbstractBeanFactory 中又定义了两个抽象方法：getBeanDefinition(String beanName)、createBean(String beanName, BeanDefinition beanDefinition) ，这两个抽象方法分别由 DefaultListableBeanFactory（可列Bean就要能返回BeanDefinition？）、AbstractAutowireCapableBeanFactory（自动注册使能，就要能创建Bean）实现。最终 DefaultListableBeanFactory 还会继承抽象类AbstractAutowireCapableBeanFactory 也就可以调用抽象类中的 createBean 方法了。

综上，这一部分的类关系和实现过程还是会有一些复杂的，因为所有的实现都以**职责划分、共性分离以及调用关系定义**为标准搭建的类关系。这部分内容的学习，可能会**丰富你在复杂业务系统开发中的设计思路**。

### 1. 实现的工程结构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304222154529.png)
```java
package com.memcpy0.springframework.beans;  
  
public class BeansException extends RuntimeException {  
    public BeansException(String msg) {  
        super(msg);  
    }  
    public BeansException(String msg, Throwable cause) {  
        super(msg, cause);  
    }  
}
```
### 2. BeanDefinition定义
在 Bean 定义类中已经把上一章节中的 Object bean 替换为 Class，这样就**可以把 Bean 的实例化操作放到容器中处理了**。如果有仔细阅读过上一章并做了相应的测试，那么你会发现 ==Bean 的实例化操作是放在初始化调用阶段传递给 BeanDefinition 构造函数的==。
```java
package com.memcpy0.springframework.beans.factory.config;  
  
/**  
 * 存储Bean的类信息  
 */  
public class BeanDefinition {  
    private Class beanClass;  
    public BeanDefinition(Class beanClass) {  
        this.beanClass = beanClass;  
    }  
    public Class getBeanClass() {  
        return beanClass;  
    }  
    public void setBeanClass() {  
        this.beanClass = beanClass;  
    }  
}
```
### 3. SingletonBeanRegistry单例Bean注册接口定义和实现
```java
package com.memcpy0.springframework.beans.factory.config;  
/**  
 * 定义一个获取单例对象的接口  
 */
public interface SingletonBeanRegistry {  
    Object getSingleton(String beanName);  
}
```
这个类比较简单，主要是定义了**一个获取单例对象的接口**。
### 4. DefaultSingletonBeanRegistry
在DefaultSingletonBeanRegistry中主要实现getSingleton方法，同时实现受保护的addSingleton方法，这个方法可以被继承此类的其他类调用。包括：AbstractBeanFactory以及继承的AbstractAutowireCapableBeanFactory、DefaultListableBeanFactory调用。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.factory.config.SingletonBeanRegistry;  
  
import java.util.HashMap;  
import java.util.Map;  
  
public class DefaultSingletonBeanRegistry implements SingletonBeanRegistry {  
    private Map<String, Object> singletonObjects = new HashMap<>();  
    @Override  
    public Object getSingleton(String beanName) {  
        return singletonObjects.get(beanName);  
    } 
    /**  
	 * 添加注册单例对象  
	 * @param beanName  
	 * @param singletonObject  
	 */ 
    protected void addSingleton(String beanName, Object singletonObject) {  
        singletonObjects.put(beanName, singletonObject);  
    }  
}
```
### 5. 抽象类AbstractBeanFactory定义模板方法
```java
package com.memcpy0.springframework.beans.factory;  
  
public interface BeanFactory {  
    Object getBean(String name);  
}
```
AbstractBeanFactory首先继承了DefaultSingletonBeanRegistry，也就具备了注册单例Bean的类方法。

接下来**很重要的一点是关于接口 BeanFactory 的实现**——在方法 getBean 的实现过
程中可以看到，主要是**对单例 Bean 对象的获取**以及==在获取不到时需要拿到 Bean
定义做相应 Bean 实例化操作==。那么 getBean 并没有自身去实现这些方法，
而是**只定义了调用过程以及提供了抽象方法**，由继承此抽象类的其他类做相应实
现。

后续继承抽象类 AbstractBeanFactory 的类有两个，包括：
AbstractAutowireCapableBeanFactory、DefaultListableBeanFactory，**这两个类分别
做了相应的实现处理**，接着往下看。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.factory.BeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.BeansException;  
  
public abstract class AbstractBeanFactory extends DefaultSingletonBeanRegistry implements BeanFactory {  
    @Override  
    public Object getBean(String name) throws BeansException {  
        Object bean = getSingleton(name);  
        if (bean != null) {  
            return bean;  
        }  
        BeanDefinition beanDefinition = getBeanDefinition(name);  
        return createBean(name, beanDefinition);  
    }  
  
    protected abstract BeanDefinition getBeanDefinition(String beanName) throws BeansException;  
    protected abstract Object createBean(String beanName, BeanDefinition beanDefinition) throws BeansException;  
}
```
### 6. 实例化Bean类AbstractAutowireCapableBeanFactory
先使用未实现的getBeanDefinition方法获取BeanDefinition，再从其中获取Class对象、并创建Bean实例，添加到DefaultSingleBeanRegistry的单例池中。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory {  
  
    @Override  
    protected Object createBean(String beanName, BeanDefinition beanDefinition) throws BeansException {  
        Object bean = null;  
        try {  
            bean = beanDefinition.getBeanClass().newInstance();  
        } catch (InstantiationException | IllegalAccessException e) {  
            throw new BeansException("Instantiation of bean failed", e);  
        }  
        addSingleton(beanName, bean);  
        return bean;  
    }  
}
```
> 在 AbstractAutowireCapableBeanFactory 类中实现了 Bean 的实例化操作
newInstance，其实这块会埋下一个坑，**有构造函数入参的对象怎么处理**？可以提前思考

在处理完 Bean 对象的实例化后，直接调用 addSingleton 方法存放到单例对象缓存中去

### 7. 核心类实现DefaultListableBeanFactory（依赖BeanDefinition、实现了BeanDefinitionRegistry接口）注册了BeanDefinition才能获得BeanDefinition
DefaultListableBeanFactory在Spring源码中是一个非常核心的类，目前的实现中也是贴近于源码，与源码类名保持一致。
DefaultListableBeanFactory继承了AbstractAutowireCapableBeanFactory类，也就具备了接口BeanFactory和AbstractBeanFactory等一堆方法的功能实现，所以==有时候会看到一些类的强转，调用某些方法，也是因为你强转的类实现接口或继承了某些类==。

除此之外这个类还实现了接口BeanDefinitionRegistry中的registerBeanDefinition(String beaName, BeanDefinition beanDefinition)方法，当然就还会看到一个getBeanDefinition的实现，这个方法我们文中提到过它是抽象类AbstractBeanFactory 中定义的抽象方法，==现在注册Bean定义与获取Bean定义就可以同时使用了==。是不是感觉这个套路还蛮深的。**接口定义了注册，抽象类定义了
获取，都集中在 DefaultListableBeanFactory 中的 beanDefinitionMap 里**
```java
package com.memcpy0.springframework.beans.factory.support;  
  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import org.springframework.context.annotation.Bean;  
  
import java.util.HashMap;  
import java.util.Map;  
  
public class DefaultListableBeanFactory extends AbstractAutowireCapableBeanFactory implements BeanDefinitionRegistry {  
    private Map<String, BeanDefinition> beanDefinitionMap = new HashMap<>();  
  
    /**  
     * 获取BeanDefinition  
     * @param beanName  
     * @return  
     * @throws BeansException  
     */    @Override  
    protected BeanDefinition getBeanDefinition(String beanName) throws BeansException {  
        BeanDefinition beanDefinition = beanDefinitionMap.get(beanName);  
        if (beanDefinition == null)  
            throw new BeansException("No bean named '" + beanName + "' is defined");  
        return beanDefinition;  
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
}
```
### 测试
事先准备：这里简单定义了一个 UserService 对象，方便我们后续对 Spring 容器测试。
```java
package com.memcpy0.bean;  
  
public class UserService {  
    public void queryUserInfo() {  
        System.out.println("查询用户信息");  
    }  
}
```
测试用例：
```java
package com.memcpy0;  
  
import com.memcpy0.bean.UserService;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
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
      BeanDefinition beanDefinition = new BeanDefinition(UserService.class); // 不用自己实例化Bean去传入了  
      beanFactory.registerBeanDefinition("userService", beanDefinition); // 注册Bean定义  
      // 3. 第一次获取Bean  
      UserService userService = (UserService) beanFactory.getBean("userService");  
      userService.queryUserInfo();  
      // 4. 第二次获取bean from Singleton  
      UserService userService_singleton = (UserService) beanFactory.getBean("userService");  
      userService_singleton.queryUserInfo();  
      if (userService == userService_singleton) System.out.println("same instance");  
   }  
}
```
这次的单测包括：
- Bean工厂、注册Bean、获取Bean三个步骤，还额外增加了一次对象的获取和调用。这里主要测试**验证单例对象是否正确存放到了缓存中**。
- 此外与上一章节测试过程中不同的是，我们把 UserService.class 传递给了 BeanDefinition 而不是像上一章节那样直接 new UserService() 操作。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230044013.png)

这里会有两次测试信息，一次是获取 Bean 时直接创建的对象，另外一次是从缓存中获取的实例化对象。

此外从调试的截图中也可以看到第二次获取单例对象，已经可以从内存中获取了，如图 3-3
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230045203.png)

关于测试过程中可以**再去断点调试下各个阶段类的调用**，熟悉调用关系。

### 总结
相对于前一章节对 Spring Bean 容器的简单概念实现，本章节中加强了功能的完善。在实现的过程中也可以看到类的关系变得越来越多了，如果没有做过一些稍微复杂的系统类系统，**那么即使现在这样 9 个类搭出来的容器工厂也可以给你绕晕**。
 
在 Spring Bean 容器的实现类中，**要重点关注类之间的职责和关系**，==几乎所有的程序功能设计都离不开**接口、抽象类、实现、继承**，而这些不同特性类的使用就可以**非常好的隔离开类的功能职责和作用范围**==。而这样的知识点也是在学习手写Spring Bean 容器框架过程非常重要的知识。

最后要强调一下关于整个系列内容的学习，可能在学习的过程中会遇到像第二章节那样非常简单的代码实现，但要做一个有成长的程序员要记住==代码实现只是最后的落地结果，而那些设计上的思考才是最有价值的地方==。

就像你是否遇到过，有人让你给一个内容做个描述、文档、说明，你总觉得太简单了没什么可写的，即使要动笔写了也不知道要从哪开始！其实这些知识内容都==来源你对整体功能的理解==，这就不只是代码开发，还包括了==需求目标、方案设计、技术实现、逻辑验证等等过程性的内容==。所以，不要只是被看似简单的内容忽略了整体全局观，要学会放开视野，开放学习视角。

---
## 对象实例化策略
CRUD 写的再多也只是能满足你作为一个搬砖工具人，

那你可能会想什么才是编程能力提升？其实更多的编程能力的提升是你对复杂场景的架构把控以及对每一个技术实现细节点的不断用具有规模体量的流量冲击验证时，是否能保证系统稳定运行从而决定你见识了多少、学到了多少、提升了多少！

最终当你在接一个产品需求时，开始思考==程序数据结构的设计、核心功能的算法逻辑实现、整体服务的设计模式使用、系统架构的搭建方式、应用集群的部署结构==，那么也就是的编程能力真正提升的时候！
### 1. 目标
这一章节的目标主要是为了解决上一章节我们埋下的坑，那是什么坑呢？其实就是一个**关于 Bean 对象在含有构造函数进行实例化**的坑。

在上一章节我们**扩充了 Bean 容器的功能**，把实例化对象交给容器来统一处理，但在我们实例化对象的代码里**并没有考虑对象类是否含构造函数**，也就是说如果我们去实例化一个含有构造函数的对象，那么就要抛异常了。

怎么验证？其实就是把 UserService 添加一个含入参信息的构造函数就可以，如下：
```java
package com.memcpy0.bean;  
  
public class UserService {  
    public void queryUserInfo() {  
        System.out.println("查询用户信息");  
    }  
    public UserService(String name) {
	    this.name = name;
    }
}
```
报错：
```java
java.lang.InstantiationException: cn.bugstack.springframework.test.bean.UserService
at java.lang.Class.newInstance(Class.java:427)
at cn.bugstack.springframework.test.ApiTest.test_newInstance(ApiTest.java:51)
...
```
发生这一现象的主要原因就是因为
beanDefinition.getBeanClass().newInstance(); 实例化方式并没有考虑构造函数的入参，所以就这个坑就在这等着你了！那么我们的目标就很明显了，==来把这个坑填平==！

### 2. 设计
填平这个坑的技术设计主要考虑两部分，一个是看流程从哪合理的**把构造函数的入参信息传递到实例化操作里**，另外一个是**怎么去实例化含有构造函数的对象**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230058356.png)
- 参考 Spring Bean 容器源码的实现方式，在 **BeanFactory** 中添加 `Object getBean(String name, Object... args)` 接口，这样就可以**在获取Bean 时把构造函数的入参信息传递进去了**。
- 另外一个核心的内容是使用什么方式来创建含有构造函数的 Bean 对象呢？这里有两种方式可以选择，
	- 一个是基于 Java 本身自带的方法**DeclaredConstructor**，
	- 另外一个是使用 Cglib 来动态创建 Bean 对象。==Cglib 是基于字节码框架 ASM 实现，所以也可以直接通过 ASM 操作指令码来创建对象==
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230113224.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304230115567.png)

本章节“填坑”主要是在现有工程中添加**InstantiationStrategy 实例化策略接口**，以及**补充相应的 getBean 入参信息**，让外部调用时可以传递构造函数的入参并顺利实例化。

### 3. 新增getBean接口方法
BeanFactory中重载了一个含有入参信息args的getBean方法，这样就可以方便传递入参、给构造函数来实例化了。
```java
package com.memcpy0.springframework.beans.factory;  
  
import com.memcpy0.springframework.beans.BeansException;  
  
public interface BeanFactory {  
    Object getBean(String beanName) throws BeansException;  
    Object getBean(String beanName, Object... args) throws BeansException;  
}
```
### 4. 定义实例化策略接口、让AbstractAutowireCapableBeanFactory负责创建Bean的、来实现这个接口中的实例化方法
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
import java.lang.reflect.Constructor;  
  
public interface InstantiationStrategy {  
    Object instantiate(BeanDefinition beanDefinition, String beanName, Constructor ctor, Object[] args) throws BeansException;  
}
```
在实例化接口instantiate方法中使用必要的入参信息（包括beanDefinition, beaName, ctor, args）来实例化对象
其中Constructor的java.lang.reflect包下的Constructor类，里面包含了一些必要的类信息，有这个参数的目的就是**为了拿到符合入参信息相对应的构造函数**。
而args就是一个具体的入参信息，最终实例化时会用到。
### 5. JDK实例化
首先通过BeanDefinition 获取Class信息，这个Class信息是在定义BeanDefinition时传入的
- 接下来判断ctor是否为空，如果为空则是无构造函数实例化，否则就是需要有构造函数的实例化
- 这里我们重点关注有构造函数的实例化，实例化方式为ctor.getClass().getDeclaredConstructor(ctor.getParameterTypes()).newInstance(args);
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
import java.lang.reflect.Constructor;  
import java.lang.reflect.InvocationTargetException;  
  
public class SimpleInstantiationStrategy implements InstantiationStrategy {  
    @Override  
    public Object instantiate(BeanDefinition beanDefinition, String beanName, Constructor ctor, Object[] args) throws BeansException {  
        Class clazz = beanDefinition.getBeanClass();  
        try {  
            if (null != ctor) {  
                return clazz.getDeclaredConstructor(ctor.getParameterTypes()).newInstance(args);  
            } else {  
                return clazz.getDeclaredConstructor().newInstance();  
            }  
        } catch (InstantiationException | IllegalAccessException | InvocationTargetException | NoSuchMethodException e) {  
            throw new BeansException("Failed to instantiate [" + clazz.getName() + "]", e);  
        }  
    }  
}
```
### 6. Cglib子类实例化
Cglib创建有构造函数的Bean也非常方便。这里更简化的处理了，如果你阅读 Spring 源码还会看到 CallbackFilter 等实现，不过我们目前的方式并不会影响创建。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import org.springframework.cglib.proxy.Enhancer;  
import org.springframework.cglib.proxy.NoOp;  
  
import java.lang.reflect.Constructor;  
  
public class CglibSubclassingInstantiationStrategy implements InstantiationStrategy {  
  
    @Override  
    public Object instantiate(BeanDefinition beanDefinition, String beanName, Constructor ctor, Object[] args) throws BeansException {  
        Enhancer enhancer = new Enhancer();  
        enhancer.setSuperclass(beanDefinition.getBeanClass()); // 实例化Bean子类  
        enhancer.setCallback(new NoOp() {  
            @Override  
            public int hashCode() {  
                return super.hashCode(); // 以Bean的哈希码为码  
            }  
        });  
        if (null == ctor) return enhancer.create();  
        return enhancer.create(ctor.getParameterTypes(), args);  
    }  
}
```
### 7. AbstractAutowireCapableBeanFactory创建策略调用来createBean
AbstractBeanFactory中将实现自BeanFactory的getBean两个方法==都委托给doGetBean来统一调用==：**createBean抽象方法的形式变化**：
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
==在createBean中，将不同形式的实例化创建委托给createBeanInstance方法==。
- 首先在AbstractAutowireCapableBeanFactory抽象类中定义了一个创建对象的实例化策略类 InstantiationStrategy instantiationStrategy，这里选中了CGLIB的实现类
- 接下来抽取 createBeanInstance 方法，在这个方法中需要注意 Constructor 代表了你有多少个构造函数，==通过 beanClass.getDeclaredConstructors() 方式可以获取到你所有的构造函数，是一个集合。==
- 接下来就需要循环比对出构造函数集合与入参信息 args 的匹配情况，这里对比的方式比较简单，只是一个数量对比，而**实际 Spring 源码中还需要比对入参类型**，否则相同数量不同入参类型的情况，就会抛异常了。
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
  
import java.lang.reflect.Constructor;  
  
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory {  
    private InstantiationStrategy instantiationStrategy = new CglibSubclassingInstantiationStrategy();  
  
    @Override  
    protected Object createBean(String beanName, BeanDefinition beanDefinition, Object[] args) throws BeansException {  
        Object bean = null;  
        try {  
//            bean = beanDefinition.getBeanClass().newInstance();  
            bean = createBeanInstance(beanDefinition, beanName, args);  
        } catch (Exception e) {  
            throw new BeansException("Instantiation of bean failed", e);  
        }  
        addSingleton(beanName, bean); // 第一次创建Bean时要加入单例池  
        return bean;  
    }  
  
    protected Object createBeanInstance(BeanDefinition beanDefinition, String beanName, Object[] args) {  
        Constructor constructorToUse = null;  
        Class<?> beanClass = beanDefinition.getBeanClass();  
        Constructor<?>[] declaredConstructors = beanClass.getDeclaredConstructors(); // 从Class对象获取所有构造函数  
        for (Constructor ctor : declaredConstructors) {  // 找到合适的构造函数，传给实例化策略
            if (null != args && ctor.getParameterTypes().length == args.length) { // 这里对比起来简化了  
                constructorToUse = ctor;  
                break;            
            }  
        }  
        return getInstantiationStrategy().instantiate(beanDefinition, beanName, constructorToUse, args);  
    }  
      
    public InstantiationStrategy getInstantiationStrategy() {  
        return instantiationStrategy;  
    }  
      
    public void setInstantiationStrategy(InstantiationStrategy instantiationStrategy) {  
        this.instantiationStrategy = instantiationStrategy;  
    }  
}
```
### 测试
这里唯一多在 UserService 中添加的就是一个有 name 入参的构造函数，方便我们验证这样的对象是否能被实例化。
```java
package com.memcpy0.bean;  
  
public class UserService {  
    private String name;  
  
    public UserService() {  
    }  
  
    public UserService(String name) {  
        this.name = name;  
    }  
  
    public void queryUserInfo() {  
        System.out.println("查询用户信息：" + name);  
    }  
  
    @Override  
    public String toString() {  
        final StringBuilder sb = new StringBuilder("");  
        sb.append("").append(name);  
        return sb.toString();  
    }  
}
```
测试用例如下：在此次的单元测试中除了包括；**Bean 工厂、注册 Bean、获取 Bean**，三个步骤，
~~还额外增加了一次对象的获取和调用。这里主要测试验证单例对象的是否正确的存放到了缓存中。
~~
~~此外与上一章节测试过程中不同的是，我们把 UserService.class 传递给了 BeanDefinition 、而不是像上一章节那样直接 new UserService() 操作。~~
```java
package com.memcpy0;  
  
import com.memcpy0.bean.UserService;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
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
      BeanDefinition beanDefinition = new BeanDefinition(UserService.class); // 不用自己实例化Bean去传入了  
      beanFactory.registerBeanDefinition("userService", beanDefinition); // 注册Bean定义  
      // 3. 第一次获取Bean  
//    UserService userService = (UserService) beanFactory.getBean("userService"); // 无参构造  
//    userService.queryUserInfo();  
  
      // 4. 第二次获取bean from Singleton  
      UserService userService_singleton = (UserService) beanFactory.getBean("userService", "memcpy0");  
      userService_singleton.queryUserInfo();  
  
//    if (userService == userService_singleton) System.out.println("same instance");  
   }  
}
```
这里如果两次获取UserService，第一次无参构造后，单例池中就有了UserService，第二次只能获取无参构造的UserService？？？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304241008555.png)
从测试结果来看，最大的变化就是可以满足带有构造函数的对象，可以被实例化了。

可以尝试分别使用两种不同的实例化策略，来进行实例化。
- SimpleInstantiationStrategy、
- CglibSubclassingInstantiationStrategy

这里我们再把几种不同方式的实例化操作，放到单元测试中，方便大家比对学习。
- 这种方式的实例化也是我们在上章节实现 Spring Bean 容器时直接使用的方式
```java
@Test
public void test_newInstance() throws IllegalAccessException, InstantiationException {
UserService userService = UserService.class.newInstance();
System.out.println(userService);
}

@Test
public void test_constructor() throws Exception {
	Class<UserService> userServiceClass = UserService.class;
	Constructor<UserService> declaredConstructor = userServiceClass.getDeclaredCons
	tructor(String.class);
	UserService userService = declaredConstructor.newInstance("小傅哥");
	System.out.println(userService);
}
```
其实最核心的点在于获取一个类中所有的构造函数，其实也就是这个方法的使用 beanClass.getDeclaredConstructors()
```java
@Test
public void test_parameterTypes() throws Exception {
	Class<UserService> beanClass = UserService.class;
	Constructor<?>[] declaredConstructors = beanClass.getDeclaredConstructors();
	Constructor<?> constructor = declaredConstructors[0];
	Constructor<UserService> declaredConstructor = beanClass.getDeclaredConstructor
	(constructor.getParameterTypes());
	UserService userService = declaredConstructor.newInstance("小傅哥");
	System.out.println(userService);
}
```
演示使用非常简单，但关于 Cglib 在 Spring 容器中的使用非常多，也可以深入的学习一下 Cglib 的扩展知识。
```java
@Test
public void test_cglib() {
	Enhancer enhancer = new Enhancer();
	enhancer.setSuperclass(UserService.class);
	enhancer.setCallback(new NoOp() {
		@Override
		public int hashCode() {
			return super.hashCode();
		}
	});
	Object obj = enhancer.create(new Class[]{String.class}, new Object[]{"小傅哥"});
	System.out.println(obj);
}
```
### 总结
本章节主要以完善实例化操作，增加 InstantiationStrategy 实例化策略接口，并新增了两个实例化类。==这部分类的名称与实现方式基本是 Spring 框架的一个缩小版==，大家在学习过程中也可以从 Spring 源码找到对应的代码

从我们不断完善增加需求可以看到，当你的代码结构设计较为合理时，就可以非常容易且方便的进行扩展不同属性的类职责，而不会因为需求的增加导致类结构混乱。所以在我们自己业务需求实现的过程中，也要尽可能的去考虑**一个良好的扩展性以及拆分好类的职责**。

**动手是学习起来最快的方式**，不要让眼睛是感觉看会了，但上手操作就废了。也希望有需要的读者可以亲手操作一下，把你的想法也融入到可落地实现的代码里，看看想的和做的是否一致。