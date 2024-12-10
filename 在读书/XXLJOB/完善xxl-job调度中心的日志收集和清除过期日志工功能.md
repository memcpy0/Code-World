大家好，上一章分我为大家详细剖析了执行器那一端的日志收集，以及定时任务执行结果回调给调度中心的功能如何实现。这一章就简单讲讲调度中心怎么处理回调过来的信息。以及根据回调过来的信息做什么操作。因为剩下的知识已经特比少了，所以我就不再带着大家详细剖析了，也不再举例迭代了，只给大家简单讲解这一章会引入什么新的组件，然后把组件的代码贴一下，简单讲解一下就行。其实具体的细节大家直接去看代码就行了，学到这里，也没剩下什么知识点了，直接去看代码绝对可以学会剩下的知识了。

上一章讲到了执行器那一端把定时任务的执行结果给回调过来了，大家应该还有印象，就是在调度任务真正开始调度任务之前，会先把定时任务的执行信息封装到XxlJobLog对象中，这个日志对象就是专门用来存储定时任务的执行信息的。具体的执行逻辑请看下面的代码块。

```
public class XxlJobTrigger {

    
    private static void processTrigger(XxlJobGroup group, XxlJobInfo jobInfo, int finalFailRetryCount, TriggerTypeEnum triggerType, int index, int total){
        //得到当前要调度的执行任务的路由策略，默认是没有
        ExecutorRouteStrategyEnum executorRouteStrategyEnum = ExecutorRouteStrategyEnum.match(jobInfo.getExecutorRouteStrategy(), null);
        //这里就要开始执行和定时任务日志相关的操作了
        //先创建一个日志对象，用于记录该定时任务执行的一些信息
        XxlJobLog jobLog = new XxlJobLog();
        //记录定时任务的执行器组id
        jobLog.setJobGroup(jobInfo.getJobGroup());
        //设置定时任务的id
        jobLog.setJobId(jobInfo.getId());
        //设置定时任务的触发时间
        jobLog.setTriggerTime(new Date());
        //在这里把定时任务日志保存到数据库中，保存成功之后，定时任务日志的id也就有了
        XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().save(jobLog);
        logger.debug(">>>>>>>>>>> xxl-job trigger start, jobId:{}", jobLog.getId());

        
        
        //中间的代码暂时省略

        
        //设置执行器地址
        jobLog.setExecutorAddress(address);
        //设置执行定时任务的方法名称
        jobLog.setExecutorHandler(jobInfo.getExecutorHandler());
        //设置执行参数
        jobLog.setExecutorParam(jobInfo.getExecutorParam());
        //jobLog.setExecutorShardingParam(shardingParam);
        //设置失败重试次数
        jobLog.setExecutorFailRetryCount(finalFailRetryCount);
        //设置触发结果码
        jobLog.setTriggerCode(triggerResult.getCode());
        //设置触发任务信息，也就是调度备注
        jobLog.setTriggerMsg(triggerMsgSb.toString());
        //更新数据库信息
        XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().updateTriggerInfo(jobLog);
        logger.debug(">>>>>>>>>>> xxl-job trigger end, jobId:{}", jobLog.getId());
    }
}
```

在上面的代码块中，虽然XxlJobLog内部的属性已经有一些被赋值了，但是最重要的一个属性，就是handleCode还没有被赋值。这个属性就是定时任务执行结果的状态码，这个状态码是从执行器那一端回调发送给调度中心的。下面，请先看一看XxlJobLog对象的内部结构。

```
public class XxlJobLog {

	//日志id
	private long id;
	//执行器组id
	private int jobGroup;
	//定时任务id
	private int jobId;
	//执行器地址
	private String executorAddress;
	//封装定时任务的JobHandler的名称
	private String executorHandler;
	//执行器参数
	private String executorParam;
	//执行器分片参数
	private String executorShardingParam;
	//失败重试次数
	private int executorFailRetryCount;
	//触发器触发时间
	private Date triggerTime;
	//触发器任务的响应码
	private int triggerCode;
	//触发任务信息
	private String triggerMsg;
	//定时任务执行时间
	private Date handleTime;
	//执行的响应码
	private int handleCode;
	//执行的具体结果
	private String handleMsg;
	//警报的状态码 0是默认，1是不需要报警，2是报警成功，3是报警失败
	private int alarmStatus;

    //其它get和set方法省略
    
}
```

