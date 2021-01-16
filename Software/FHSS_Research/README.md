# FHSS Research
> https://github.com/FASTSHIFT/FHSS_Research

# 更新日志
## [v1.0] - 2020.12.12
* 1.框架搭建完成，等待硬件中
* 2.UI基于LVGL v7.8.0

## [v1.1] - 2020.12.19
* 1.硬件搭建完成
* 2.测试程序完成
* 3.功能框架搭建完成
* 4.添加成品图片

## [v1.2] - 2020.12.29
* 1.NRF库更新至v3.4
* 2.重新整理跳频算法，基于时隙同步
* 3.添加NRF_Manager模块，支持定频和跳频通信
* 4.添加了第一个界面，用于展示丢包率
* 5.使用lvgl自带的button indev

## [v1.3] - 2021.1.16
* 1.实现双向跳频通信
* 2.去除ComMaster.cpp和ComSlave.cpp
* 3.增加LED_SetToggle
* 4.主从机使用统一的100us定时中断
* 5.使用2Mbps速率通信，保证MAX_RT触发时在10ms以内
* 6.通信周期改为5ms
* 7.添加波形图说明
