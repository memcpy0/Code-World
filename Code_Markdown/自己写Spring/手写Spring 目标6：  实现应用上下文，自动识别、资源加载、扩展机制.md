> 我们在承接紧急的产品需求时候，通常会选择在原有同类项目中进行扩展，如果没有相关类型项目的储备，也可能会选择临时搭建出一个工程来实现产品的需求。但这个时候就会遇到非常现实的问题，选择完整的设计和开发就可能满足不了上线时间，临时拼凑式的完成需求又可能不具备上线后响应产品的临时调整。
> 
> 上线后的调整有哪些呢？项目刚一上线，运营了还不到半天，老板发现自己的配置的活动好像金额配置的太小了，用户都不来，割不到韭菜呀。赶紧半夜联系产品，来来来，你给我这改改，那修修，把人均优惠 1 万元放大大的，把可能两字缩小放在后面。再把优惠的奖金池配置从 10 元调整 11 元，快快快，赶紧修改，你修改了咱们能赚 1 个亿！！！
> 好家伙，项目是临时开发堆出来的，没有后台系统、没有配置中心、没有模块拆分，老板一句句改改改，产品来传达催促，最后背锅的可就是研发了。你这不能写死，这优惠配置得抽出来，这文案也后台下发吧，这接口入参也写死了，再写一个新接口吧！ 一顿操作猛如虎，研发搬砖修接口，运营折腾好几宿，最后 PV150！
> 无论业务、产品、运营如何，但就研发自身来讲，尽可能的要不避免临时堆出一个服务来，尤其是在团队建设初期或者运营思路经常调整的情况下，更要注重设计细节和实现方案。哪怕去报风险延期，也不要让自己背上一个明知是烂坑还要接的活。
 
说到不把代码写死，就是因为我们需要继续在手写 Spring 框架中**继续扩展新的功能**，如**一个 Bean 的定义和实例化的过程前后，是否可以满足我们进行自定义扩展，对 Bean 对象执行一些修改、增强、记录等操作**呢？ 这个过程基本就是你在使用 Spring 容器框架时候**做的一些中间件扩展开发**

### 1. 目标
如果你在自己的实际工作中开发过基于 Spring 的技术组件，或者学习过关于SpringBoot 中间件设计和开发 等内容。那么你一定会继承或者实现了 Spring 对外暴露的类或接口，在接口的实现中获取了 BeanFactory 以及 Bean 对象的 获取等内容，并对这些内容做一些操作，例如：修改 Bean 的信息，添加日志打印、处理数据库路由对数据源的切换、给 RPC 服务连接注册中心等。

在**对容器中 Bean 的实例化过程添加扩展机制**的同时，还需要**把目前关于Spring.xml 初始化和加载策略进行优化**，因为我们**不太可能让面向 Spring 本身开发的 DefaultListableBeanFactory 服务，直接给予用户使用**。修改点如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304251655092.png)
DefaultListableBeanFactory、XmlBeanDefinitionReader，是我们==在目前 Spring 框架中对于服务功能测试的使用方式==，**它能很好的体现出 Spring 是如何对 xml 加载以及注册 Bean 对象的操作过程**，但这种方式是面向 Spring 本身的，还不具备一定的扩展性。

就像我们现在需要提供出一个==可以在 Bean 初始化过程中完成对 Bean 对象的扩展==时，就很难做到自动化处理。所以我们**要把 Bean 对象扩展机制功能和对 Spring 框架上下文的包装**融合起来，对外提供完整的服务。
### 2. 设计
为了能满足于**在 Bean 对象从注册到实例化的过程中**执行**用户的自定义操作**，就需要==在 Bean 的定义和初始化过程中**插入接口类**，这个接口再有外部去实现自己需要的服务==。那么*在结合对 Spring 框架上下文的处理能力，就可以满足我们的目标需求了*。整体设计结构如下图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304251659598.png)
- 满足于对 Bean 对象扩展的两个接口，其实也是 Spring 框架中非常具有重量级的两个接口：BeanFactoryPostProcess 和 BeanPostProcessor，也几乎是大家在使用 Spring 框架**额外新增开发自己组件需求的两个必备接口**。
- ==BeanFactoryPostProcessor，是由 Spring 框架组件提供的容器扩展机制，允许在Bean 对象注册后但未实例化之前，对 Bean 的定义信息 BeanDefinition 执行修改操作==。
- BeanPostProcessor，也是 Spring 提供的扩展机制，不过 ==BeanPostProcessor 是在Bean 对象实例化之后修改 Bean 对象，也可以替换 Bean 对象==。这部分与后面要实现的 AOP 有着密切的关系。
- 同时如果只是添加这两个接口，不做任何包装，那么对于使用者来说还是非常麻烦的。我们希望于**开发 Spring 的上下文操作类**，把**相应的 XML 加载 、注册、实例化以及新增的修改和扩展都融合进去**，让 Spring 可以**自动扫描到我们的新增服务，便于用户使用**

