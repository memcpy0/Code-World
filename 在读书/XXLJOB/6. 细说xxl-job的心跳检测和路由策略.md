朋友们，我用前面的五个章节，为大家详细剖析了一个分布式调度中心的演变过程和构建原理，这也就意味着 xxl-job 最核心的知识已经完全讲解完毕了。之后的章节，充其量只是在其基础上填充一下细节。比如说，这一节课，我就要为大家讲讲 xxl-job 中的心跳检测和路由策略的功能。但是在讲解之前，我有必要跟大家解释一下，因为剩下的知识都比较简单，而且比较零碎，没有一个整体的脉络，讲起来也是东一块西一块，上一章可能还在讲路由策略，下一章可能就讲到什么子任务被调度了。从知识上来说，剩下的内容都不是连贯的，而且非常简单，因此，我也就不再给大家列举例子，然后一步步迭代了，这时候再用之前的讲课方法是行不通的。因为这些知识本身就不连贯，即便我想迭代，也无能为力。所以，在接下来的几章，我将采用直接讲解源码的方法，为大家剖析剩下的知识点。这就要求大家在学习接下来的课程之前，务必看完前五章，并且对应的版本代码也都掌握了。这时候，再学习接下来的知识，会发现特比容易就掌握了，甚至还会觉得我的文章写得啰嗦，直接就去看源码了。好了，就让我们直接进入本章的课程吧。

我之前就跟大家说过很多次，xxl-job其实就是个rpc，任务远程调度不就是rpc的体现吗？只不过xxl-job调度的是定时任务，需要在特定的时间才能调度定时任务。既然是个rpc，那么rpc的一些属性xxl-job也应该具备，心跳检测和路由策略就是首先要添加的功能。

说到心跳检测，其实也很简单，我们都知道，xxl-job的执行器是要把自身部署的服务器的信息发送给调度中心的，调度中心会把这些信息记录到数据库，当然也会把这些执行器的信息收集起来，把appName相同的执行器，封装到同一个Group中。我想，这一点对大家来说应该已经很熟悉了。就是下面这个代码块中呈现的代码。

```
//执行器组的实体类，实际上就是把执行相同定时任务的执行器用这个对象来封装
//因为执行相同定时任务的执行器除了服务实例地址不同，其他的都相同，所以可以放到一起，服务实例地址用逗号隔开即可
//该实体类对应的就是数据库中的xxl-job-group这张表
public class XxlJobGroup {

    private int id;
    //执行器中配置的项目名称
    private String appname;
    //中文名称
    private String title;
    //执行器的注册方法，0为自动注册，1为手动注册
    //这里其实很容易理解，web界面是可以手动录入执行器地址的，同时启动执行器
    //执行器也会自动将自己注册到调度中心的服务器中
    private int addressType;
    //执行器的地址，地址为IP+Port，不同的地址用逗号分开
    private String addressList;
    //更新时间
    private Date updateTime;
    //这里实际上就是把addressList属性中的多个地址转变成list集合了，
    //集合中存储的就是注册的所有执行器地址
    private List<String> registryList;

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/4
     * @Description:该方法会讲addressList属性中的所有地址转变成list集合
     */
    public List<String> getRegistryList() {
        if (addressList!=null && addressList.trim().length()>0) {
            registryList = new ArrayList<String>(Arrays.asList(addressList.split(",")));
        }
        return registryList;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getAppname() {
        return appname;
    }

    public void setAppname(String appname) {
        this.appname = appname;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getAddressType() {
        return addressType;
    }

    public void setAddressType(int addressType) {
        this.addressType = addressType;
    }

    public String getAddressList() {
        return addressList;
    }

    public Date getUpdateTime() {
        return updateTime;
    }

    public void setUpdateTime(Date updateTime) {
        this.updateTime = updateTime;
    }

    public void setAddressList(String addressList) {
        this.addressList = addressList;
    }

}
```

只要是appName相同的执行器的服务器信息，都会被封装到同一个XxlJobGroup对象中，并且该对象中的registryList成员变量，存放的就是这些执行器部署的服务器的IP地址。这样一来，当调度中心将要调度一个定时任务的时候，就可以根据定时任务信息中的jobGroup这个id，查询出定时任务究竟属于哪一个XxlJobGroup。就像下面代码块展示的这样。

```
//定时任务的实体类，对应数据库中的xxl-job-info这张表
public class XxlJobInfo {
	//定时任务id
	private int id;
	//该定时任务所属的执行器的id
	private int jobGroup;

    //其他内容省略
}


//具体查询的方式
//根据jobGroup获取所有的执行器
XxlJobGroup group = XxlJobAdminConfig.getAdminConfig().getXxlJobGroupDao().load(jobInfo.getJobGroup());
```

当得到了上面代码块的group对象后，就可以进一步获得该对象内部的所有执行器的地址，就像下面这样。

```
//得到所有注册到服务端的执行器的地址，并且做判空处理
List<String> registryList = group.getRegistryList();
```

