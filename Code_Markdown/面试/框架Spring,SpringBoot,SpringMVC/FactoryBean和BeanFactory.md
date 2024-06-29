// 实现FactoryBean接口,目的在于简化Bean的实例化过程  
// 1. 通过FactoryBean封装了Bean的实例化过程  
// 2. 将FactoryBean装配到Spring容器中  
// 3. 通过FactoryBean注入给其他Bean  
// 4. 该Bean得到的是FactoryBean管理的对象实例

```java
@Configuration  
public class QuartzConfig {  
    // 实现FactoryBean接口,目的在于简化Bean的实例化过程  
    // 1. 通过FactoryBean封装了Bean的实例化过程  
    // 2. 将FactoryBean装配到Spring容器中  
    // 3. 通过FactoryBean注入给其他Bean  
    // 4. 该Bean得到的是FactoryBean管理的对象实例  
    @Bean  
    public JobDetailFactoryBean alphaJobDetail() { // 配JobDetail  
        // JobDetailFactoryBean底层封装了JobDetail详细实例化的过程  
        JobDetailFactoryBean factoryBean = new JobDetailFactoryBean();  
        factoryBean.setJobClass(AlphaJob.class);  
        factoryBean.setName("alphaJob");  
        factoryBean.setGroup("alphaJobGroup"); // 多个任务可以同属于一组  
        factoryBean.setDurability(true); // 任务是否持久保存  
        factoryBean.setRequestsRecovery(true); //  
        return factoryBean;  
    }  
    // 配置Trigger: SimpleTriggerFactoryBean, CronTriggerFactoryBean  
    @Bean  
    public SimpleTriggerFactoryBean alphaTrigger(JobDetail alphaJobDetail) { // 配置Trigger,依赖于JobDetail  
        return null;  
    }  
  
      
  
}
```