其实不依赖SpringBoot的执行器之前已经讲过了，我记得简单讲解过一次。其实SpringBoot所做的就是把用户定义的每一个定时任务包装成bean对象，然后把这些对象创建出来交给容器来管理了。如果不使用SpringBoot，那就自己定义一个执行器，当执行器启动的时候，把用户定义的定时任务用对象包装起来不就好了？然后把这些对象放到一个容器中，比如就放到集合中。整体的逻辑十分简单，所以，接下来我就先为执行器引入一个新的类，就是

XxlJobSimpleExecutor类/

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/2
 * @Description:不依赖SpringBoot的执行器
 */
public class XxlJobSimpleExecutor extends XxlJobExecutor {
    private static final Logger logger = LoggerFactory.getLogger(XxlJobSimpleExecutor.class);

	//存放定时任务对象的集合
    private List<Object> xxlJobBeanList = new ArrayList<>();
    
    public List<Object> getXxlJobBeanList() {
        return xxlJobBeanList;
    }
    public void setXxlJobBeanList(List<Object> xxlJobBeanList) {
        this.xxlJobBeanList = xxlJobBeanList;
    }


    @Override
    public void start() {

        initJobHandlerMethodRepository(xxlJobBeanList);

        try {
            super.start();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    @Override
    public void destroy() {
        super.destroy();
    }


    private void initJobHandlerMethodRepository(List<Object> xxlJobBeanList) {
        if (xxlJobBeanList==null || xxlJobBeanList.size()==0) {
            return;
        }
        for (Object bean: xxlJobBeanList) {
            Method[] methods = bean.getClass().getDeclaredMethods();
            if (methods.length == 0) {
                continue;
            }
            for (Method executeMethod : methods) {
                XxlJob xxlJob = executeMethod.getAnnotation(XxlJob.class);
                registJobHandler(xxlJob, bean, executeMethod);
            }
        }
    }
}
```

在执行器启动的时候创建上面这个类的对象，然后调用该类的setXxlJobBeanList方法，就可以把用户创建的定时任务对象存放到该类的集合中。请看下面的代码块。

```
public class FrameLessXxlJobConfig {
    private static Logger logger = LoggerFactory.getLogger(FrameLessXxlJobConfig.class);


    private static FrameLessXxlJobConfig instance = new FrameLessXxlJobConfig();
    public static FrameLessXxlJobConfig getInstance() {
        return instance;
    }


    //定义一个XxlJobSimpleExecutor对象
    private XxlJobSimpleExecutor xxlJobExecutor = null;

    //在这个方法中把定时任务对象放到XxlJobSimpleExecutor内部的集合中
    public void initXxlJobExecutor() {

        // load executor prop
        Properties xxlJobProp = loadProperties("xxl-job-executor.properties");

        // init executor
        xxlJobExecutor = new XxlJobSimpleExecutor();
        xxlJobExecutor.setAdminAddresses(xxlJobProp.getProperty("xxl.job.admin.addresses"));
        xxlJobExecutor.setAccessToken(xxlJobProp.getProperty("xxl.job.accessToken"));
        xxlJobExecutor.setAppname(xxlJobProp.getProperty("xxl.job.executor.appname"));
        xxlJobExecutor.setAddress(xxlJobProp.getProperty("xxl.job.executor.address"));
        xxlJobExecutor.setIp(xxlJobProp.getProperty("xxl.job.executor.ip"));
        xxlJobExecutor.setPort(Integer.valueOf(xxlJobProp.getProperty("xxl.job.executor.port")));
        xxlJobExecutor.setLogPath(xxlJobProp.getProperty("xxl.job.executor.logpath"));
        xxlJobExecutor.setLogRetentionDays(Integer.valueOf(xxlJobProp.getProperty("xxl.job.executor.logretentiondays")));

        //new SampleXxlJob就是创建了一个定时任务对象，然后把这个对象设置到集合中了
        xxlJobExecutor.setXxlJobBeanList(Arrays.asList(new SampleXxlJob()));

        // start executor
        try {
            xxlJobExecutor.start();
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
        }
    }

    /**
     * destroy
     */
    public void destroyXxlJobExecutor() {
        if (xxlJobExecutor != null) {
            xxlJobExecutor.destroy();
        }
    }


    public static Properties loadProperties(String propertyFileName) {
        InputStreamReader in = null;
        try {
            ClassLoader loder = Thread.currentThread().getContextClassLoader();

            in = new InputStreamReader(loder.getResourceAsStream(propertyFileName), "UTF-8");;
            if (in != null) {
                Properties prop = new Properties();
                prop.load(in);
                return prop;
            }
        } catch (IOException e) {
            logger.error("load {} error!", propertyFileName);
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    logger.error("close {} error!", propertyFileName);
                }
            }
        }
        return null;
    }

}
```

在第七版本的代码中，有两个测试类模块，第一个测试类模块提供的就是不依赖SpringBoot的执行器。大家可以在哪个测试模块中进行测试。

接下来就是定时任务超时功能的实现，其实这个功能也没什么可说的，就是用户创建定时任务的时候，也许已经考虑到了任务执行比较耗时的情况，不管是什么原因吧，用户给要调度的定时任务设置了一个超时时间，比如说这个定时任务只能在3秒内执行完，超过3秒还没有执行完就算是超时了。这个超时时间是用户在web界面设定的，会被保存到XxlJobInfo对象中，并且存储到数据库中。定时任务调度的时候，这个超时时间会被封装到TriggerParam对象中发送给执行器这一端。而执行器这一端得到定时任务的超时时间后，就会采取相应的措施，请看下面的代码块。

```
public class JobThread extends Thread{