然后，调度中心就可以根据具体的路由策略来选择哪一个执行器的服务地址，然后远程调度该服务器去执行定时任务。这就是前五章远程调度的过程中缺失的步骤，现在我给大家补充完整了。但是，问题也随着暴露了。调度中心会接收到所有执行器的信息，然后把执行器的信息记录到数据库。如果仅仅是这样，调度中心接收到执行器发送过来的信息后，就再也不和执行器维持联系了，那么，当调度中心远程调度任务的时候，根据路由策略选取了一个具体的服务器地址，要去远程调用了，但这个时候，地址对应的服务器已经停止运行了，那该怎么办呢？很显然，定时任务就无法调度成功了。而修复这一问题的方法也很简单，那就是让执行器事先知道，那个执行器掉线了，然后把掉线的执行器从XxlJobGroup对象中的registryList集合中删掉就行了。这样，registryList集合保存的永远都是在线的执行器了，不管用什么样的路由策略选取执行器的IP地址，总能保证这个地址是可用的。所以思路已经明确了，那就是让调度中心和执行器一直保持联系即可，就比如说，当执行器把自己部署的服务器的信息发送给调度中心后，每隔30秒就重复一次之前的动作。而在调度中心这一边，如果超过90秒没收到执行器发送过来的信息了，也就是数据库中对应的执行器地址信息超过90秒没有更新了，就把该地址删除掉。为什么是90秒呢？因为要给执行器一些机会，万一因为网络问题，丢失了一次执行器发送的注册信息，但是执行器明明还在工作，这不就把执行器地址误删了吗？总之，按照这个方向来编写代码，什么问题就都解决了。当然，代码也早就准备好了，请看下面的代码块。

首先是执行器一端的代码。

```
public class ExecutorRegistryThread {

    private static Logger logger = LoggerFactory.getLogger(ExecutorRegistryThread.class);

    //创建该类的对象
    private static ExecutorRegistryThread instance = new ExecutorRegistryThread();
    //通过该方法将该类对象暴露出去
    public static ExecutorRegistryThread getInstance(){
        return instance;
    }
    //将执行器注册到调度中心的线程，也就是真正干活的线程
    private Thread registryThread;
    //线程是否停止工作的标记
    private volatile boolean toStop = false;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/7/8
     * @Description:启动注册线程
     */
    public void start(final String appname, final String address){
        //对appname判空，这个就是执行器要记录在调度衷心的名称
        if (appname==null || appname.trim().length()==0) {
            logger.warn(">>>>>>>>>>> xxl-job, executor registry config fail, appname is null.");
            return;
        }
        //判断adminBizList集合不为空，因为个客户端是用来和调度中心通信的
        if (XxlJobExecutor.getAdminBizList() == null) {
            logger.warn(">>>>>>>>>>> xxl-job, executor registry config fail, adminAddresses is null.");
            return;
        }
        //创建线程
        registryThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //在一个循环中执行注册任务
                while (!toStop) {
                    try {
                        //根据appname和address创建注册参数，注意，这里的address是执行器的地址，只有一个，别和调度中心的地址搞混了
                        RegistryParam registryParam = new RegistryParam(RegistryConfig.RegistType.EXECUTOR.name(), appname, address);
                        //这里考虑到调度中心也许是以集群的形式存在，所以从集合中得到每一个和调度中心通话地客户端，然后发送注册消息即可
                        for (AdminBiz adminBiz: XxlJobExecutor.getAdminBizList()) {
                            try {
                                //在这里执行注册
                                ReturnT<String> registryResult = adminBiz.registry(registryParam);
                                if (registryResult!=null && ReturnT.SUCCESS_CODE == registryResult.getCode()) {
                                    registryResult = ReturnT.SUCCESS;
                                    logger.debug(">>>>>>>>>>> xxl-job registry success, registryParam:{}, registryResult:{}", new Object[]{registryParam, registryResult});
                                    //注册成功则打破循环，因为注册成功一个后，调度中心就把相应的数据写到数据库中了，没必要每个都注册
                                    //直接退出循环即可
                                    //注册不成功，再找下一个注册中心继续注册
                                    break;
                                } else {
                                    //如果注册失败了，就寻找下一个调度中心继续注册
                                    logger.info(">>>>>>>>>>> xxl-job registry fail, registryParam:{}, registryResult:{}", new Object[]{registryParam, registryResult});
                                }
                            } catch (Exception e) {
                                logger.info(">>>>>>>>>>> xxl-job registry error, registryParam:{}", registryParam, e);
                            }
                        }
                    } catch (Exception e) {
                        if (!toStop) {
                            logger.error(e.getMessage(), e);
                        }
                    }
                    try {
                        if (!toStop) {
                            //这里是每隔30秒，就再次循环重新注册一次，也就是维持心跳信息。
                            TimeUnit.SECONDS.sleep(RegistryConfig.BEAT_TIMEOUT);
                        }
                    } catch (InterruptedException e) {
                        if (!toStop) {
                            logger.warn(">>>>>>>>>>> xxl-job, executor registry thread interrupted, error msg:{}", e.getMessage());
                        }
                    }
                }
                try {
                    //这里要注意，当程序执行到这里的时候，就意味着跳出了上面那个工作线程的循环，其实也就意味着那个工作线程要结束工作了，不再注册执行器，也不再刷新心跳信息
                    //这也就意味着执行器这一端可能不再继续提供服务了，所以下面要把注册的执行器信息从调度中心删除，所以发送删除的信息给调度中心
                    //再次创建注册参数对象
                    RegistryParam registryParam = new RegistryParam(RegistryConfig.RegistType.EXECUTOR.name(), appname, address);
                    for (AdminBiz adminBiz: XxlJobExecutor.getAdminBizList()) {
                        try {
                            //在这里发送删除执行器的信息
                            ReturnT<String> registryResult = adminBiz.registryRemove(registryParam);
                            if (registryResult!=null && ReturnT.SUCCESS_CODE == registryResult.getCode()) {
                                registryResult = ReturnT.SUCCESS;
                                logger.info(">>>>>>>>>>> xxl-job registry-remove success, registryParam:{}, registryResult:{}", new Object[]{registryParam, registryResult});
                                break;
                            } else {
                                logger.info(">>>>>>>>>>> xxl-job registry-remove fail, registryParam:{}, registryResult:{}", new Object[]{registryParam, registryResult});
                            }
                        } catch (Exception e) {
                            if (!toStop) {
                                logger.info(">>>>>>>>>>> xxl-job registry-remove error, registryParam:{}", registryParam, e);
                            }
                        }
                    }
                } catch (Exception e) {
                    if (!toStop) {
                        logger.error(e.getMessage(), e);
                    }
                }
                logger.info(">>>>>>>>>>> xxl-job, executor registry thread destroy.");

            }
        });
        //在这里启动线程
        registryThread.setDaemon(true);
        registryThread.setName("xxl-job, executor ExecutorRegistryThread");
        registryThread.start();
    }
}
```

