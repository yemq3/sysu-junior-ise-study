# Network Layer:The Data Plane

## Overview

**two important network-layer functions**：

- Forwarding
- Routing

## What's inside a router

### Router Architecture 

#### Input Ports

- 作为接入Rounter的实体
- 完成查询和转发的功能

#### Switching Fabric

三种类型

![image-20191113092934606](ch4.assets/image-20191113092934606.png)

#### Output Ports

- 将重新封装的包发出

#### Routing processor

- 控制流量

### Virtual-Circuit Networks

交换机维护路由器之间连接的虚电路

### Destination-based forwarding  

终端封包时标记目的终端的地址，Router维持一个表用于查询对应的目的终端地址应该转发到哪里

当有多个符合，采用最长前置码相符的原则

![image-20191113092151295](ch4.assets/image-20191113092151295.png)

## IP

### IP datagram

![image-20191113093409624](ch4.assets/image-20191113093409624.png)