    //其他方法省略


    //在最核心的run方法中，会多出来一段代码
    @Override
	public void run() {
        /其他的逻辑省略
        
        //如果设置了超时时间，就要设置一个新的线程来执行定时任务
		if (triggerParam.getExecutorTimeout() > 0) {
			Thread futureThread = null;
			try {
				FutureTask<Boolean> futureTask = new FutureTask<Boolean>(new Callable<Boolean>() {
					@Override
					public Boolean call() throws Exception {
						//子线程可以访问父线程的本地变量
						XxlJobContext.setXxlJobContext(xxlJobContext);
						//在FutureTask中执行定时任务
						handler.execute();
						return true;
					}
				});
				//创建线程并且启动线程
				futureThread = new Thread(futureTask);
				futureThread.start();
				//最多等待用户设置的超时时间
				Boolean tempResult = futureTask.get(triggerParam.getExecutorTimeout(), TimeUnit.SECONDS);
			} catch (TimeoutException e) {
				XxlJobHelper.log("<br>----------- xxl-job job execute timeout");
				XxlJobHelper.log(e);
				//超时直接设置任务执行超时
				XxlJobHelper.handleTimeout("job execute timeout ");
			} finally {
				futureThread.interrupt();
			}
		}else {
			//没有设置超时时间，通过反射执行了定时任务，终于在这里执行了
			handler.execute();
		}
    }
    
}
```

通过创建一个FutureTask来执行定时任务，然后让一个新的线程来执行这个FutureTask。在超时时间之内没有获得执行结果，就意味着定时任务超时了。这时候程序就会走到catch块中，将定时任务的执行结果设置为失败。这就是定时任务超时的简单逻辑。

最后一个功能就是子任务的实现，这个也是在web界面设置的。如果一个服务器中部署了两个定时任务1和2，这两个定时任务其实都是没什么关系。但是这两个定时任务的信息会被调度中心记录在数据库中，每个定时任务都会对应一个唯一ID。如果在web界面，用户非要把2号任务设置成1号任务的子任务，就需要把2号任务的唯一ID填写到子任务ID处。这样这两个任务就具有关系了。![](https://cdn.nlark.com/yuque/0/2023/jpeg/26725125/1692105799260-d27f28a7-e8f0-42ca-83f8-8e1e80e79b81.jpeg)

而这种关系的具体表现就是，当1号任务执行成功后，调度中心接收到执行器发来的执行结果信息，这些信息会现在调度中心的JobCompleteHelper类中被处理。请看下面的代码块。

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
		log.setHandleCode(handleCallbackParam.getHandleCode());
		log.setHandleMsg(handleMsg.toString());
		//更新数据库中的日志信息
		XxlJobCompleter.updateHandleInfoAndFinish(log);
		return ReturnT.SUCCESS;
	}
}
```