在上面的代码块中，可以看到这样一行非常重要的代码。

```
while(!toStop){

//其他的内容统统省略
    
//这里是每隔30秒，就再次循环重新注册一次，也就是维持心跳信息。
TimeUnit.SECONDS.sleep(RegistryConfig.BEAT_TIMEOUT);  


}

RegistryConfig.BEAT_TIMEOUT = 30;
```

在上面的这个循环中，实际上是每隔30秒才循环一次，实际上就是每30秒，就发送一次本服务器的信息给调度中心。这就是执行器这一端最简单的维持心跳的方法。

接下来就是调度中心这一端，更具执行器定期发送过来的执行器的信息，所做的处理。请看下面代码。

```
public class JobRegistryHelper {

	private static Logger logger = LoggerFactory.getLogger(JobRegistryHelper.class);
	//在这里创建本类对象
	private static JobRegistryHelper instance = new JobRegistryHelper();
	//通过该方法把本类对象暴露出去
	public static JobRegistryHelper getInstance(){
		return instance;
	}


	//这个线程池就是用来注册或者移除执行器地址的
	private ThreadPoolExecutor registryOrRemoveThreadPool = null;

	//该线程的作用就是检测注册中心过期的执行器的
	private Thread registryMonitorThread;


	private volatile boolean toStop = false;

	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/4
	 * @Description:创建并启动上面的线程池
	 */
	public void start(){
		//执行注册和移除执行器地址任务的线程池在这里被创建了
		registryOrRemoveThreadPool = new ThreadPoolExecutor(
				2,
				10,
				30L,
				TimeUnit.SECONDS,
				new LinkedBlockingQueue<Runnable>(2000),
				new ThreadFactory() {
					@Override
					public Thread newThread(Runnable r) {
						return new Thread(r, "xxl-job, admin JobRegistryMonitorHelper-registryOrRemoveThreadPool-" + r.hashCode());
					}
				},
				//下面这个是xxl-job定义的线程池拒绝策略，其实就是把被拒绝的任务再执行一遍
				new RejectedExecutionHandler() {
					@Override
					public void rejectedExecution(Runnable r, ThreadPoolExecutor executor) {
						//在这里能看到，所谓的拒绝，就是把任务再执行一遍
						r.run();
						logger.warn(">>>>>>>>>>> xxl-job, registry or remove too fast, match threadpool rejected handler(run now).");
					}
				});

		//该线程就是用来循环检测注册中心注册的执行器是否过期，如果过期就移除过期数据，说白了，就是起到一个
		//心跳检测的作用，该线程每次循环都会睡30秒，其实就是30秒检测一次过期的执行器
		registryMonitorThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!toStop) {
					try {
						//这里查询的是所有自动注册的执行器组，手动录入的执行器不在次查询范围内，所谓自动注册，就是执行器启动时，通过http，把注册信息发送到
						//调度中心的注册方式，并不是用户在web界面手动录入的注册方式
						//注意，这里查询的是执行器组，还不是单个的执行器，也许现在大家还不明白是什么意思，往下看就会清楚了
						List<XxlJobGroup> groupList = XxlJobAdminConfig.getAdminConfig().getXxlJobGroupDao().findByAddressType(0);
						if (groupList!=null && !groupList.isEmpty()) {
							//这里的逻辑其实还要去对应的Mapper中查看，这里我把Mapper中的sql语句截取关键部分贴出来
							//WHERE t.update_time <![CDATA[ < ]]> DATE_ADD(#{nowTime},INTERVAL -#{timeout} SECOND)
							//其实就是判断数据库中记录的所有执行器的最新一次的更新时间是否小于当前时间减去90秒，这就意味着执行器的超时时间
							//就是90秒，只要90秒内，执行器没有再更新自己的信息，就意味着它停机了
							//而在执行器那一端，是每30秒就重新注册一次到注册中心
							//注意，这里并没有区分是手动注册还是自动注册，只要是超时了的执行器都检测出来，然后从数据库中删除即可
							List<Integer> ids = XxlJobAdminConfig.getAdminConfig().getXxlJobRegistryDao().findDead(RegistryConfig.DEAD_TIMEOUT, new Date());
							if (ids!=null && ids.size()>0) {
								//上面的到了所有过期执行器的id集合，这里就直接删除过期的执行器
								XxlJobAdminConfig.getAdminConfig().getXxlJobRegistryDao().removeDead(ids);
							}
							//该Map是用来缓存appName和对应的执行器地址的
							HashMap<String, List<String>> appAddressMap = new HashMap<String, List<String>>();
							//这里查处的就是所有没有过期的执行器，同样不用考虑注册类型，是否自动注册或手动录入，对应的sql语句如下
							//WHERE t.update_time <![CDATA[ > ]]> DATE_ADD(#{nowTime},INTERVAL -#{timeout} SECOND)
							//就是把小于号改成了大于号而已
							List<XxlJobRegistry> list = XxlJobAdminConfig.getAdminConfig().getXxlJobRegistryDao().findAll(RegistryConfig.DEAD_TIMEOUT, new Date());
							if (list != null) {
								//走到这里说明数据库中存在没有超时的执行器数据
								for (XxlJobRegistry item: list) {
									//遍历这些执行器
									//先判断执行器是不是自动注册的，
									if (RegistryConfig.RegistType.EXECUTOR.name().equals(item.getRegistryGroup())) {
										//如果是自动注册，就先获得执行器的项目名称，就是那个appName
										String appname = item.getRegistryKey();
										//以appName为key，判断刚才的Map中是否缓存着该appName对应的执行器地址
										List<String> registryList = appAddressMap.get(appname);
										if (registryList == null) {
											//如果没有则创建一个集合
											//这里之所以是集合，还是考虑到定时任务很可能部署在多个执行器上，而相同定时任务
											//的执行器名称是相同，正好可以用来当作key，value就是不同的执行器地址
											registryList = new ArrayList<String>();
										}
										//如果创建的这个集合尚未包含当前循环的执行器的地址
										if (!registryList.contains(item.getRegistryValue())) {
											//就把该地址存放到集合中
											registryList.add(item.getRegistryValue());
										}
										//把集合添加到Map中，至此，一个appName对应的执行器地址，这样的数据就通过键值对缓存成功了
										appAddressMap.put(appname, registryList);
									}
								}
							}
							//到这里会遍历最开始查询出来的自动注册的所有执行器组，注意，这时候，在上面的那个循环中，已经把
							//所有未过期的执行器的信息用键值对的方式缓存在Map中了
							for (XxlJobGroup group: groupList) {
								//根据这个执行器注册到注册中心时记录的appName
								//从Map中查询到所有的执行器地址，是个集合
								List<String> registryList = appAddressMap.get(group.getAppname());
								String addressListStr = null;
								//判空操作
								if (registryList!=null && !registryList.isEmpty()) {
									//如果该执行器地址集合不为空，就把地址排一下序
									//这里排序有什么意义呢？我没想出来。。排不排都无所谓吧，反正会有路由策略帮我们选择执行器地址
									Collections.sort(registryList);
									//开始把这些地址拼接到一块
									StringBuilder addressListSB = new StringBuilder();
									for (String item:registryList) {
										addressListSB.append(item).append(",");
									}
									addressListStr = addressListSB.toString();
									//去掉最后一个逗号
									addressListStr = addressListStr.substring(0, addressListStr.length()-1);
								}
								//然后把最新的执行器地址存放到执行器组中
								group.setAddressList(addressListStr);
								//更新执行器组的更新时间
								group.setUpdateTime(new Date());
								//在数据库中落实执行器组的更新
								//到这里，大家应该能意识到了，执行器把自己注册到调度中心，是通过XxlJobRegistry对象
								//来封装注册信息的，会被记录到数据库中
								//但是注册线程会在后台默默工作，把各个appName相同的执行器的地址整合到一起，用XxlJobGroup对象封装
								//等待调度定时任务的时候，其实就是从XxlJobGroup对象中获得appName的所有执行器地址，然后根据路由策略去
								//选择具体的执行器地址来远程调用，这就是和注册有关的所有逻辑了，到此，该类中的代码和源码也完全一致了
								XxlJobAdminConfig.getAdminConfig().getXxlJobGroupDao().update(group);
							}
						}
					} catch (Exception e) {
						if (!toStop) {
							logger.error(">>>>>>>>>>> xxl-job, job registry monitor thread error:{}", e);
						}
					}
					try {
						//线程在这里睡30秒，也就意味着检测周期为30秒
						TimeUnit.SECONDS.sleep(RegistryConfig.BEAT_TIMEOUT);
					} catch (InterruptedException e) {
						if (!toStop) {
							logger.error(">>>>>>>>>>> xxl-job, job registry monitor thread error:{}", e);
						}
					}
				}
				logger.info(">>>>>>>>>>> xxl-job, job registry monitor thread stop");
			}
		});
		registryMonitorThread.setDaemon(true);
		registryMonitorThread.setName("xxl-job, admin JobRegistryMonitorHelper-registryMonitorThread");
		registryMonitorThread.start();
	}


    	/**
	 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyang。
	 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
	 * @Date:2023/7/4
	 * @Description:注册执行器的方法，就是这个方法，会一直更新数据库中执行器的信息
	 */
	public ReturnT<String> registry(RegistryParam registryParam) {
		//校验处理
		if (!StringUtils.hasText(registryParam.getRegistryGroup())
				|| !StringUtils.hasText(registryParam.getRegistryKey())
				|| !StringUtils.hasText(registryParam.getRegistryValue())) {
			return new ReturnT<String>(ReturnT.FAIL_CODE, "Illegal Argument.");
		}
		//提交注册执行器的任务给线程池执行
		registryOrRemoveThreadPool.execute(new Runnable() {
			@Override
			public void run() {
				//这里的意思也很简单，就是先根据registryParam参数去数据库中更新相应的数据
				//如果返回的是0，说明数据库中没有相应的信息，该执行器还没注册到注册中心呢，所以下面
				//就可以直接新增这一条数据即可
				int ret = XxlJobAdminConfig.getAdminConfig().getXxlJobRegistryDao().registryUpdate(registryParam.getRegistryGroup(), registryParam.getRegistryKey(), registryParam.getRegistryValue(), new Date());
				if (ret < 1) {
					//这里就是数据库中没有相应数据，直接新增即可
					XxlJobAdminConfig.getAdminConfig().getXxlJobRegistryDao().registrySave(registryParam.getRegistryGroup(), registryParam.getRegistryKey(), registryParam.getRegistryValue(), new Date());
					//该方法从名字上看是刷新注册表信息的意思
					//但是作者还没有实现，源码中就是空的，所以这里我就照搬过来了
					freshGroupRegistryInfo(registryParam);
				}
			}
		});
		return ReturnT.SUCCESS;
	}

    
}
```

