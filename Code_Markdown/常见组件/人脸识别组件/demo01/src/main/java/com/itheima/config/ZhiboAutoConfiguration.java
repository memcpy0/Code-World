package com.itheima.config;

import com.itheima.template.ApiFaceTemplate;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class ZhiboAutoConfiguration {



    @Bean
    public ApiFaceTemplate apiFaceTemplate(){

        return new ApiFaceTemplate();
    }
}