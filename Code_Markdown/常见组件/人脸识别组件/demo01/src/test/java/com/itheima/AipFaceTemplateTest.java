package com.itheima;

import com.itheima.template.ApiFaceTemplate;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

@SpringBootTest(classes = Demo01Application.class)
@RunWith(SpringRunner.class)
public class AipFaceTemplateTest {


    @Autowired
    private ApiFaceTemplate apiFaceTemplate; //注入工具

    @Test
    public void detectFace() {
        String image = "https://itcast-face.oss-cn-beijing.aliyuncs.com/1.jpg";//图片地址
        apiFaceTemplate.decete(image);//检测

    }


}