可以看到，在上面的代码块中，是registryMonitorThread线程在执行更新执行器地址信息的功能。具体的逻辑都写在代码块中了，所以我也就不再详细讲解了。其实看到这里大家就可以直接去看源码了，因为我已经把新的功能添加在哪个类，哪个方法中已经展示得很清楚了。当然，也许有些朋友会觉得我不再剖析文章，而是直接展示代码，是在敷衍大家。我有必要为我自己解释一句，现在所讲解的知识真的特别简单，而且没什么连贯性，无法再用之前的那种方式讲解了。这时候直接展示代码，比那种慢吞吞得剖析实现思路的讲解方式反而高效得多。如果我真的想要敷衍大家，就不会专门在文章里写这么多字了。直接迭代下一门的代码或者录视频不好吗🐶？好了，维持心跳的功能已经讲解完了，下面，就为大家讲解一下xxl-job中的几种路由策略。

当然，在讲解之前，有必要为大家解释一下，在第一版本的手写代码中，是没有用到路由策略的，而是直接把选取的执行器服务器的地址写死了。请看下面的代码块。

```
   private static void processTrigger(XxlJobGroup group, XxlJobInfo jobInfo, int finalFailRetryCount, TriggerTypeEnum triggerType, int index, int total){
        //初始化触发器参数，这里的这个触发器参数，是要在远程调用的另一端，也就是执行器那一端使用的
        TriggerParam triggerParam = new TriggerParam();
        //设置任务id
        triggerParam.setJobId(jobInfo.getId());
        //设置执行器要执行的任务的方法名称
        triggerParam.setExecutorHandler(jobInfo.getExecutorHandler());
        //把执行器要执行的任务的参数设置进去
        triggerParam.setExecutorParams(jobInfo.getExecutorParam());
        //设置执行模式，一般都是bean模式
        triggerParam.setGlueType(jobInfo.getGlueType());
        //接下来要再次设定远程调用的服务实例的地址
        //这里其实是考虑到了路由策略，但是第一版本还不涉及这些知识，所以就先不这么做了
        String address = null;
        //得到所有注册到服务端的执行器的地址，并且做判空处理
        List<String> registryList = group.getRegistryList();
        if (registryList!=null && !registryList.isEmpty()) {
            //在源码中，本来这里就要使用路由策略，选择具体的执行器地址了，但是现在我们还没有引入路由策略
            //所以这里就简单处理，就使用集合中的第一个地址
            address = registryList.get(0);
        }
        //接下来就定义一个远程调用的结果变量
        ReturnT<String> triggerResult = null;
        //如果地址不为空
        if (address != null) {
            //在这里进行远程调用，这里就是最核心远程调用的方法，但是方法内部的逻辑很简单，就是用http发送调用
            //消息而已
            triggerResult = runExecutor(triggerParam, address);
            //这里就输出一下状态码吧，根据返回的状态码判断任务是否执行成功
            logger.info("返回的状态码"+triggerResult.getCode());
        } else {
            triggerResult = new ReturnT<String>(ReturnT.FAIL_CODE, null);
        }
    }
```