### 3. 工程结构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304251704941.png)
主要集中在Context下的包。

Spring 应用上下文和对 Bean 对象扩展机制的类关系
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304251714609.png)
在整个类图中主要体现出来的是**关于 Spring 应用上下文以及对 Bean 对象扩展机制的实现**。
- 以继承了 ListableBeanFactory 接口的 ApplicationContext 接口开始，扩展出一系列应用上下文的抽象实现类，并最终完成 ClassPathXmlApplicationContext 类的实现。而这个类就是最后交给用户使用的类。

同时==在实现应用上下文的过程中，通过定义接口：BeanFactoryPostProcessor、BeanPostProcessor 两个接口，把关于对 Bean 的扩展机制串联进去了==。

### 2. 定义 BeanFactoryPostProcessor
在 Spring 源码中有这样一段描述 Allows for custom modification of an application context's bean definitions,adapting the bean property values of the context's underlying bean factory.  允许自定义修改ApplicationContext的bean定义，从而调整上下文底层bean工厂的bean属性值。

其实也就是说这个接口是==满足于在所有的 BeanDefinition 加载完成后，实例化 Bean 对象之前，提供修改 BeanDefinition 属性的机制==。
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
  
/**  
 * Allows for custom modification of an application context's bean definitions, * adapting the bean property values of the context's underlying bean factory. * * 允许自定义修改 BeanDefinition 属性信息  
 */  
public interface BeanFactoryPostProcessor {  
    /**  
     * 在所有的 BeanDefinition 加载完成后，实例化 Bean 对象之前，提供修改 BeanDefinition 属性的机制  
     *  
     * @param beanFactory  
     * @throws BeansException  
     */    void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException;  
}
```
### 3. 定义BeanPostProcessor
Factory hook that allows for custom modification of new bean instances,e.g. checking for marker interfaces or wrapping them with proxies  工厂挂钩，允许对新的bean实例进行自定义修改，例如检查标记接口或用代理包装它们

也就是提供了修改新实例化 Bean 对象的扩展点。
- 另外此接口提供了两个方法：postProcessBeforeInitialization 用于**在 Bean 对象执行初始化方法之前**，执行此方法、
- postProcessAfterInitialization 用于**在 Bean 对象执行初始化方法之后**，执行此方法。

```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.BeansException;  
  
/**  
 * Factory hook that allows for custom modification of new bean instances, * e.g. checking for marker interfaces or wrapping them with proxies. * * 用于修改新实例化 Bean 对象的扩展点  
 */  
public interface BeanPostProcessor {  
  
    /**  
     * 在 Bean 对象执行初始化方法之前，执行此方法  
     *  
     * @param bean  
     * @param beanName 用于确定是哪个Bean，好分别处理
     * @return  
     * @throws BeansException  
     */    Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException;  
  
    /**  
     * 在 Bean 对象执行初始化方法之后，执行此方法  
     *  
     * @param bean  
     * @param beanName 用于确定是哪个Bean，好分别处理
     * @return  
     * @throws BeansException  
     */    Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException;  
}
```
### 4. ApplicationContext
context 是本次**实现应用上下文功能**新增的服务包。ApplicationContext，继承于 ListableBeanFactory，也就继承了关于 BeanFactory 方法，比如一些 getBean 的方法。另外 **ApplicationContext 本身是 Central 接口**，但目前还不需要添加一些获取 ID 和父类上下文，所以暂时没有接口方法的定义。
```java
package com.memcpy0.springframework.context;  
  
import com.memcpy0.springframework.beans.factory.ListableBeanFactory;  
  
/**  
 * Central interface to provide configuration for an application. * This is read-only while the application is running, but may be * reloaded if the implementation supports this. * * 应用上下文  
 */  