从上面的代码块中可以看到，不管定时任务执行成功或者是失败，都需要XxlJobCompleter这个类来更新数据库的信息。而XxlJobCompleter类的具体内容如下。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/7/17
 * @Description:更新日志信息，触发子任务的类
 */
public class XxlJobCompleter {

    private static Logger logger = LoggerFactory.getLogger(XxlJobCompleter.class);


    public static int updateHandleInfoAndFinish(XxlJobLog xxlJobLog) {
        //触发子任务的方法
        finishJob(xxlJobLog);
        //判断字符串长度
        if (xxlJobLog.getHandleMsg().length() > 15000) {
            //太长的话需要截取一段
            xxlJobLog.setHandleMsg( xxlJobLog.getHandleMsg().substring(0, 15000) );
        }
        //更新数据库
        return XxlJobAdminConfig.getAdminConfig().getXxlJobLogDao().updateHandleInfo(xxlJobLog);
    }


   /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2023/7/17
    * @Description:触发子任务的方法
    */
    private static void finishJob(XxlJobLog xxlJobLog){
        String triggerChildMsg = null;
        //先判断定时任务是不是执行成功的状态
        if (XxlJobContext.HANDLE_CODE_SUCCESS == xxlJobLog.getHandleCode()) {
            //如果成功了，就先得到该定时任务的具体信息
            XxlJobInfo xxlJobInfo = XxlJobAdminConfig.getAdminConfig().getXxlJobInfoDao().loadById(xxlJobLog.getJobId());
            //判断子任务id不为null
            if (xxlJobInfo!=null && xxlJobInfo.getChildJobId()!=null && xxlJobInfo.getChildJobId().trim().length()>0) {
                triggerChildMsg = "<br><br><span style=\"color:#00c0ef;\" > >>>>>>>>>>>"+ I18nUtil.getString("jobconf_trigger_child_run") +"<<<<<<<<<<< </span><br>";
                //如果有多个子任务，就切分子任务id为数组
                String[] childJobIds = xxlJobInfo.getChildJobId().split(",");
                //遍历子任务id数组
                for (int i = 0; i < childJobIds.length; i++) {
                    //得到子任务id
                    int childJobId = (childJobIds[i]!=null && childJobIds[i].trim().length()>0 && isNumeric(childJobIds[i]))?Integer.valueOf(childJobIds[i]):-1;
                    if (childJobId > 0) {
                        //在这里直接调度子任务
                        JobTriggerPoolHelper.trigger(childJobId, TriggerTypeEnum.PARENT, -1, null, null, null);
                        //设置调度成功的结果
                        ReturnT<String> triggerChildResult = ReturnT.SUCCESS;
                        triggerChildMsg += MessageFormat.format(I18nUtil.getString("jobconf_callback_child_msg1"),
                                (i+1),
                                childJobIds.length,
                                childJobIds[i],
                                (triggerChildResult.getCode()==ReturnT.SUCCESS_CODE?I18nUtil.getString("system_success"):I18nUtil.getString("system_fail")),
                                triggerChildResult.getMsg());
                    } else {
                        triggerChildMsg += MessageFormat.format(I18nUtil.getString("jobconf_callback_child_msg2"),
                                (i+1),
                                childJobIds.length,
                                childJobIds[i]);
                    }
                }
            }
        }
        if (triggerChildMsg != null) {
            xxlJobLog.setHandleMsg( xxlJobLog.getHandleMsg() + triggerChildMsg );
        }
    }



    private static boolean isNumeric(String str){
        try {
            int result = Integer.valueOf(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

}
```

可以看到，在XxlJobCompleter类更新数据库的信息之前，还会执行finishJob(xxlJobLog);这个方法。在该方法中，就会根据子任务ID，也就是2号定时任务的ID，去调度2号定时任务。这样一来，就体现出一种关系，那就是父任务一旦执行完成，子任务就自动执行了。说白了，更像是一种回调函数。原理大同小异，逻辑简简单单，这就是一个完整的xxl-job定时任务调度框架。