接着请大家再看一下执行器那一端回调给调度中心的执行结果的信息，这些信息是封装在HandleCallbackParam对象中的，该对象中恰好也有一个handleCode，就是用来被赋值给XxlJobLog对象中的handleCode属性的。请看下面的代码块。

```
public class HandleCallbackParam implements Serializable {
    private static final long serialVersionUID = 42L;
	//这个就是定时任务的主键ID
    private long logId;
    private long logDateTim;
    //定时任务执行结果的状态码，成功还是失败
    private int handleCode;
    private String handleMsg;

    //其他内容暂且省略
}
```

这些算是上一章的知识了，现在，让我们来看看执行器把定时任务执行结果的信息发送给调度中心后，调度中心是怎么接收的。当然，上一章我给大家返回强调了，执行器是通过http协议发送的定时任务执行结果信息给调度中心，那么调度中心肯定就是要以http的放方式来接受的。具体的逻辑就在调度中心的JobApiController这个类中。请大家看下面的代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/7/11
 * @Description:这个类不对web界面开放，而是程序内部执行远程调用时使用的，这个类中的接口是对执行器那一端暴露的
 */
@Controller
@RequestMapping("/api")
public class JobApiController {

    @Resource
    private AdminBiz adminBiz;

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/11
     * @Description:该方法就是执行注册执行器的方法，执行器那一端会访问下面这个接口，进行注册
     * 其实该方法内还有其他功能，这里暂时只引入注册功能
     */
    @RequestMapping("/{uri}")
    @ResponseBody
    @PermissionLimit(limit=false)
    public ReturnT<String> api(HttpServletRequest request, @PathVariable("uri") String uri, @RequestBody(required = false) String data) {
        //判断是不是post请求
        if (!"POST".equalsIgnoreCase(request.getMethod())) {
            return new ReturnT<String>(ReturnT.FAIL_CODE, "invalid request, HttpMethod not support.");
        }
        //对路径做判空处理
        if (uri==null || uri.trim().length()==0) {
            return new ReturnT<String>(ReturnT.FAIL_CODE, "invalid request, uri-mapping empty.");
        }
        //判断执行器配置的token和调度中心的是否相等
        if (XxlJobAdminConfig.getAdminConfig().getAccessToken()!=null
                && XxlJobAdminConfig.getAdminConfig().getAccessToken().trim().length()>0
                && !XxlJobAdminConfig.getAdminConfig().getAccessToken().equals(request.getHeader(XxlJobRemotingUtil.XXL_JOB_ACCESS_TOKEN))) {
            return new ReturnT<String>(ReturnT.FAIL_CODE, "The access token is wrong.");
        }
        //判断是不是执行器端回调定时任务执行结果了
        if ("callback".equals(uri)) {
            List<HandleCallbackParam> callbackParamList = GsonTool.fromJson(data, List.class, HandleCallbackParam.class);
            return adminBiz.callback(callbackParamList);
        }
        //判断是不是注册操作
        else if ("registry".equals(uri)) {
            RegistryParam registryParam = GsonTool.fromJson(data, RegistryParam.class);
            //执行注册任务
            return adminBiz.registry(registryParam);
            //判断是不是从调度中心移除执行器的操作
        } else if ("registryRemove".equals(uri)) {
            RegistryParam registryParam = GsonTool.fromJson(data, RegistryParam.class);
            //执行移除任务
            return adminBiz.registryRemove(registryParam);
        } else {
            //都不匹配则返回失败
            return new ReturnT<String>(ReturnT.FAIL_CODE, "invalid request, uri-mapping("+ uri +") not found.");
        }

    }

}
```

如果url匹配的就是callback字符串，那就说明是执行器那一端发送定时任务执行信息给调度中心了，然后再调度中心这一端，程序就会来打AdminBizImpl类中。请看下面的代码块。

```
@Service
public class AdminBizImpl implements AdminBiz {


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/17
     * @Description:调度中心要调用的方法，把执行器回调的定时任务执行的结果信息收集起来
     */
    @Override
    public ReturnT<String> callback(List<HandleCallbackParam> callbackParamList) {
        return JobCompleteHelper.getInstance().callback(callbackParamList);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/4
     * @Description:把执行器注册到注册中心
     */
    @Override
    public ReturnT<String> registry(RegistryParam registryParam) {
        //通过JobRegistryHelper组件中创建的线程池来完成注册任务
        return JobRegistryHelper.getInstance().registry(registryParam);
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/4
     * @Description:移除执行器的方法
     */
    @Override
    public ReturnT<String> registryRemove(RegistryParam registryParam) {
        return JobRegistryHelper.getInstance().registryRemove(registryParam);
    }

}
```

在上面代码块的callback方法中，调度中心的JobCompleteHelper组件就终于启动了。当然，这个组件就是这一章新引入的组件，就是这一章要讲解的知识。下面我就把这个JobCompleteHelper新的组件，其实就是一个新的类，把这个类的代码贴在下面，然后简单讲解一下。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/7/17
 * @Description:调度中心接收执行器回调信息的工作组件
 */
public class JobCompleteHelper {

	private static Logger logger = LoggerFactory.getLogger(JobCompleteHelper.class);
	//单例对象
	private static JobCompleteHelper instance = new JobCompleteHelper();

	public static JobCompleteHelper getInstance(){
		return instance;
	}

	//回调线程池，这个线程池就是处理执行器端回调过来的日志信息的
	private ThreadPoolExecutor callbackThreadPool = null;
	//监控线程
	private Thread monitorThread;
	private volatile boolean toStop = false;


	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/17
	 * @Description:启动该组件
	 */
	public void start(){
		//创建回调线程池
		callbackThreadPool = new ThreadPoolExecutor(
				2,
				20,
				30L,
				TimeUnit.SECONDS,
				new LinkedBlockingQueue<Runnable>(3000),
				new ThreadFactory() {
					@Override
					public Thread newThread(Runnable r) {
						return new Thread(r, "xxl-job, admin JobLosedMonitorHelper-callbackThreadPool-" + r.hashCode());
					}
				},
				new RejectedExecutionHandler() {
					@Override
					public void rejectedExecution(Runnable r, ThreadPoolExecutor executor) {
						r.run();
						logger.warn(">>>>>>>>>>> xxl-job, callback too fast, match threadpool rejected handler(run now).");
					}
				});


		//创建监控线程
		monitorThread = new Thread(new Runnable() {
			@Override
			public void run() {
				//这里休息了一会，是因为需要等待JobTriggerPoolHelper组件初始化，因为不执行远程调度，也就没有
				//回调过来的定时任务执行结果信息
				try {
					TimeUnit.MILLISECONDS.sleep(50);
				} catch (InterruptedException e) {
					if (!toStop) {
						logger.error(e.getMessage(), e);
					}
				}
				while (!toStop) {
					try {
						//这里得到了一个时间信息，就是当前时间向前10分钟的时间
						//这里传进去的参数-10，就是减10分钟的意思
						Date losedTime = DateUtil.addMinutes(new Date(), -10);
						//这里最后对应的就是这条sql语句
						//t.trigger_code = 200 AND t.handle_code = 0 AND t.trigger_time <![CDATA[ <= ]]> #{losedTime} AND t2.id IS NULL
						//其实就是判断了一下，现在在数据库中的xxljoblog的触发时间，其实就可以当作定时任务在调度中心开始执行的那个时间
						//这里其实就是把当前时间前十分钟内提交执行的定时任务，但是始终没有得到执行器回调的执行结果的定时任务全找出来了
						//因为t.handle_code = 0，并且注册表中也没有对应的数据了，说明心跳断了
						//具体的方法在XxlJobLogMapper中
						List<Long> losedJobIds  = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().findLostJobIds(losedTime);
						if (losedJobIds!=null && losedJobIds.size()>0) {
							//开始遍历定时任务
							for (Long logId: losedJobIds) {
								XxlJobLog jobLog = new XxlJobLog();
								jobLog.setId(logId);
								//设置执行时间
								jobLog.setHandleTime(new Date());
								//设置失败状态
								jobLog.setHandleCode(ReturnT.FAIL_CODE);
								jobLog.setHandleMsg( I18nUtil.getString("joblog_lost_fail") );
								//更新失败的定时任务状态
								XxlJobCompleter.updateHandleInfoAndFinish(jobLog);
							}
						}
					}
					catch (Exception e) {
						if (!toStop) {
							logger.error(">>>>>>>>>>> xxl-job, job fail monitor thread error:{}", e);
						}
					}
                    try {
						//每60秒工作一次
                        TimeUnit.SECONDS.sleep(60);
                    } catch (Exception e) {
                        if (!toStop) {
                            logger.error(e.getMessage(), e);
                        }
                    }
                }
				logger.info(">>>>>>>>>>> xxl-job, JobLosedMonitorHelper stop");
			}
		});
		monitorThread.setDaemon(true);
		monitorThread.setName("xxl-job, admin JobLosedMonitorHelper");
		monitorThread.start();
	}


	//终止组件工作的方法
	public void toStop(){
		toStop = true;
		callbackThreadPool.shutdownNow();
		monitorThread.interrupt();
		try {
			monitorThread.join();
		} catch (InterruptedException e) {
			logger.error(e.getMessage(), e);
		}
	}


	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/17
	 * @Description:处理回调信息的方法
	 */
	public ReturnT<String> callback(List<HandleCallbackParam> callbackParamList) {
		callbackThreadPool.execute(new Runnable() {
			@Override
			public void run() {
				for (HandleCallbackParam handleCallbackParam: callbackParamList) {
					//在这里处理每一个回调的信息
					ReturnT<String> callbackResult = callback(handleCallbackParam);
					logger.debug(">>>>>>>>> JobApiController.callback {}, handleCallbackParam={}, callbackResult={}",
							(callbackResult.getCode()== ReturnT.SUCCESS_CODE?"success":"fail"), handleCallbackParam, callbackResult);
				}
			}
		});

		return ReturnT.SUCCESS;
	}


	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/17
	 * @Description:真正处理回调信息的方法
	 */
	private ReturnT<String> callback(HandleCallbackParam handleCallbackParam) {
		//得到对应的xxljoblog对象
		XxlJobLog log = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().load(handleCallbackParam.getLogId());
		if (log == null) {
			return new ReturnT<String>(ReturnT.FAIL_CODE, "log item not found.");
		}
		//判断日志对象的处理结果码
		//因为这个响应码无论是哪种情况都是大于0的，如果大于0了，说明已经回调一次了
		//如果等于0，说明还没得到回调信息，任务也可能还处于运行中状态
		if (log.getHandleCode() > 0) {
			return new ReturnT<String>(ReturnT.FAIL_CODE, "log repeate callback.");
		}
		//拼接信息
		StringBuffer handleMsg = new StringBuffer();
		if (log.getHandleMsg()!=null) {
			handleMsg.append(log.getHandleMsg()).append("<br>");
		}
		if (handleCallbackParam.getHandleMsg() != null) {
			handleMsg.append(handleCallbackParam.getHandleMsg());
		}
		log.setHandleTime(new Date());
		//在这里把定时任务执行的状态码赋值给XxlJobLog对象中的handleCode成员变量了
		log.setHandleCode(handleCallbackParam.getHandleCode());
		log.setHandleMsg(handleMsg.toString());
		//更新数据库中的日志信息
		XxlJobCompleter.updateHandleInfoAndFinish(log);
		return ReturnT.SUCCESS;
	}
}
```

可以看到，在JobCompleteHelper组件中，有两个特别重要的成员变量，一个就是callbackThreadPool线程池，一个就是monitorThread监控线程。callbackThreadPool线程池负责把执行器发送回来的定时任务执行信息赋值给XxlJobLog对象中的成员变量，然后更新数据库中XxlJobLog的信息。这样，定时任务的执行信息就终于完整了。整个操作就是在上看代码块的callback方法内执行的，而在callback方法中，又会调用到该类的被private修饰的callback方法。就是在这个callback方法中，会执行下面的几行核心代码。请看下面的代码块。

```
	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/17
	 * @Description:真正处理回调信息的方法
	 */
	private ReturnT<String> callback(HandleCallbackParam handleCallbackParam) {
		//得到对应的xxljoblog对象
		XxlJobLog log = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().load(handleCallbackParam.getLogId());
		if (log == null) {
			return new ReturnT<String>(ReturnT.FAIL_CODE, "log item not found.");
		}
		//判断日志对象的处理结果码
		//因为这个响应码无论是哪种情况都是大于0的，如果大于0了，说明已经回调一次了
		//如果等于0，说明还没得到回调信息，任务也可能还处于运行中状态
		if (log.getHandleCode() > 0) {
			return new ReturnT<String>(ReturnT.FAIL_CODE, "log repeate callback.");
		}


    	//省略中间的代码


		//在这里把定时任务执行的状态码赋值给XxlJobLog对象中的handleCode成员变量了
		log.setHandleCode(handleCallbackParam.getHandleCode());
		log.setHandleMsg(handleMsg.toString());
		//更新数据库中的日志信息
		XxlJobCompleter.updateHandleInfoAndFinish(log);
		return ReturnT.SUCCESS;
	}
}
```

到此为止，callbackThreadPool线程池的作用就讲解完了，并且调度中心这边，定时任务的执行信息也终于补全完整了，当然，到此为止，也仅仅是把定时任务的执行信息补全完整了，而判断定时任务是否执行失败的逻辑，我还没有为大家讲解(其实这里说的有些不严谨，当定时任务的执行结果信息从执行器返回之后，定时任务的执行结果是成功还是失败就已经一目了然了，我这里之所以这么说，是为了引出另一种任务执行失败的情况。也就是monitorThread线程要进行的操作)。接下来，让我们看看JobCompleteHelper类中的monitorThread监控线程的作用，该线程的作用就是用来判断调度中心调度的哪些定时任务真的是失败了。通过上一章的学习，我们都知道定时任务在被调度中心调度的时候，会创建一个XxlJobLog对象存储定时任务的执行信息，然后把这个对象的信息存储到数据库中，但是在存储数据库的时候，其内部成员变量handleCode实际上没有赋值的，在数据库中默认为0。只有当执行器那一端返回定时任务的执行结果信息了，handleCode属性才会成功复制，这时候它的值才会大于0。因为在执行器那一端为handleCode属性设置了三个状态值，请看下面的代码块。

```
public class XxlJobContext {

    public static final int HANDLE_CODE_SUCCESS = 200;
    public static final int HANDLE_CODE_FAIL = 500;
    public static final int HANDLE_CODE_TIMEOUT = 502;

    //其他内容省略

}
```

handleCode属性可以被成功，失败和超时这三个整数赋值。这也就意味着，只要XxlJobLog对象的handleCode属性为0，就说明当前调度的定时任务还没有一个执行结果，只要数据库中XxlJobLog对象的handleCode属性不为0了，就说明被调度的定时任务一定有结果了，因为从执行器那一端的返回的handleCode状态码，不管是成功，失败还是超时，都是大于0的。所以，完全可以根据这个状态码的值来判断当前定时任务被调度的状态，看是否收到执行结果了。如果理解了这个逻辑，那么接下来，我们就可以看看monitorThread线程的工作内容了。请看下面的代码块。

```
//创建监控线程
monitorThread = new Thread(new Runnable() {
	@Override
	public void run() {
		//这里休息了一会，是因为需要等待JobTriggerPoolHelper组件初始化，因为不执行远程调度，也就没有
		//回调过来的定时任务执行结果信息
		try {
			TimeUnit.MILLISECONDS.sleep(50);
		} catch (InterruptedException e) {
			if (!toStop) {
				logger.error(e.getMessage(), e);
			}
		}
		while (!toStop) {
			try {
				//这里得到了一个时间信息，就是当前时间向前10分钟的时间
				//这里传进去的参数-10，就是减10分钟的意思
				Date losedTime = DateUtil.addMinutes(new Date(), -10);
				//这里最后对应的就是这条sql语句
				//t.trigger_code = 200 AND t.handle_code = 0 AND t.trigger_time <![CDATA[ <= ]]> #{losedTime} AND t2.id IS NULL
				//其实就是判断了一下，现在在数据库中的xxljoblog的触发时间，其实就可以当作定时任务在调度中心开始执行的那个时间
				//这里其实就是把当前时间前十分钟内提交执行的定时任务，但是始终没有得到执行器回调的执行结果的定时任务全找出来了
				//因为t.handle_code = 0，并且注册表中也没有对应的数据了，说明心跳断了
				//具体的方法在XxlJobLogMapper中
				List<Long> losedJobIds  = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().findLostJobIds(losedTime);
				if (losedJobIds!=null && losedJobIds.size()>0) {
					//开始遍历定时任务
					for (Long logId: losedJobIds) {
						XxlJobLog jobLog = new XxlJobLog();
						jobLog.setId(logId);
						//设置执行时间
						jobLog.setHandleTime(new Date());
						//设置失败状态
						jobLog.setHandleCode(ReturnT.FAIL_CODE);
						jobLog.setHandleMsg( I18nUtil.getString("joblog_lost_fail") );
						//更新失败的定时任务状态
						XxlJobCompleter.updateHandleInfoAndFinish(jobLog);
					}
				}
			}
			catch (Exception e) {
				if (!toStop) {
					logger.error(">>>>>>>>>>> xxl-job, job fail monitor thread error:{}", e);
				}
			}
            try {
				//每60秒工作一次
                TimeUnit.SECONDS.sleep(60);
            } catch (Exception e) {
                if (!toStop) {
                    logger.error(e.getMessage(), e);
                }
            }
        }
		logger.info(">>>>>>>>>>> xxl-job, JobLosedMonitorHelper stop");
	}
});
monitorThread.setDaemon(true);
monitorThread.setName("xxl-job, admin JobLosedMonitorHelper");
monitorThread.start();
```

可以看到，在上面的代码块中，monitorThread线程启动之后，就会在一个循环中不断地从数据库中查找定时任务的执行信息，并且，查找的时候，会以当前时间为标志，查找到当前时间的十分钟之前调度的所有定时任务的信息，当然，返回的就是这些定时任务的ID。为什么要这么查找呢？原因其实也不复杂，如果一个定时任务被调度了十分钟了，仍然没有收到执行结果，那这个定时任务的执行肯定就出问题了呀。具体查找的逻辑已经添加在上面代码块的注释中了，最关键的一点就是在sql语句中判断handle_code为0，至于为什么要判断这个字段，在上面已经解释过了。当然，monitorThread线程在查找执行失败的定时任务信息时，还在sql语句中附加了一个条件，那就是定时任务所部署的服务器已经不在和调度中心维持心跳了，就是AND t2.id IS NULL这行代码，这就表明要查询的地址只存在于XxlJobLog对应的表中，而在xxl_job_register这种表中并没有对应的数据，这不就意味着执行定时任务的执行器因为心跳断连而被删除了吗？所以，这就是我要说的另一种定时任务执行失败的情况。

现在，大家应该明白了，定时任务被调度后如果收到执行结果了，会在JobCompleteHelper类中通过callbackThreadPool线程池来设置真正的执行结果，但同时还有另一种情况，那就是定时任务执行了10分钟了还没结果，并且执行器的服务器下线了，这时候也要设置执行失败的结果。至于这两种定时任务结果设置有什么不同，可以用一句话来总结，那就是callbackThreadPool线程池设置会设置成功，失败，超时三种结果，而monitorThread线程设置的只有执行器下线并且定时任务执行失败的结果。

好了，定时任务执行结果的内容就告一段落吧，下面，让我们来看看调度中心这一端过期日志是如何清楚的。要想清楚过期日志，首先得明确日志的过期时间，而日志的过期时间是要让用户自己配置的，就定义在配置文件中。请看下面代码块。

```
//xxl-job, log retention days
xxl.job.logretentiondays=30
```

从上面的代码块中可以看出来，日志的过期时间默认为30天，当然，这个值用户是可以随意更改的。而清楚过期日志也是需要引入新的组件的，该组件就是JobLogReportHelper。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/1
 * @Description:该组件的功能也很简答，就是统计定时任务日志的信息，成功失败次数等等
 * 同时也会清除过期日志，过期日志时间是用户写在配置文件中的，默认为30天
 */
public class JobLogReportHelper {

    private static Logger logger = LoggerFactory.getLogger(JobLogReportHelper.class);
    //创建单例对象
    private static JobLogReportHelper instance = new JobLogReportHelper();
    //把对象暴露出去
    public static JobLogReportHelper getInstance(){
        return instance;
    }
    //工作线程
    private Thread logrThread;
    //线程是否停止运行
    private volatile boolean toStop = false;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/1
     * @Description:启动该组件的方法
     */
    public void start(){
        logrThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //定义一个变量，用来记录上一次清理日志的时间
                long lastCleanLogTime = 0;
                while (!toStop) {
                    try {
                        //根据时间开始遍历，这里遍历三次，实际上对应的是三天
                        //分别为今天，昨天，前天，每次都遍历三天，收集这三天的日志信息
                        for (int i = 0; i < 3; i++) {
                            //得到当前时间日期
                            Calendar itemDay = Calendar.getInstance();
                            //这里就开始设置要获得具体时间了，如果是第一次循环，-i还是0，所以仍然是当前这一天
                            //如果是第二次，第三次循环，就是-1，-2，得到的就是昨天和前天了
                            itemDay.add(Calendar.DAY_OF_MONTH, -i);
                            //设置小时
                            itemDay.set(Calendar.HOUR_OF_DAY, 0);
                            //设置分钟
                            itemDay.set(Calendar.MINUTE, 0);
                            //设置秒
                            itemDay.set(Calendar.SECOND, 0);
                            //设置毫秒
                            itemDay.set(Calendar.MILLISECOND, 0);
                            //得到今天的零点时间
                            Date todayFrom = itemDay.getTime();
                            //接下来设置的是今天的结束时间
                            //设置23时
                            itemDay.set(Calendar.HOUR_OF_DAY, 23);
                            //设置分钟，到60分钟就是第二天的零时了，所以设置为59
                            itemDay.set(Calendar.MINUTE, 59);
                            //设置秒
                            itemDay.set(Calendar.SECOND, 59);
                            //设置毫秒
                            itemDay.set(Calendar.MILLISECOND, 999);
                            //得到这一天的截止日期，也就是24小时那个时刻
                            Date todayTo = itemDay.getTime();
                            //创建XxlJobLogReport对象，该对象就是用来封装收集到的日志信息的
                            XxlJobLogReport xxlJobLogReport = new XxlJobLogReport();
                            //先把该日志报告对应的哪一天设置进去，其他设置默认值0
                            xxlJobLogReport.setTriggerDay(todayFrom);
                            xxlJobLogReport.setRunningCount(0);
                            xxlJobLogReport.setSucCount(0);
                            xxlJobLogReport.setFailCount(0);
                            //从数据库中查询具体信息，findLogReport方法就是查询数据库的方法，该方法会返回一个Map
                            //有三组键值对，分别为triggerDayCount-value，triggerDayCountRunning-value，triggerDayCountSuc-value，
                            //其中为triggerDayCount这一天触发的定时任务的个数，triggerDayCountRunning为正在运行的定时任务的个数
                            //triggerDayCountSuc运行成功的定时任务个数
                            //最后还有一个triggerDayCountFail，为运行失败的定时任务的个数，这个并不是从数据库中查到的，而是返回Map后，让总个数减去成功个数和正在运行个数计算出来的
                            //对应的sql语句如下
                            //    <select id="findLogReport" resultType="java.util.Map" >
                            //		SELECT
                            //			COUNT(handle_code) triggerDayCount,
                            //			SUM(CASE WHEN (trigger_code in (0, 200) and handle_code = 0) then 1 else 0 end) as triggerDayCountRunning,
                            //			SUM(CASE WHEN handle_code = 200 then 1 else 0 end) as triggerDayCountSuc
                            //		FROM xxl_job_log
                            //		WHERE trigger_time BETWEEN #{from} and #{to}
                            //    </select>
                            Map<String, Object> triggerCountMap = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().findLogReport(todayFrom, todayTo);
                            if (triggerCountMap!=null && triggerCountMap.size()>0) {
                                int triggerDayCount = triggerCountMap.containsKey("triggerDayCount")?Integer.valueOf(String.valueOf(triggerCountMap.get("triggerDayCount"))):0;
                                int triggerDayCountRunning = triggerCountMap.containsKey("triggerDayCountRunning")?Integer.valueOf(String.valueOf(triggerCountMap.get("triggerDayCountRunning"))):0;
                                int triggerDayCountSuc = triggerCountMap.containsKey("triggerDayCountSuc")?Integer.valueOf(String.valueOf(triggerCountMap.get("triggerDayCountSuc"))):0;
                                int triggerDayCountFail = triggerDayCount - triggerDayCountRunning - triggerDayCountSuc;
                                //设置最新的信息
                                xxlJobLogReport.setRunningCount(triggerDayCountRunning);
                                xxlJobLogReport.setSucCount(triggerDayCountSuc);
                                xxlJobLogReport.setFailCount(triggerDayCountFail);
                            }
                            //更新数据库信息
                            int ret = XxlJobAdminConfig.getAdminConfig().getXxlJobLogReportDao().update(xxlJobLogReport);
                            if (ret < 1) {
                                //如果更新失败，则意味着数据库中还没有信息，是第一次收集这一天的信息，所以直接保存即可
                                XxlJobAdminConfig.getAdminConfig().getXxlJobLogReportDao().save(xxlJobLogReport);
                            }
                        }
                    } catch (Exception e) {
                        if (!toStop) {
                            logger.error(">>>>>>>>>>> xxl-job, job log report thread error:{}", e);
                        }
                    }
                    //接下来就该处理过期日志了，把过期的日志清除一下
                    //下面这个判断，首先判断用户是否设置了日志过期时间，所以getLogretentiondays()>0必须成立，过期日志时间默认为30天
                    //System.currentTimeMillis() - lastCleanLogTime > 24*60*60*1000这行代码就意味着距离上一次清除日志必须得过去24个小时
                    //也就是说，不管怎么样，我的这些日志最少要保留一天
                    if (XxlJobAdminConfig.getAdminConfig().getLogretentiondays()>0
                            && System.currentTimeMillis() - lastCleanLogTime > 24*60*60*1000) {
                        //得到当前时间
                        Calendar expiredDay = Calendar.getInstance();
                        //根据用户设置的日志过期时间，获得具体的时间，比如，用户设置的日志过期时间为10天，现在就得到了10天前的那个时间
                        expiredDay.add(Calendar.DAY_OF_MONTH, -1 * XxlJobAdminConfig.getAdminConfig().getLogretentiondays());
                        //把时间设置成零点
                        expiredDay.set(Calendar.HOUR_OF_DAY, 0);
                        expiredDay.set(Calendar.MINUTE, 0);
                        expiredDay.set(Calendar.SECOND, 0);
                        expiredDay.set(Calendar.MILLISECOND, 0);
                        //得到10天前的具体时间，然后就以这个时间点为标尺，清除该时间之前的所有日志
                        Date clearBeforeTime = expiredDay.getTime();
                        List<Long> logIds = null;
                        do {
                            //<select id="findClearLogIds" resultType="long" >
                            //		SELECT id FROM xxl_job_log
                            //		<trim prefix="WHERE" prefixOverrides="AND | OR" >
                            //			<if test="jobGroup gt 0">
                            //				AND job_group = #{jobGroup}
                            //			</if>
                            //			<if test="jobId gt 0">
                            //				AND job_id = #{jobId}
                            //			</if>
                            //			<if test="clearBeforeTime != null">
                            //				AND trigger_time <![CDATA[ <= ]]> #{clearBeforeTime}
                            //			</if>
                            //			<if test="clearBeforeNum gt 0">
                            //				AND id NOT in(
                            //				SELECT id FROM(
                            //				SELECT id FROM xxl_job_log AS t
                            //				<trim prefix="WHERE" prefixOverrides="AND | OR" >
                            //					<if test="jobGroup gt 0">
                            //						AND t.job_group = #{jobGroup}
                            //					</if>
                            //					<if test="jobId gt 0">
                            //						AND t.job_id = #{jobId}
                            //					</if>
                            //				</trim>
                            //				ORDER BY t.trigger_time desc
                            //				LIMIT 0, #{clearBeforeNum}
                            //				) t1
                            //				)
                            //			</if>
                            //		</trim>
                            //		order by id asc
                            //		LIMIT #{pagesize}
                            //	</select>
                            //以上就是findClearLogIds方法对应的sql语句，可以看到在方法内传入的几个0，其实在sql语句中都会判断条件是否成立。如果是0条件就不会成立
                            //所以最后查询的是小于这个时间的所有数据，也就是所有日志的id集合
                            logIds = XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().findClearLogIds(0, 0, clearBeforeTime, 0, 1000);
                            //判断集合是否为空
                            if (logIds!=null && logIds.size()>0) {
                                //在这里根据id真正清除数据库中的信息
                                XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().clearLog(logIds);
                            }
                            //循环判断，直到集合中没有数据了。循环第一次就会把数据清空了，第二次循环的时候会查询一下数据库，查不到数据，就会直接退出循环了
                        } while (logIds!=null && logIds.size()>0);
                        //更新上一次清除日志信息的时间
                        lastCleanLogTime = System.currentTimeMillis();
                    }
                    try {//干完活了让线程睡一分钟，说明是一分钟清理一次
                        TimeUnit.MINUTES.sleep(1);
                    } catch (Exception e) {
                        if (!toStop) {
                            logger.error(e.getMessage(), e);
                        }
                    }
                }
                logger.info(">>>>>>>>>>> xxl-job, job log report thread stop");
            }
        });
        logrThread.setDaemon(true);
        logrThread.setName("xxl-job, admin JobLogReportHelper");
        logrThread.start();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/1
     * @Description:停止该组件的方法
     */
    public void toStop(){
        toStop = true;
        logrThread.interrupt();
        try {
            logrThread.join();
        } catch (InterruptedException e) {
            logger.error(e.getMessage(), e);
        }
    }
}
```

这个组件的逻辑非常简单，可以分为两个功能模块，一个就是统计调度中心所有被调度的定时任务的执行情况，返回给前端展示，第二个功能就是清楚过期日志。具体的逻辑在注释中写得非常详细。各位，我就不再讲解了，大家知道哪个组件被引入了，具体的作用是什么，其实就可以直击去撸源码了，就是我给大家提供的代码。到现在，我给大家提供的代码其实就是加了详细注释的源码，直接去看就行了。这一章就到此为止了，我们下一章见。