public interface ApplicationContext extends ListableBeanFactory {  
}
```
ConfigurableApplicationContext 继承自 ApplicationContext，并提供了 refresh 这个核心方法。==如果有看过一些 Spring 源码，那么一定会看到这个方法。 接下来也是需要在上下文的实现中完成刷新容器的操作过程==。
```java
package com.memcpy0.springframework.context;  
  
  
import com.memcpy0.springframework.beans.BeansException;  
/**  
 * SPI interface to be implemented by most if not all application contexts. * Provides facilities to configure an application context in addition * to the application context client methods in the * {@link com.memcpy0.springframework.context.ApplicationContext} interface.  
 */public interface ConfigurableApplicationContext extends ApplicationContext {  
    /**  
     * 刷新容器  
     * @throws BeansException  
     */    
    void refresh() throws BeansException;  
}
```
关于其他类：
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.factory.HierarchicalBeanFactory;  
  
/**  
 * Configuration interface to be implemented by most bean factories. Provides * facilities to configure a bean factory, in addition to the bean factory * client methods in the {@link com.memcpy0.springframework.beans.factory.BeanFactory}  
 * interface. * 将由大多数Bean工厂实现的配置接口，提供了配置Bean工厂的方法，还有Bean Factory客户端方法（即一系列getBean方法）  
 */  
public interface ConfigurableBeanFactory extends HierarchicalBeanFactory, SingletonBeanRegistry {  
    String SCOPE_SINGLETON = "singleton";  
  
    String SCOPE_PROTOTYPE = "prototype";  
  
    void addBeanPostProcessor(BeanPostProcessor beanPostProcessor);  
}
```
AbstractBeanFactory实现ConfigurableBeanFactory的addBeanPostProcessor方法：
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
import com.memcpy0.springframework.beans.factory.config.ConfigurableBeanFactory;  
  
import java.util.ArrayList;  
import java.util.List;  
  
public abstract class AbstractBeanFactory extends DefaultSingletonBeanRegistry implements ConfigurableBeanFactory { // ConfigurableListableBeanFactory给DefaultListableBeanFactory实现  
  
    /** BeanPostProcessors to apply in createBean */  
    private final List<BeanPostProcessor> beanPostProcessors = new ArrayList<>();  
  
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
  
    @Override  
    public void addBeanPostProcessor(BeanPostProcessor beanPostProcessor){  
        this.beanPostProcessors.remove(beanPostProcessor);  
        this.beanPostProcessors.add(beanPostProcessor);  
    }  
  
    /**  
     * Return the list of BeanPostProcessors that will get applied     * to beans created with this factory.     */    
    public List<BeanPostProcessor> getBeanPostProcessors() {  
        return this.beanPostProcessors;  
    }  
}
```
DefaultListableBeanFactory实现ConfigurableListableBeanFactory的其他方法：包括getBeanDefinition和preInstantiateSingletons
```java
package com.memcpy0.springframework.beans.factory;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.AutowireCapableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
import com.memcpy0.springframework.beans.factory.config.ConfigurableBeanFactory;  
  
/**  
 * Configuration interface to be implemented by most listable bean factories. * In addition to {@link com.memcpy0.springframework.beans.factory.config.ConfigurableBeanFactory},  
 * it provides facilities to analyze and modify bean definitions, and to pre-instantiate singletons. * 将由大多数Bean工厂实现的配置接口，提供了配置Bean工厂的方法，还有分析和修改Bean定义以及提前实例化单例的操作接口  
 */  
public interface ConfigurableListableBeanFactory extends ListableBeanFactory, AutowireCapableBeanFactory, ConfigurableBeanFactory  {  
    BeanDefinition getBeanDefinition(String beanName) throws BeansException;  
  
    void preInstantiateSingletons() throws BeansException;  
  