在XxlJobTrigger类的processTrigger方法中，也就是代码块的第16行到21行之间的代码，可以看到从group中获得了所有的执行器地址信息的集合后，默认选取了集合的首位地址进行远程调度。这是我写死的。那么，当我们现在引入调度策略后，代码该怎么编写呢？就比如说，我就想定义一个永远选取首位地址的路由策略，那该怎么做呢？路由策略有很多种不同的实现，但是使用的规则肯定是统一的，既然是这样，肯定要首先引入一个路由的接口，然后用这个接口做具体的实现，当然，也应该定义一个路由的枚举类，这样就可以判断当前使用的是哪种路由策略了。请看下面的代码块。

首先是路由策略的接口。

```
public abstract class ExecutorRouter {
    protected static Logger logger = LoggerFactory.getLogger(ExecutorRouter.class);
	//选取路由策略的具体方法
    public abstract ReturnT<String> route(TriggerParam triggerParam, List<String> addressList);

}
```

接着是永远选取首位地址的路由策略，也就是上面接口的一个实现类。

```
//路由策略之一，选择集合中的第一个地址使用
public class ExecutorRouteFirst extends ExecutorRouter {

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList){
        return new ReturnT<String>(addressList.get(0));
    }

}
```

最后是一个包含所有路由策略的枚举类。

