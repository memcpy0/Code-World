# 1. Policy 基础概念

Policy 是一个五元组，匹配特定的流量，并对匹配到的流量指定特定特定动作，Policy因此是流量管理规则。
- 五元组是源端口+IP、目的端口+IP、协议（但其实还有更多，源端站点，目的端站点（是 Site 或 Private Network）或 Internet）；
- 特定动作是：丢弃 或 以指定路径进行转发

Policy 可以**用于实现**用户的**所有**组网需求或Saas访问需求，只要创建特定的 Policy 。

Policy 根据访问互联网与否，一般可以分为两类：
- Site-to-Site Policy 主要用于**构建用户站点到站点的流量访问策略 ，一般在组网业务下使用；**
- Site-to-Internet Policy 用于**构建用户站点到互联网的流量访问策略**，一般用于Saas应用访问场景。

从产品上面看，目前Policy包括了：
- DNS规则（通过DNS消息下发）
- Policy规则（通过Policy消息下发）

DNS在PolicyZone体现为：-8 是通过模板创建的DNS
```cpp
public enum PolicyZone {  
  DNS(-10),  
  DNS_TEMPLATE_AUTO_GENERATE(-8),
  ...
```

Policy还可分类为：？？？
- Site Root Policy：在**多CPE场景**（建立 Spoke-Hub 拓扑时，Hub 端可以为多 CPE ）下，或者 **Site-to-Internet Policy 选择了Access List**（AccessUnit 的列表，一个 AccessUnit 代表多个 DomainSet 和 IPSet ）时，展示在 Dashboard 界面的 Policy 列表中，带有三角展开按钮的就是 Site Root Policy ，但并不实际下发给DP的 Policy 。
- Policy Rule ：实际下发给DP的 Policy 。**一般在 Policy 列表中一条 Policy 对应一条 Policy Rule** ，此时 Policy 的 isPolicyRule = true 且 parentId = null ？？ 。而一个 Site Root Policy 可以有多条 Policy Rule（展开后可查看）。不做特殊说明时，提到的 Policy 均为 Policy Rule 。

初始创建 Site 时，没有CPE，也就没有策略；添加一个CPE，就新增一个 INTERNET_... site_Default 策略：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308021653525.png)
此时该 Policy 就是 Site Root Policy ，也是 PolicyRule ，parentId = null ？？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308021654280.png)
之前添加的是「可以加多个CPE的CPE」，现在就只能加「能容忍多CPE的CPE设备」。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308021657036.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308021658621.png)


---
# 2. Policy 属性
```java
@Id private String id; // Policy ID（自动生成）
private Integer netId = 0; // 暂时不使用
private Integer customerId;  
```
安全域：选择策略生效的安全域，必须选择**源端和目的端共同拥有的安全域**，一般为公共域（Public Security Zone）。
```java
@Transient private String securityZoneName;  
private Long securityZoneId = SecurityZone.DEFAULT_LONG_ID;  
private String securityZoneUuid;  

/* 最小值 100 */
private Long policyId;  

private PolicyZone zone = PolicyZone.MANUAL;  
private Integer zoneNumber;  

private boolean customerBaseOrder = true; // policy排序是在customer级别排序还是cpe级别排序，默认未customer级别  
private Integer orderSequence = 0;  
```
policy的名称，系统默认自动生成策略名称，可进行自定义的设置。
```java
/*  
 * SaasInternet与Policy模块融合后，Policy分为两种：  
 * 1，用户在界面上配置的Policy，对应原SaasInternet以及Manual Policy  
 * 2，根据用户配置生成的Policy Rule，用于下发DP规则。一个Policy可以对应多个Policy Rule  
 * 原来的Policy模型即为现在的Policy Rule，通过parentId指定其父资源（Saas Policy的一对多关系）  
 * * 2.1版本中的Policy现分为以下几类：  
 * 1，原有的Manual Policy既是现在的Policy也是Policy Rule，没有parent，且isPolicyRule=true  
 * 2，原Topology创建的Connectivity Zone的default policy，没有parent，且isPolicyRule=true  
 * 3，原Default SaaS policy，没有parent，isPolicyRule=true  
 * 4，原SaasInternet对象变为现在的Policy(isPolicyRule=false），其下属的多条rule Policy的parentId为Saas Policy Id  
 * * UI界面上只展示parent=null的Policy，也即根Policy  
 * （Connectivity Zone以及Saas default policy的展示根据新需求决定是否展示到Policy界面）  
 */
private String parentId;  
@Transient private Long parentIntId;  
@Transient private String parentName;  
private boolean isPolicyRule = true;  
@Transient private List<Policy> policyRules;  
  
@Transient private PolicyType viewType; // 为了兼容3.5 Hybrid type与老版本前端逻辑，引入viewType，此字段仅供前端使用  
  
private boolean layer2Policy = false; // 标记仅能使用二层Connection的Policy  
```