    void addBeanPostProcessor(BeanPostProcessor beanPostProcessor);  
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261315601.png)
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
  
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
     */    @Override  
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
     */    @Override  
    public <T> Map<String, T> getBeansOfType(Class<T> type) throws BeansException {  
        Map<String, T> result = new HashMap<>();  
        beanDefinitionMap.forEach((beanName, beanDefinition) -> {  
            Class beanClass = beanDefinition.getBeanClass();  
            if (type.isAssignableFrom(beanClass))  
                result.put(beanName, (T) getBean(beanName));  
        });  
        return result;  
    }  
  
    /**  
     * 提前实例化单例  
     * @throws BeansException  
     */    @Override  
    public void preInstantiateSingletons() throws BeansException {  
        beanDefinitionMap.keySet().forEach(this::getBean);  
    }  
}
```
#### 5. 应用上下文抽象类实现
使用了模板方法设计模式，需要具体子类去实现它。
AbstractApplicationContext 继承 DefaultResourceLoader 是**为了处理spring.xml 配置资源的加载**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261320704.png)

```java
package com.memcpy0.springframework.context.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanFactoryPostProcessor;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
import com.memcpy0.springframework.context.ConfigurableApplicationContext;  
import com.memcpy0.springframework.core.io.DefaultResourceLoader;  
  
import java.util.Map;  
  
/**  
 * Abstract implementation of the {@link com.memcpy0.springframework.context.ApplicationContext}  
 * interface. Doesn't mandate the type of storage used for configuration; simply * implements common context functionality. Uses the Template Method design pattern, * requiring concrete subclasses to implement abstract methods. * <p>  
 * 抽象应用上下文  
 * <p>  
 */  
public abstract class AbstractApplicationContext extends DefaultResourceLoader implements ConfigurableApplicationContext {  
    @Override  
    public void refresh() throws BeansException {  
        // 1. 创建BeanFactory，并加载BeanDefinition  
        refreshBeanFactory();  
        // 2. 获取BeanFactory  
        ConfigurableListableBeanFactory beanFactory = getBeanFactory();  
        // 3. 在Bean实例化之前，执行BeanFactoryPostProcessor  (Invoke factory processors registered as beans in the context.)  
        invokeBeanFactoryPostProcessors(beanFactory);  
        // 4. BeanPostProcessor需要提前于其他Bean对象实例化之前执行注册操作  
        registerBeanPostProcessors(beanFactory);  
        // 5. 提前实例化单例对象  
        beanFactory.preInstantiateSingletons();  
    }  
  
    protected abstract void refreshBeanFactory() throws BeansException;  
  
    protected abstract ConfigurableListableBeanFactory getBeanFactory();  
  
    private void invokeBeanFactoryPostProcessors(ConfigurableListableBeanFactory beanFactory) {  
        Map<String, BeanFactoryPostProcessor> beanFactoryPostProcessorMap = beanFactory.getBeansOfType(BeanFactoryPostProcessor.class);  
        for (BeanFactoryPostProcessor beanFactoryPostProcessor : beanFactoryPostProcessorMap.values()) {  
            beanFactoryPostProcessor.postProcessBeanFactory(beanFactory);  
            // 在所有的 BeanDefinition 加载完成后，实例化 Bean 对象之前，修改 BeanFactory 底层的 BeanDefinition 属性  
        }  
    }  
  
    private void registerBeanPostProcessors(ConfigurableListableBeanFactory beanFactory) {  
        Map<String, BeanPostProcessor> beanPostProcessorMap = beanFactory.getBeansOfType(BeanPostProcessor.class);  
        for (BeanPostProcessor beanPostProcessor : beanPostProcessorMap.values()) {  
            beanFactory.addBeanPostProcessor(beanPostProcessor);  
        }  
    }  
  
    @Override  
    public <T> Map<String, T> getBeansOfType(Class<T> type) throws BeansException {  
        return getBeanFactory().getBeansOfType(type);  
    }  
  
    @Override  
    public String[] getBeanDefinitionNames() {  
        return getBeanFactory().getBeanDefinitionNames();  
    }  
  
    @Override  
    public Object getBean(String name) throws BeansException {  
        return getBeanFactory().getBean(name);  
    }  
  
    @Override  
    public Object getBean(String name, Object... args) throws BeansException {  
        return getBeanFactory().getBean(name, args);  
    }  
  