```
public enum ExecutorRouteStrategyEnum {
	//使用第一个地址
    FIRST(I18nUtil.getString("jobconf_route_first"), new ExecutorRouteFirst()),
    //使用最后一个
    LAST(I18nUtil.getString("jobconf_route_last"), new ExecutorRouteLast()),
    //轮训
    ROUND(I18nUtil.getString("jobconf_route_round"), new ExecutorRouteRound()),
    //随机
    RANDOM(I18nUtil.getString("jobconf_route_random"), new ExecutorRouteRandom()),
    //一致性哈希
    CONSISTENT_HASH(I18nUtil.getString("jobconf_route_consistenthash"), new ExecutorRouteConsistentHash()),
    //最不经常使用
    LEAST_FREQUENTLY_USED(I18nUtil.getString("jobconf_route_lfu"), new ExecutorRouteLFU()),
    //最近最久未使用
    LEAST_RECENTLY_USED(I18nUtil.getString("jobconf_route_lru"), new ExecutorRouteLRU()),
    //故障转移
    FAILOVER(I18nUtil.getString("jobconf_route_failover"), new ExecutorRouteFailover()),
    //忙碌转移
    BUSYOVER(I18nUtil.getString("jobconf_route_busyover"), new ExecutorRouteBusyover()),
    //分片广播
    SHARDING_BROADCAST(I18nUtil.getString("jobconf_route_shard"), null);
    ExecutorRouteStrategyEnum(String title, ExecutorRouter router) {
        this.title = title;
        this.router = router;
    }

    private String title;
    private ExecutorRouter router;

    public String getTitle() {
        return title;
    }
    public ExecutorRouter getRouter() {
        return router;
    }

    public static ExecutorRouteStrategyEnum match(String name, ExecutorRouteStrategyEnum defaultItem){
        if (name != null) {
            for (ExecutorRouteStrategyEnum item: ExecutorRouteStrategyEnum.values()) {
                if (item.name().equals(name)) {
                    return item;
                }
            }
        }
        return defaultItem;
    }

}
```

而引入了路由策略之后，原有的代码该怎么改动呢？请看下面的代码块。