**站点和设备**元组：
- 源站点：选择源端站点；
- 目的端站点：选择目的端站点，会创建 Site-to-Site 策略；或选择目的端为 Internet ，会创建 Site-to-Internet 策略；或者选择目的端为 Private Network 。**源端和目的端站点不能为同个**。
```java
// origin properties  
private String siteId;  
@Transient private String siteName;  
private String edgeId;  
private Integer edgeIntId;  
private @Transient String edgeName;  
  
// peer properties  
private String peerSiteId;  
@Transient private String peerSiteName;  
private String peerEdgeId;  
private Integer peerEdgeIntId;  
private @Transient String peerEdgeName;  
private Set<String> proxySites;  
  
// 用于c2pn类型的policy  
private String peerPopId;  
private Integer peerPopIntId;  
// c2pn的policy会下发到pop上，此时，下发到pop的policy消息里需要使用该值作为消息里的id，  
// 防止多个customer共用一个pop时出现policyId重复  
private Long popPolicyId;  

```

```java
/*  
  Policy场景相关参数  
*/  
private PolicyModeEnum mode; // 策略场景，例如site-site，此字段之前是给前端使用的，后端没有用到  
/**  
 * - manual site2site, dualflag=1 - manual internet, dualflag=0 - full mesh, dualflag=1 - * hubspoke, dualflag=1 - transit, dualflag=0 - saas & internet, dualflag=0 - c2pn, dualflag=0 */// false 当前policy的流量只能被单向匹配  
// true 当前policy的流量能被反向匹配  
private boolean dualFlag;  
// 标识此policy是否为下载在Spoke CPE上的transit场景policy  
// 即origin和peer lw是各自spoke到Hub的connection  
private boolean transit = false;  
  
// 标识此Policy是否为下发在Hub站点CPE上，用于transit流量到Spoke站点的单向Policy  
private boolean hubTransit = false;  
private boolean hubTransitMsgDeleted = false;  
@Transient private boolean masterChangedForDualRelated = false;  
@Transient private boolean needFilterOutgoingForDualHubTransit;  
@Transient private boolean dualHubTransitPolicyBetweenHub;  
  
// 只有多cpe，或曾经是多cpe的policy里该值为true  
private boolean multiCpeFormat = false;  
private long enableMultiCpeTimestamp = 0;  
  
/*  
 * 下发消息时决定是否要进行Flow与Rule的一致性检查  
 * 默认为true，对于用户修改配置或者是原来bypass的policy变为非bypass，都需要进行检查  
 * 但对于切路变化outgoing interface的情况，无需检查，设为false  
 */@Transient @JsonIgnore private boolean revalflag = true;  
  
// 由非Policy应用生成的Policy  
private boolean innerServiceGenerated = false; // Per cpe配置的高级配置生成的policyRule(如ACL，Bridge模式等)  
private String identityKey;  
private PolicyBasedServiceEnum serviceType; 
```
Policy 的**五元组**：
```java
/*  
 Match条件部分（对应界面上filter配置）  
*/  
private @Valid PolicyMatch match;  

private String accessListId; // saas policy的match条件使用 // 如果PolicyMatch中关联了AccessList，则这里是对应ID
private @Transient boolean invalidAccessList = false; // 当policy配置的access list是invalid,该字段改为true  
```
具体的 Match ：
- Layer2 ：**只有目的端选择 Site 时才有这一配置项**。二层组网时需要开启。
- proto：选择协议类型，是个数字。ANY 表示 **匹配策略不对协议类型进行限制** 。
    - 选择TCP、UDP等协议后，还可配置端口号；
    - ICMP，DNS等协议不需要端口号；
    - 如果选择了DNS协议，则不通过 Policy 消息进行下发，而是**通过 DNS 消息和 DNS Table 进行下发**，并且**配置后不支持和其他协议互相切换**。