    @Override  
    public <T> T getBean(String name, Class<T> requiredType) throws BeansException {  
        return getBeanFactory().getBean(name, requiredType);  
    }  
}
```
之后是在 refresh() 定义实现过程，包括：
1. 创建 BeanFactory，并加载 BeanDefinition
2. 获取 BeanFactory
3. 在 Bean 实例化之前，执行 BeanFactoryPostProcessor (Invoke factory processors registered as beans in the context.)
4. **BeanPostProcessor 需要提前于其他 Bean 对象实例化之前执行注册操作**
5. 提前实例化单例 Bean 对象

另外把定义出来的抽象方法，refreshBeanFactory()、getBeanFactory() 由后面的继承此抽象类的其他抽象类实现。

### 6. 获取Bean工厂和加载资源
context.support.AbstractRefreshableApplicationContext
- 在 refreshBeanFactory() 中主要是获取了 DefaultListableBeanFactory的实例化以及**对资源配置的加载操作**
- loadBeanDefinitions(beanFactory)，在**加载完成**后**即可完成对spring.xml 配置文件中 Bean 对象的定义和注册**，同时也包括==实现了接口BeanFactoryPostProcessor、BeanPostProcessor 的配置 Bean 信息==(在AbstractApplicationContext中)。
- 但此时资源加载还只是定义了一个抽象类方法loadBeanDefinitions(DefaultListableBeanFactory beanFactory)，继续由其他抽象类继承实现。
```java
package com.memcpy0.springframework.context.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.support.DefaultListableBeanFactory;  
  
public abstract class AbstractRefreshableApplicationContext extends AbstractApplicationContext {  
    private DefaultListableBeanFactory beanFactory;  
    private DefaultListableBeanFactory createBeanFactory() {  
        return new DefaultListableBeanFactory();  
    }  
    @Override  
    protected void refreshBeanFactory() throws BeansException {  
        DefaultListableBeanFactory beanFactory = createBeanFactory(); // 创建Bean工厂  
        loadBeanDefinitions(beanFactory); // 加载BeanDefinition  
        this.beanFactory = beanFactory;  
    }  
  
    protected abstract void loadBeanDefinitions(DefaultListableBeanFactory beanFactory);  
  
    @Override  
    protected ConfigurableListableBeanFactory getBeanFactory() {  
        return beanFactory;  
    }  
}
```
### 7. 上下文中对配置信息的加载
context.support.AbstractXmlApplicationContext
在XMLBeanDefinitionReader中添加一个函数：
```java
@Override  
public void loadBeanDefinitions(String... locations) throws BeansException {  
    for (String location : locations) {  
        loadBeanDefinitions(location);  
    }  
}
```
在 AbstractXmlApplicationContext 抽象类的 loadBeanDefinitions 方法实现中，**使用 XmlBeanDefinitionReader 类，处理了关于 XML 文件配置信息的操作**。
 
同时这里又留下了一个==抽象类方法，getConfigLocations()，此方法是为了从入口上下文类，拿到配置信息的地址描述==。
```java
package com.memcpy0.springframework.context.support;  
  
import com.memcpy0.springframework.beans.factory.support.DefaultListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.xml.XmlBeanDefinitionReader;  
  
/**  
 * Convenient base class for {@link com.memcpy0.springframework.context.ApplicationContext}  
 * implementations, drawing configuration from XML documents containing bean definitions 
 * understood by an {@link com.memcpy0.springframework.beans.factory.xml.XmlBeanDefinitionReader}.  
 * */public abstract class AbstractXmlApplicationContext extends AbstractRefreshableApplicationContext {  
    @Override  
    protected void loadBeanDefinitions(DefaultListableBeanFactory beanFactory) {  
        XmlBeanDefinitionReader beanDefinitionReader = new XmlBeanDefinitionReader(beanFactory, this); // 将继承DefaultResourceLoader的ApplicationContext传入  
        String[] configLocations = getConfigLocations();  
        if (null != configLocations) {  
            beanDefinitionReader.loadBeanDefinitions(configLocations);  
        }  
    }  
  
    protected abstract String[] getConfigLocations();  
}
```
### 8. 应用上下文实现类(ClassPathXmlApplicationContext)
ClassPathXmlApplicationContext，是**具体对外给用户提供的应用上下文方法**。
```java
package com.memcpy0.springframework.context.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
  
/**  
 * Standalone XML application context, taking the context definition files 
 * from the class path, interpreting plain paths as class path resource names 
 * that include the package path (e.g. "mypackage/myresource.txt"). Useful for 
 * test harnesses as well as for application contexts embedded within JARs. 
 * <p>  
 * 独立的XML应用程序上下文,从类路径中获取上下文定义文件,  
 * 将普通路径解释为包括包路径的类路径资源名称,  
 * 对于测试工具以及嵌入JAR中的应用程序上下文都很有用  
 * <p>  
 */  
