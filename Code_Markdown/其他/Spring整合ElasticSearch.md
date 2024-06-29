### 1. 配置（废弃了）
```xml
# ElasticSearchProperties
spring:
	data:
		elasticsearch:
			cluster-name: nowcoder-community
			cluster-nodes: 127.0.0.1:9300 # 用TCP去访问它
```
ElasticSearch底层基于Netty，而Redis底层也基于Netty，两者会有冲突，主要体现在ES底层的代码上。解决冲突：
```java
package com.nowcoder.community;  
  
import lombok.extern.slf4j.Slf4j;  
import org.springframework.boot.SpringApplication;  
import org.springframework.boot.autoconfigure.SpringBootApplication;  
  
import javax.annotation.PostConstruct;  
  
@Slf4j  
@SpringBootApplication  
public class CommunityApplication {  
  
   @PostConstruct  
   public void init() {  
      // 解决netty启动冲突问题  
      // see Netty4Utils.setAvailableProcessors()   
System.setProperty("os.set.netty.runtime.available.processors", "false");  
   }  
   public static void main(String[] args) {  
      SpringApplication.run(CommunityApplication.class, args);  
      log.info("项目启动成功");  
   }  
  
}
```
### 2. 使用
在ES要访问的数据表上用注解进行标明，将实体数据自动映射到ES服务器里面的索引，映射到哪个索引、哪个类型、创建几个分片、几个副本——没有索引会自动创建索引，创建分片和副本，插入数据：
```java
package com.nowcoder.community.entity;  
  
import lombok.AllArgsConstructor;  
import lombok.Data;  
import lombok.NoArgsConstructor;  
import lombok.ToString;  
import org.springframework.data.annotation.Id;  
import org.springframework.data.elasticsearch.annotations.Document;  
import org.springframework.data.elasticsearch.annotations.Field;  
import org.springframework.data.elasticsearch.annotations.FieldType;  
  
import java.time.LocalDateTime;  
  
@Document(indexName = "discusspost", shards = 6, replicas = 3) // 根据服务器处理能力来配置  
@Data  
@AllArgsConstructor  
@NoArgsConstructor  
@ToString  
public class DiscussPost {  
    @Id // 建索引时把这个数据存到ID字段  
    private int id; // 帖子ID  
  
    @Field(type = FieldType.Integer)  
    private int userId; //  
  
    @Field(type = FieldType.Text, analyzer = "ik_max_word", searchAnalyzer = "ik_smart") // 用于搜索,存储时的解析器,搜索时的解析器  
    private String title; // 标题  
  
    @Field(type = FieldType.Text, analyzer = "ik_max_word", searchAnalyzer = "ik_smart") // 用于搜索,存储时的解析器,搜索时的解析器  
    private String content; // 内容  
  
    @Field(type = FieldType.Integer)  
    private int type; // 是否置顶  
  
    @Field(type = FieldType.Integer)  
    private int status; // 0-正常,1-精华,2-拉黑  
    @Field(type = FieldType.Date)  
    private LocalDateTime createTime; // 创建时间  
  
    @Field(type = FieldType.Integer)  
    private int commentCount; // 评论数  
  
    @Field(type = FieldType.Double)  
    private double score; // 帖子分数  
  
}
```
@Mapper是Mybatis专有的注解，@Repository是Spring提供的针对数据访问层的注解，[文档](https://docs.spring.io/spring-data/elasticsearch/docs/current/reference/html/#elasticsearch.mapping.meta-model.annotations)。

测试一下：
```java
package com.nowcoder.community;  
  
import com.nowcoder.community.dao.DiscussPostMapper;  
import com.nowcoder.community.dao.elasticsearch.DiscussPostRepository;  
import org.junit.jupiter.api.Test;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.boot.test.context.SpringBootTest;  
import org.springframework.data.elasticsearch.core.ElasticsearchRestTemplate;  
import org.springframework.test.context.ContextConfiguration;  
  
@SpringBootTest  
@ContextConfiguration(classes = CommunityApplication.class)  
public class ElasticSearchTests {  
    @Autowired  
    private DiscussPostMapper discussPostMapper;  
    @Autowired  
    private DiscussPostRepository discussPostRepository;  
    @Autowired  
    private ElasticsearchRestTemplate elasticsearchRestTemplate;  
  
    @Test  
    public void testInsert() {  
        discussPostRepository.save(discussPostMapper.selectById(241)); // 从数据库查询数据,插入ES中(自动创建索引)  
        discussPostRepository.save(discussPostMapper.selectById(242));  
        discussPostRepository.save(discussPostMapper.selectById(243));  
    }  
}
```
运行后，自动创建了Index和Mapping：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304182115728.png)