- 源端：选择：
    - 可信本地子网 Trust Subnet ：会自动配置站点的可信本地子网；**要注意，不同安全域的本地可信子网可能不同**。因此会根据上面配置的安全域，选择对应的本地可信子网。
    - 手动配置 Manual：手动填入源端 IPv4, v6 ，可以填写多个；IPv4, v6 可以同时配置在一个 Policy 中。
    - 任意 ANY ：不限制源端 IP
- 目的端：选择目的端子网地址，**配置项内容与源端大部分相同**。但==目的端为 Internet 时，支持通过接入列表的方式选择==。
- dscp ：定义在报文头部的某个字段，标识该报文的优先级。
```java
public class PolicyMatch implements End2EndTaskElement {
// 对应界面上Filter部分src和dst选择的模式  
private PolicyMatchMode srcMode;  
private PolicyMatchMode dstMode;  
  public enum PolicyMatchMode {
      ANY(0),  // 任意子网
      MANUAL(1),  // 手动配置的IPv4,v6,域名 
      ACCESS_NAME(2),  // 访问列表
      SUBNET(3),  
      WAN_AVAILABILITY_ZONE(4);

// 如果mode为entire subnet，则此字段存储对应cpe的key（customerId + cpeId）  
private String srcLocalSubnet;  
private String dstLocalSubnet;  
  
// 五元组条件  
private List<PolicyMatchSubnetPort> srcLocalSubnetPorts; // Site支持多Lan后，支持配置单独lan的subnet  
private List<PolicyMatchSubnetPort>  
    dstLocalSubnetPorts; // #18371：支持多Wan Bgp后，policy允许选择dst为Wan subnet  

/// 下列参数都可多个
@MultiIpvalidator private String srcIp;  /// 源端IP
@MultiIpv6Validator private String srcIpv6s;  /// 源端IP
@PortRangeValidator  
private String srcPort; /// support single port or multiple port separated by comma   /// 源端端口
@MultiIpvalidator private String dstIp;  /// 目的端IP
@MultiIpv6Validator private String dstIpv6s;  /// 目的端IPv6
@PortRangeValidator  
private String dstPort; /// support single port or multiple port separated by comma  /// 目的端端口
  
private String proto;  /// 协议
  
// Pop policy使用，待确认
private Set<String> multiProtocols;  
  
private Integer dscp;  
  
@Deprecated private String inif;  
@Deprecated private String outif;  
  
private String accessnameId;  
private Integer accessnameIntId;  
private String alName;  
  
// 对于需要匹配Inport的Policy，可以配置源目的两侧的入端口类型(Lan/Wan)和接口ID  
private DeviceInterfaceTypeEnum srcInportType;  
private Integer srcInportInterfaceId;  
private DeviceInterfaceTypeEnum dstInportType;  
private Integer dstInportInterfaceId;  
  
// Dst匹配项  
private DeviceInterfaceTypeEnum outPortType;  
private Integer outPortInterfaceId;  
  
private boolean drop = false; // SaaS 中目标 Access Name 是否可以访问
```
加速模式：**TCP协议的用户数据流量**，进入TCP-Tunnel，开启ZetaTCP加速。 accelerate
带宽限速：针对**匹配策略的流量**，按照上下行带宽限速，可以选择已经配置的速率限制。
按照源IP限速：针对匹配策略的流量，按照源IP地址限速，且每个源IP地址单独限速至所配置的数值。
```java
/*  
Policy Action部分（drop放在了match里面）  
*/  
private @Valid Ratelimit ratelimit;  
private boolean accelerate = false;  
```
出口模式：**当目的端为 Internet 时，支持配置模式**。
```java
PolicyOutgoingStrategyEnum outgoingStrategy = PolicyOutgoingStrategyEnum.AVAILABLE;  
private int qualityAssuranceMode = 0; // 0: default, 集成customer配置；1：打开；-1：关闭  
  
private PolicyType type = PolicyType.INTRANET_OVER_LW; // outgoing interface类型  
private PolicyProxyPopSelectMode proxyGatewaySelectMode =  
    PolicyProxyPopSelectMode.AUTO; // ProxyGateway模式下需要选择  
private String proxyPopRegion;  
  
// Outgoint Action  
private InternetLink originTransparentWan;  
private List<InternetLink> originWans;  
private List<LanLink> originLans;  
private List<HybridOutgoingDto>  
    hybridOutgoingInterfaceConfigForPortal; // Only store in root policy  
private List<GeneralOutgoingInterface>  
    originHybridOutgoingInterface; // 3.5.3版本新增，Site2Internet Policy可以混用不同模式的Outgoing Interface  
  
/*  
 * 符合policy的origin，peer参数的topology列表  
 */
private List<String> topologyIds;  
// 是否手动排序  
private boolean manualSort = false;  
/**  
 * origin 到 peer 的 LightWAN Connections 只有 Transit Mode 产生的 Policy，Spoke 之间的双向 connection 不相同 其余情况  
 * {@link #originLightWans} 与 {@link #peerLightWans} 的值相同  
 */  
private List<LightWanLink> originLightWans;  
  
/** peer 到 origin 的 LightWAN Connections */  
private List<LightWanLink> peerLightWans;  
  
/** 无需手动指定该字段，save 时，会自动维护 该字段将 originLightWans 与 peerLightWans 的结果合并，用于方便查询 */  
private Set<LightWanLink> allLightWans;  
  
/** Proxy Gateway模式的policy用户配置信息存储，用于生成PolicyRule */  
private List<CloudWANGateway> cloudwanGateways;  
  
private List<GatewayPopIfItem>  
    gatewayPopIfItems; // 存储Saas policy选择的gateway pop if，用于后续指定pop if的policy刷新  
  
@Transient private List<C2pLink> lastMileLinks; // 用于刷新LastMile Policy时，传递新计算出的有序的LastMileLink  
private List<String> lastMileLinkIds;  
private List<InternetLink> lastMileAccelerateWans;  
  
// 高级配置部分  
private boolean autoProbeSupoort = true; // 标志此policy可以被用于全自动探测，云视讯南北向全自动探测使用  
private boolean exclusiveMode = false; // #20074: policy独占wan模式开关  
private long exclusivePolicyTriggerThreshold =  
    EXCLUSIVE_THRESHOLD; // 默认触发独占模式的流量阈值(目前阈值最大设置为1,000,000,000 kbps，可以满足目前场景)  
@Valid private PolicyEffectTimeConfig policyEffectTimeConfig = new PolicyEffectTimeConfig();  
  
// 和Persistent Policy相关的字段  
@JsonIgnore @Transient private boolean fullSync = false; // CPE login全量回复的policy消息需要增加标识  
  
private boolean persistent = true; // 表示此Policy需要固化在CPE上  
private boolean historyPersistent = true;  
  
@Transient private boolean supportIpv6 = false;  
  
// Hidden policy flag  
// 此标记位在3.3.5版本后不再使用，所有用户可见的policy不应需要多点一次hidden按钮  
@Deprecated private boolean hidden = false;  
  
// PolicyTemplate相关参数，只有rootPolicy中有值  
private String policyTemplateInstanceId;  
private Set<String> defaultPolicyTemplateInstanceIds;  
private List<String> policyTagIds; // 配置了policyTag的policyTemplate在生成policy时会传递该参数  
  
// 2.2版本新增Duplicate connection outgoing interface类型  
private boolean duplicateConnection = false;  
private int reseqTimeout = 32;  
private int reseqBufferSize = 256;  
  
/** 为加速处理Default Policy合并而保存的冗余信息 */  
private Map<String, List<LightWanLink>> topologyOriginLwMap;  
  
private Map<String, List<LightWanLink>> topologyPeerLwMap;  
  
/** 根据SLA选路的结果 */  
@Deprecated private InternetLink activeWan;  
  
@Deprecated private LightWanLink activeOriginLightWan;  
@Deprecated private LightWanLink activePeerLightWan;  
  
/*  
 2.2版本后outgoing切换的逻辑下放到DP，需要将配置的所有outgoing interface下发  
 为了兼容老版本CPE，原activeWan、activeOriginLightWan、activePeerLightWan暂时保留  
*/  
private Integer transparentConnectionLabel; // 透明模式的Connection由流量匹配Transparent policy后生成并上报  
private List<InternetLink> outgoingWans;  
private List<LanLink> outgoingLans;  
private List<LightWanLink> outgoingOriginLightWans;  
private List<LightWanLink> outgoingPeerLightWans;  
private List<GeneralOutgoingInterface>  
    outgoingOriginInterfaces; // 3.5.3 混合类型outgoing interface模式policy下发消息使用  
  
@Transient private List<Map<String, Object>> connectivityList;  
@Transient private Map<Integer, Map<String, Object>> connectivityMap;  
  
@Transient  
private List<LightWanLink>  
    c2pConnectionNeedToCheck; // 编辑Policy类型时，暂存旧Policy的C2P Connection用于检查是否需要删除  
  
/** indicates whether the policy rule should be bypassed */  
private Boolean bypass = false;  
  
/**  
 * Policy是否完成部署： 1，如果deployed=false，那么下发给Agent的Policy消息中Bypass=true  
 * 2，如果deployed=true且bypass=false，那么下发的Policy消息Bypass=false，即为生效状态 使用场景：AccessUnit需要异步通过Restful  
 * API下载的情况下，在AccessUnit下载完成前，Policy不生效 兼容性：deployed默认为生效状态  
 */  
private boolean deployed = true;  
  
private boolean enable = true;  
// 多cpe场景中，当前policy是否为master policy  
private boolean master = true;  
  
private AccessUnitDeployStatusEnum accessUnitDeployStatus = AccessUnitDeployStatusEnum.DEPLOYED;  
  
@JsonSerialize(using = LocalDateTimeSerializer.class)  
private LocalDateTime startDeployTime;  
  
// 若policy为创建site时生成的默认通往Internet的policy时，该值为true  
private boolean forMasterWan = false;  
private boolean defaultPolicyManualChanged = false;  
private boolean c2pConnectionOrderManualChanged = false;  
private boolean needUpdateC2PConnection =  
    false; // 如果C2P Connection因为订阅到期，Connection被删除，则下一次save policy时，需要强制刷新重建C2P Connection  
  
/* 用于标识该policy 是否为 relatedAU对应的policyRule  
 * 历史数据查出来可能为null  
 */private Boolean useRelatedAU = false;  
  
private List<HybridPNOutgoing> hybridPNOutgoingList;  
  
/*  
 * 下发 Policy 消息前，根据顺序构造该字段  
 */@JsonIgnore @Transient private Integer originAfter;  
  
@JsonIgnore @Transient private Integer originPersistentAfter;  
  
@JsonIgnore @Transient private Integer peerAfter;  
  
@JsonIgnore @Transient private Integer peerPersistentAfter;  
  
/** 2.2版本开始记录待下发消息的RatelimitInstance（edit or delete） */  
@JsonIgnore @Transient private List<RatelimitInstance> editList = new ArrayList<>();  
  
@JsonIgnore @Transient private List<RatelimitInstance> deleteList = new ArrayList<>();  
  
/**  
 * 在新的排序策略里，用于记录需要删除的rateLimit key:rateLimitInstanceId value:edgeId 实现zone4  
 * policy的保存与消息下发拆分后，在policy保存阶段，会将需要删除的rateLimit记录在此 在执行消息下发任务时，会根据它来下发删除rateLimit的消息，消息下发后会将该值清空  
 */  
private Map<Long, Integer> deletedRateLimitMap = new HashMap<>();  
  
/**  
 * 供SaasInternet创建删除或编辑Policy时使用的标志位 1,  
 * 如果编辑Policy时，AccessUnit或OriginLw没有发生变化，实际上无需重新下发AccessName或RttTunnel消息 2,  
 * 如果编辑Policy时，有OriginLw减少，应该将没有其他Policy使用的Lw下发消息删除 3，删除Policy时，需要考虑需要删除的OriginLw和AccessUnit  
 * * <p>为了安全起见: 下发Edit的标志参数默认为true，当PolicyService中逻辑明确判断无需下发时再标记为false  
 * 控制下发删除的标志参数默认为false，当PolicyService中操作逻辑明确判断需要删除资源时再置为true  
 */@JsonIgnore @Transient private boolean isAccessUnitChanged = true; // 控制AccessUnit的Edit消息  
  
// 在引用了accessList后，accessList中的每个au都会生成policy  
// 在au第一次生成policy时，需要下发au消息，会将该字段赋值为1，下发之后会将该值更新为null，这样就能保证au消息只下发一次  
// 目前主要使用在zone4的policy保存和消息下发拆分里  
private Boolean auNeedSend;  
  
/*  
子policy顺序  
目前只用在Hybrid_PN policy下  
*/  
private Integer childOrder;  
  
/** 当policy消息体变更时，此字段加1 */  
private Integer msgVersion = 0;  
  
@JsonIgnore @Transient  
private boolean isAccessUnitNeedDelete = false; // 控制删除Policy时，AccessUnit的Delete消息  
  
@JsonIgnore @Transient  
private boolean needCleanIpset = false; // 当两站点间所有Topology被删除后，需要清空对端站点ipset
```