public class ClassPathXmlApplicationContext extends AbstractXmlApplicationContext {  
    private String[] configLocations;  
    public ClassPathXmlApplicationContext() {}  
    /**  
     * 从 XML 中加载 BeanDefinition，并刷新上下文  
     * @param configLocations  
     * @throws BeansException  
     */    
    public ClassPathXmlApplicationContext(String configLocations) throws BeansException {  
        this(new String[]{configLocations});  
    }  
  
    /**  
     * 从 XML 中加载 BeanDefinition，并刷新上下文  
     * @param configLocations  
     * @throws BeansException  
     */    
    public ClassPathXmlApplicationContext(String[] configLocations) throws BeansException {  
        this.configLocations = configLocations;  
        refresh();  
    }  
  
    @Override  
    protected String[] getConfigLocations() {  
        return configLocations;  
    }  
}
```
在继承了 AbstractXmlApplicationContext 以及层层抽象类的功能分离实现后，在此类 ClassPathXmlApplicationContext 的实现中就简单多了，==主要是**对继承抽象类中方法的调用**和**提供了配置文件地址信息**==。

### 9. 在Bean创建前后完成前置 后置处理
实现 BeanPostProcessor 接口后，会涉及到两个接口方法，
postProcessBeforeInitialization、postProcessAfterInitialization，分别作用于 **Bean 对象执行初始化前后的额外处理**。

也就是需要在创建 Bean 对象时，在 createBean 方法中添加initializeBean(beanName, bean, beanDefinition); 操作。==而这个操作主要主要是对于方法 applyBeanPostProcessorsBeforeInitialization、applyBeanPostProcessorsAfterInitialization 的使用==。另外需要提一下，applyBeanPostProcessorsBeforeInitialization、applyBeanPostProcessorsAfterInitialization 两个方法是在接口类AutowireCapableBeanFactory 中新增加的。
```java
package com.memcpy0.springframework.beans.factory.config;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.BeanFactory;  
  
/**  
 * Extension of the {@link com.memcpy0.springframework.beans.factory.BeanFactory}  
 * interface to be implemented by bean factories that are capable of   
 * autowiring, provided that they want to expose this functionality for 
 * existing bean instances. 
 * 由进行自动装配的Bean工厂实现，前提是他们希望为现有的Bean实例公开此功能  
 */  
public interface AutowireCapableBeanFactory extends BeanFactory {  
    /**  
     * 执行 BeanPostProcessors 接口实现类的 postProcessBeforeInitialization 方法  
     *  
     * @param existingBean  
     * @param beanName  
     * @return  
     * @throws BeansException  
     */    Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName) throws BeansException;  
  
    /**  
     * 执行 BeanPostProcessors 接口实现类的 postProcessorsAfterInitialization 方法  
     *  
     * @param existingBean  
     * @param beanName  
     * @return  
     * @throws BeansException  
     */    Object applyBeanPostProcessorsAfterInitialization(Object existingBean, String beanName) throws BeansException;  
  
}
```

beans.factory.support.AbstractAutowireCapableBeanFactory
```java
package com.memcpy0.springframework.beans.factory.support;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.PropertyValues;  
import com.memcpy0.springframework.beans.factory.config.AutowireCapableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
import com.memcpy0.springframework.beans.factory.config.BeanReference;  
import cn.hutool.core.bean.BeanUtil;  
  
import java.lang.reflect.Constructor;  
  
public abstract class AbstractAutowireCapableBeanFactory extends AbstractBeanFactory implements AutowireCapableBeanFactory {  
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
            // 执行Bean的初始化方法和BeanPostProcessor的前置和后置处理方法  
            bean = initializeBean(beanName, bean, beanDefinition);  
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
  
    private Object initializeBean(String beanName, Object bean, BeanDefinition beanDefinition) {  
        // 1. 执行BeanPostProcessor Before处理  
        Object wrappedBean = applyBeanPostProcessorsBeforeInitialization(bean, beanName);  
        // 待完成内容  
        invokeInitMethods(beanName, wrappedBean, beanDefinition);  
        // 2. 执行BeanPostProcessor After处理  
        wrappedBean = applyBeanPostProcessorsAfterInitialization(bean, beanName);  
        return wrappedBean;  
    }  
  