```
private static void processTrigger(XxlJobGroup group, XxlJobInfo jobInfo, int finalFailRetryCount, TriggerTypeEnum triggerType, int index, int total){
        //得到当前要调度的执行任务的路由策略，默认是没有
        ExecutorRouteStrategyEnum executorRouteStrategyEnum = ExecutorRouteStrategyEnum.match(jobInfo.getExecutorRouteStrategy(), null);
        //初始化触发器参数，这里的这个触发器参数，是要在远程调用的另一端，也就是执行器那一端使用的
        TriggerParam triggerParam = new TriggerParam();
        //设置任务id
        triggerParam.setJobId(jobInfo.getId());
        //设置执行器要执行的任务的方法名称
        triggerParam.setExecutorHandler(jobInfo.getExecutorHandler());
        //把执行器要执行的任务的参数设置进去
        triggerParam.setExecutorParams(jobInfo.getExecutorParam());
        //定时任务的路由策略设置进去
        triggerParam.setExecutorBlockStrategy(jobInfo.getExecutorBlockStrategy());
        //设置执行模式，一般都是bean模式
        triggerParam.setGlueType(jobInfo.getGlueType());
        //接下来要再次设定远程调用的服务实例的地址
        //这里其实是考虑到了路由策略
        String address = null;
        ReturnT<String> routeAddressResult = null;
        //得到所有注册到服务端的执行器的地址，并且做判空处理
        List<String> registryList = group.getRegistryList();
        if (registryList!=null && !registryList.isEmpty()) {
            //在这里根据路由策略获得最终选用的执行器地址
            routeAddressResult = executorRouteStrategyEnum.getRouter().route(triggerParam, registryList);
            if (routeAddressResult.getCode() == ReturnT.SUCCESS_CODE) {
                address = routeAddressResult.getContent();
            } else {
                //如果没得到地址，就赋值失败，这里还用不到这个失败结果，但是先列出来吧
                routeAddressResult = new ReturnT<String>(ReturnT.FAIL_CODE, I18nUtil.getString("jobconf_trigger_address_empty"));
            }
        }
        //接下来就定义一个远程调用的结果变量
        ReturnT<String> triggerResult = null;
        //如果地址不为空
        if (address != null) {
            //在这里进行远程调用，这里就是最核心远程调用的方法，但是方法内部的逻辑很简单，就是用http发送调用
            //消息而已
            triggerResult = runExecutor(triggerParam, address);
            //这里就输出一下状态码吧，根据返回的状态码判断任务是否执行成功
            logger.info("返回的状态码"+triggerResult.getCode());
        } else {
            triggerResult = new ReturnT<String>(ReturnT.FAIL_CODE, null);
        }
    }
```

可以看到，在上看代码块的第三行，就得到了用户设置的定时任务的路由策略，当然，这个路由策略是用户在web界面添加定时任务的时候就设置好的，其信息封装在了XxlJobInfo对象中。而得到了具体的路由地址后，在上面代码块的第24行，得到了最终选取的执行器的IP地址。请看下面这行代码。

```
 //在这里根据路由策略获得最终选用的执行器地址
routeAddressResult = executorRouteStrategyEnum.getRouter().route(triggerParam, registryList);
```

因为之前已经根据用户定义的路由策略的名称得到具体的路由策略的枚举类，所以这里就可以直接从枚举类中取出具体的路由策略的实现类了，然后调用实现类的route方法，得到具体的执行器的IP地址。在这里，我们使用的是选取第一个地址的路由策略。就是我刚才定义好的那个，请看下面的代码块。

```
//路由策略之一，选择集合中的第一个地址使用
public class ExecutorRouteFirst extends ExecutorRouter {

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList){
        //在这里，直接返回了addressList集合的0号位置的地址
        return new ReturnT<String>(addressList.get(0));
    }

}
```

经过上面的一系列的流程的展示，到此为止，大家应该也明白了路哟策略的具体执行的过程。这点知识是非常简单的。好了，刚才我使用了选取首位地址的路由策略，如果我想使用选取最后一位地址的路由策略呢？请看下面的代码块。

```
public class ExecutorRouteLast extends ExecutorRouter {

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList) {
        //在这里把最后一位地址返回
        return new ReturnT<String>(addressList.get(addressList.size()-1));
    }

}
```

上面就是使用最后一个地址的路由策略的展示。那接下来，我想使用随机选取执行器地址的路由策略呢？请看下面代码块。

```
public class ExecutorRouteRandom extends ExecutorRouter {

    private static Random localRandom = new Random();

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList) {
        //从addressList集合中随机选取一个地址
        String address = addressList.get(localRandom.nextInt(addressList.size()));
        return new ReturnT<String>(address);
    }

}
```

如果想使用轮询的策略呢？这个就要仔细想一想了。首先我们要知道，一个定时任务是对应着多个IP地址的，而这多个IP地址存放在一个集合中，实际上就是一个数组中。既然要使用轮询，最简单的方法肯定是用一个整数和数组的长度取余，每取一次余，这个数就自增一，这样不就达到轮询的效果了吗？如果是这样，完全可以创建一个Map，然后将每个定时任务的ID当作key，其对应的value就是那个需要一直自增的整数，这样就完事大吉了。请看下面的代码块。

