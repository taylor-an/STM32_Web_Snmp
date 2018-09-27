# STM32_Web_Snmp
Web, Snmp，Lwip

Snmp部分暂时未完成， 只是添加了私有节点，可以查询，不能修改
Web部分可以修改本机IP、NTP的IP，可以显示监控的信息，利用SSI，将数据分组打包上传到网页，同组内的数据用$分隔
Uart用来与其他ARM通信，接收监控信息，TOD信息，设置IP等