    private void invokeInitMethods(String beanName, Object wrappedBean, BeanDefinition beanDefinition) {  
  
    }  
  
    @Override  
    public Object applyBeanPostProcessorsBeforeInitialization(Object existingBean, String beanName) throws BeansException {  
        Object result = existingBean;  
        for (BeanPostProcessor processor : getBeanPostProcessors()) {  
            Object current = processor.postProcessBeforeInitialization(result, beanName);  
            if (null == current) return result; // 当前Bean为空，不继续后置处理  
            result = current;  
        }  
        return result;  
    }  
  
    @Override  
    public Object applyBeanPostProcessorsAfterInitialization(Object existingBean, String beanName) throws BeansException {  
        Object result = existingBean;  
        for (BeanPostProcessor processor : getBeanPostProcessors()) {  
            Object current = processor.postProcessAfterInitialization(result, beanName);  
            if (null == current) return result; // 当前Bean为空，不继续后置处理  
            result = current;  
        }  
        return result;  
    }  
}
```
---
### 测试-事先准备
这里新增加了 company、location，两个属性信息，便于测试BeanPostProcessor、BeanFactoryPostProcessor **两个接口对 Bean 属性信息扩展的作用**。
```java
package com.memcpy0.bean;  
  
public class UserService {  
    private String uId;  
    private String company;  
    private String location;  
    private UserDao userDao;  
  
    public UserService() {}  
      
    public String getuId() { return uId; }  
  
    public void setuId(String uId) { this.uId = uId; }  
  
    public String getCompany() { return company; }  
  
    public void setCompany(String uId) { this.company = company; }  
  
    public String getLocation() { return location; }  
  
    public void setLocation(String location) { this.location = location; }  
  
    public UserDao getUserDao() { return userDao; }  
  
    public void setUserDao(UserDao userDao) { this.userDao = userDao; }  
    public String queryUserInfo() {  
        return userDao.queryUserName(uId) + "," + company + "," + location;  
    }  
}
```
test.common.MyBeanFactoryPostProcessor：
```java
package com.memcpy0.common;  
  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.PropertyValue;  
import com.memcpy0.springframework.beans.PropertyValues;  
import com.memcpy0.springframework.beans.factory.ConfigurableListableBeanFactory;  
import com.memcpy0.springframework.beans.factory.config.BeanDefinition;  
import com.memcpy0.springframework.beans.factory.config.BeanFactoryPostProcessor;  
  
public class MyBeanFactoryPostProcessor implements BeanFactoryPostProcessor {  
  
    @Override  
    public void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException {  
        BeanDefinition beanDefinition = beanFactory.getBeanDefinition("userService");  
        PropertyValues propertyValues = beanDefinition.getPropertyValues();  
        propertyValues.addPropertyValue(new PropertyValue("company", "改为：abaaba"));  
    }  
}
```
如果你在 Spring 中做过一些组件的开发那么一定非常熟悉这两个类，本文的测试也是实现了这两个类，**对（createBean中）实例化过程中的 Bean 对象做一些操作**。
```java
package com.memcpy0.common;  
  
import com.memcpy0.bean.UserService;  
import com.memcpy0.springframework.beans.BeansException;  
import com.memcpy0.springframework.beans.factory.config.BeanPostProcessor;  
  
public class MyBeanPostProcessor implements BeanPostProcessor {  
  
    @Override  
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {  
        if ("userService".equals(beanName)) {  
            UserService userService = (UserService) bean;  
            userService.setLocation("改为：深圳");  
        }  
        return bean;  
    }  
  
    @Override  
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {  
        return bean;  
    }  
}
```
### 配置文件
基础配置，无 BeanFactoryPostProcessor、BeanPostProcessor，实现类
```html
<?xml version="1.0" encoding="UTF-8"?>  
<beans>  
  
    <bean id="userDao" class="com.memcpy0.bean.UserDao"/>  
  
    <bean id="userService" class="com.memcpy0.bean.UserService">  
        <property name="uId" value="10001"/>  
        <property name="company" value="腾讯"/>  
        <property name="location" value="杭州"/>  
        <property name="userDao" ref="userDao"/>  
    </bean>  