注意： ProductNotSupportedError: The client noticed that the server is not Elasticsearch and we do not support this unknown product。这种错误说明，不兼容——[支持一览表 | Elastic](https://www.elastic.co/cn/support/matrix#matrix_compatibility)。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304182135714.png)
排序时优先置顶（type）、然后看分数score（加精提高分数）
最后是按照时间排序。

```java
NativeSearchQuery searchQuery = new NativeSearchQueryBuilder()  
     .withQuery(QueryBuilders.multiMatchQuery("互联网寒冬", "title", "content")) // 从哪个Field中进行查询  
        .withSort(SortBuilders.fieldSort("type").order(SortOrder.DESC)) // 多级排序,先写先排  
        .withSort(SortBuilders.fieldSort("score").order(SortOrder.DESC))  
        .withSort(SortBuilders.fieldSort("createTime").order(SortOrder.DESC))  
        .withPageable(PageRequest.of(0, 10)) // 分页  
        .withHighlightFields(  
                new HighlightBuilder.Field("title").preTags("<em>").postTags("</em>"),  
                new HighlightBuilder.Field("content").preTags("<em>").postTags("</em>")  
        ).build();
```
Repository的search方法底层用elasticTemplate.queryForPage(searchQuery, class, SearchResultMapper)方法处理查询。底层获取到了高亮显示的值，但没有返回，返回的是原始数据。

我们干脆用后者算了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304182148026.png)
实现一个SearchResultMapper接口的匿名子类，在里面遍历并处理命中的数据：
```java
new SearchResultMapper() {
	@Override
	public <T> AggregatedPage<T> mapResults(SearchResponse response, Class<T> aClass, Pageable pageable) {
		SearchHits hits = response.getHits();
		if (hits.getTotalHits() <= 0) {
			return null;
		}
		List<DiscussPost> list = new ArrayList<>();
		for (SearchHit hit : hits) {
			DiscussPost post = new DiscussPost();
			String id = hit.getSourceAsMap().get("id").toString();
			post.setId(Integer.valueOf(id));
			String userId = hit.getSourceAsMap().get("userId").toString();
			post.setUserId(Integer.valueOf(userId));
			// 字段里面可能没有要高亮显示内容
			
			String title = hit.getSourceAsMap().get("title").toString();
			post.setTitle(title);
			
			String content = hit.getSourceAsMap().get("content").toString();
			post.setContent(content);

			String status = hit.getSourceAsMap().get("status").toString();
			post.setStatus(Integer.valueOf(status));

			String createTime = hit.getSourceAsMap().get("createTime").toString();
			post.setCreateTime(new LocalDateTime(Long.valueOf(status))); // es在存日期时将其转为Long存储
			String commentCount = hit.getSourceAsMap().get("commentCount").toString();
			post.setCommentCount(Integer.valueOf(commentCount));

			// 处理高亮显示的结果
			HighlightField titleField = hit.getHighlightFields().get("title");
			if (titleField != null) { //只高亮第一段
				post.setTitle(titleField.getFragments()[0].toString());
			}
			HighlightField contentField = hit.getHighlightFields().get("content");
			if (contentField != null) {
				post.setTitle(contentField.getFragments()[0].toString());
			}
			list.add(post);
			return new AggregatedPageImpl<>(list, pageable, hits.getTotalHits(), /* 多少条数据 */ response.getAggregations(), response.getScrollId(), hits.getMaxScore());
		}
	}
}
```
执行结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304182200571.png)
返回的结果里是匹配到的那一段的内容，而非整个数据。