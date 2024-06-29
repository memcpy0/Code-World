package com.itheima;

import com.itheima.aliyun.GreenImageScan;
import com.itheima.aliyun.GreenTextScan;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.Map;

@SpringBootTest
@RunWith(SpringRunner.class)
public class Demo02Test {

    @Autowired
    private GreenTextScan greenTextScan;//注入对象

    @Autowired
    private GreenImageScan greenImageScan;


    @Test
    public void testScanText() throws Exception {
        Map map = greenTextScan.greeTextScan("我喜欢打球");//输出文本
        System.out.println(map);//检测结果
    }



}