</beans>
```
增强配置，有 BeanFactoryPostProcessor、BeanPostProcessor，实现类
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<beans>  
    <bean id="userDao" class="com.memcpy0.bean.UserDao"/>  
  
    <bean id="userService" class="com.memcpy0.bean.UserService">  
        <property name="uId" value="10001"/>  
        <property name="company" value="腾讯"/>  
        <property name="location" value="杭州"/>  
        <property name="userDao" ref="userDao"/>  
    </bean>    <bean class="com.memcpy0.common.MyBeanPostProcessor"/>  
    <bean class="com.memcpy0.common.MyBeanFactoryPostProcessor"/>  
</beans>
```
这里提供了两个配置文件，一个是不包含 BeanFactoryPostProcessor、
BeanPostProcessor，另外一个是包含的。之所以这样配置主要对照验证，**在运用 Spring 新增加的应用上下文和不使用的时候，都是怎么操作的**。

### 不用应用上下文
```java
@Test  
public void test_BeanFactoryPostProcessorAndBeanPostProcessor(){  
   // 1.初始化 BeanFactory   
   DefaultListableBeanFactory beanFactory = new DefaultListableBeanFactory();  
  
   // 2. 读取配置文件&注册Bean  
   XmlBeanDefinitionReader reader = new XmlBeanDefinitionReader(beanFactory);  
   reader.loadBeanDefinitions("classpath:spring.xml");  
  
   // 3. BeanDefinition 加载完成 & Bean实例化之前，修改 BeanDefinition 的属性值  
   MyBeanFactoryPostProcessor beanFactoryPostProcessor = new MyBeanFactoryPostProcessor();  
   beanFactoryPostProcessor.postProcessBeanFactory(beanFactory);  
  
   // 4. Bean实例化之后，修改 Bean 属性信息  
   MyBeanPostProcessor beanPostProcessor = new MyBeanPostProcessor();  
   beanFactory.addBeanPostProcessor(beanPostProcessor);  
  
   // 5. 获取Bean对象调用方法  
   UserService userService = beanFactory.getBean("userService", UserService.class);  
   String result = userService.queryUserInfo();  
   System.out.println("测试结果：" + result);  
}
```
DefaultListableBeanFactory 创建 beanFactory 并使用 XmlBeanDefinitionReader 加载配置文件的方式，还是比较熟悉的。

接下来就是对 MyBeanFactoryPostProcessor 和 MyBeanPostProcessor 的处理，一个是在 BeanDefinition 加载完成 & Bean 实例化之前，修改 BeanDefinition 的属性值，另外一个是在 Bean 实例化之后，修改 Bean 属性信息。

通过测试结果可以看到，我们配置的属性信息已经与 spring.xml 配置文件中不一样了
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261451340.png)

### 使用应用上下文
```java
@Test  
public void test_xml() {  
  // 1.初始化 BeanFactory      ClassPathXmlApplicationContext applicationContext = new ClassPathXmlApplicationContext("classpath:springPostProcessor.xml");  

  // 2. 获取Bean对象调用方法  
  UserService userService = applicationContext.getBean("userService", UserService.class);  
  String result = userService.queryUserInfo();  
  System.out.println("测试结果：" + result);  
}   
```
另外使用**新增加的 ClassPathXmlApplicationContext 应用上下文类==**，再操作起来就方便多了，这才是面向用户使用的类==，在这里可以一步把配置文件交给ClassPathXmlApplicationContext，也**不需要管理一些**自定义实现的 Spring 接口的类。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304261454930.png)
### 总结
本文主要新增了 Spring 框架中两个非常重要的接口 BeanFactoryPostProcess、BeanPostProcessor 同时还添加了**关于应用上下文的实现**，==ApplicationContext 接口的定义是继承 BeanFactory 外新增加功能的接口，它可以满足于自动识别、资源加载、容器事件、监听器等功能，同时例如一些国际化支持、单例 Bean 自动初始化等，也是可以在这个类里实现和扩充的==。

通过本文的实现一定会非常了解 BeanFactoryPostProcessor、BeanPostProcessor，以后再做一些关于 Spring 中间件的开发时，**如果需要用到 Bean 对象的获取以及修改一些属性信息，那么就可以使用这两个接口了**。同时 ==BeanPostProcessor 也是实现 AOP 切面技术的关键所在==。

> 其实核心技术的原理学习，是更有助于你完成更复杂的架构设计，当你的知识能更全面覆盖所承接的需求时，也就能更好的做出合理的架构和落地。


---