```
public class  ExecutorRouteRound extends ExecutorRouter {
    //该Map的key为定时任务的id，value为一个次数，用于和地址集合的长度取余
    private static ConcurrentMap<Integer, AtomicInteger> routeCountEachJob = new ConcurrentHashMap<>();
    //Map中数据的缓存时间
    private static long CACHE_VALID_TIME = 0;

    private static int count(int jobId) {
        //判断当前时间是否大于Map的缓存时间
        if (System.currentTimeMillis() > CACHE_VALID_TIME) {
            //如果大于，则意味着数据过期了，清除即可
            routeCountEachJob.clear();
            //重新设置数据缓存有效期
            CACHE_VALID_TIME = System.currentTimeMillis() + 1000*60*60*24;
        }
        //根据定时任务id从Map中取出对应的次数
        AtomicInteger count = routeCountEachJob.get(jobId);
        //如果是第一次执行这个定时任务，那么Map中肯定没有缓存着对应的定时任务id，也没有value
        //这里对应的做法就是初始化一个value，从0-100之间选择一个随机数，赋值给value
        if (count == null || count.get() > 1000000) {
            //这里其实还有一个操作，就是如果value对应的值已经大于1000000，也执行下面的这个初始化
            //重新给count赋值
            //但是这里为什么会有这个操作呢？为什么当定时任务第一次执行的时候，要弄一个随机数来取余呢？
            //这是因为每一个定时任务第一次执行的时候，如果不弄一个随机数做取余运算，那这些定时任务选择的执行器一定都是
            //相同的，如果第一时间有很多定时任务第一次执行，就会在同一个执行器上执行，可能会给执行器造成很大的压力
            //明明执行器也搭建了集群，却没有集群的效果，所以这里弄一个随机数，把第一次执行的定时任务分散到各个执行器上来执行
            //缓解执行器的压力
            //这样后面再执行相同的任务，调度的执行器也都分散了，压力会小很多的
            count = new AtomicInteger(new Random().nextInt(100));
        } else {
            //如果count有值，说明不是第一次执行这个定时任务了
            //在这里加1
            count.addAndGet(1);
        }
        //把value更新到Map中
        routeCountEachJob.put(jobId, count);
        //得到count的值
        return count.get();
    }

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList) {
        //这里就是简单的取余操作，在routeCountEachJob中会记录着定时任务对应的一个数值，这个数值会和执行器集合
        //的长度做%运算，得到要使用的执行器地址。而且定时任务每调度一次，它在routeCountEachJob中对应的
        //value就会加1，然后再和集合长度取余，这样就达到了轮训地址的效果。
        String address = addressList.get(count(triggerParam.getJobId())%addressList.size());
        return new ReturnT<String>(address);
    }

}
```

上面代码块中的内容虽然比较多，但是逻辑相当简单，代码中注释添加的十分详细，大家简单看看就行。到此为止，我已经为大家介绍了四中路由策略了。剩下的还有几种策略，比如一致性哈希策略，最不经常使用策略，最近最久未使用策略，故障转移路由策略。这里我就不一一介绍了，这几种路由策略在我提供的第二版本代码中，每一行代码都有非常详细的注释，而且逻辑特别简单。实际上，xxl-job这个框架的理念就是用简单的代码实现全面的功能，想想它的时间轮，难道不觉得简单吗？剩下的几种策略，就留给大家自己去研究吧。

当然，唯一值得提一句的是，所谓的故障转移，在xxl-job中其实就是在每次调度前，向执行器发送心跳消息，如果消息发送失败，就意味着这个执行器不能使用，那就紧接着给下一个执行器发送心跳消息，一旦返回成功消息，就直接使用该执行器地址，不会再继续向其他的执行器发送心跳消息了。具体的代码如下。

```
public class ExecutorRouteFailover extends ExecutorRouter {

    @Override
    public ReturnT<String> route(TriggerParam triggerParam, List<String> addressList) {
        StringBuffer beatResultSB = new StringBuffer();
        //遍历得到的执行器地址
        for (String address : addressList) {
            ReturnT<String> beatResult = null;
            try {
                //得到访问执行器的客户端
                ExecutorBiz executorBiz = XxlJobScheduler.getExecutorBiz(address);
                //向执行器发送心跳检测请求，看执行器是否还在线
                beatResult = executorBiz.beat();
            } catch (Exception e) {
                logger.error(e.getMessage(), e);
                beatResult = new ReturnT<String>(ReturnT.FAIL_CODE, ""+e );
            }
            beatResultSB.append( (beatResultSB.length()>0)?"<br><br>":"")
                    .append(I18nUtil.getString("jobconf_beat") + "：")
                    .append("<br>address：").append(address)
                    .append("<br>code：").append(beatResult.getCode())
                    .append("<br>msg：").append(beatResult.getMsg());
            //心跳检测没问题，就直接使用该执行器
            if (beatResult.getCode() == ReturnT.SUCCESS_CODE) {
                beatResult.setMsg(beatResultSB.toString());
                beatResult.setContent(address);
                //一旦成功，就直接退出循环了
                return beatResult;
            }
        }
        return new ReturnT<String>(ReturnT.FAIL_CODE, beatResultSB.toString());

    }
}
```

到此为止，本章的内容就结束了。大家也许会觉得，其实本章的内容特比少，我写了这么多难免有凑字数的嫌疑。因为就讲了一个心跳检测和路由策略，我简单讲解了一下，然后展示了一下代码。我相信肯定有朋友看了代码之后觉得根本不用讲解这种知识，直接去看源码就行了。其实我就是这么想的，但是为了不让大家觉得我是在敷衍，所以，我还是写了这么多字🐶。简单的知识就随便看看吧，早点看完，就可以去打打游戏，陪女朋友吃饭饭，逛逛街，看看电影，多爽啊。

最后再补充一点，xxl-job的故障转移是作为路由策略出现的，在这一章已经得到实现了。并且所有的路由策略都在调度中心的core包下的route包下。大家可以抽时间看看代码。