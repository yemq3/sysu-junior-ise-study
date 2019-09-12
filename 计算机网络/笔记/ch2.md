## Principles of network applications

### Application architectures

- client-server
- peer-to-peer (P2P)



### Processs Communicating

- *client process:* process that initiates communication
- *server process:* process that waits to be contacted

### Sockets

process sends/receives messages to/from its socket

### Addressing Processes

#### Identifier

- IP Address
- Port Number

### App-layer Protocol Defines

- Message Type
- Message Synax
- Message Semantics
- Message Rules



### Transport Services that the app need

- Data Loss
- Timing
- Throughput
- Security

#### TCP

- Reliable
- Flow conrol
- Congestion control
- Connection-Oriented

#### UDP

- Unrealiable
- No Flow conrol
- No Congestion control
- Connection-less

## Web abd HTTP

- Web page consists of objects
- each object is addressable by a URL

- uses TCP(port 80)

### HTTP Connection

#### Non-Persistent HTTP

一次连接只能发送一个object

![1568287041441](ch2.assets/1568287041441.png)

![1568287098900](ch2.assets/1568287098900.png)

RTT：time for a small packet to travel from client to server and back

![1568287226971](ch2.assets/1568287226971.png)

#### Persistent HTTP

一次连接可以发送多个object

连接初始化需要一个RTT，之后object的发送不再需要重复建立连接



### HTTP request message

Two types of HTTP messages：

- request
- response

![1568287617830](ch2.assets/1568287617830.png)

![1568287454341](ch2.assets/1568287454341.png)

![1568287590046](ch2.assets/1568287590046.png)

#### Method types

- HTTP/1.0
  - GET
  - POST
  - HEAD
- HTTP/1.1
  - GET,POST,HEAD
  - PUT
  - DELETE

#### HTTP response status codes

200,301,400,404,505

### Cookies

用于记录用户信息

- authorization
- shopping carts
- recommendations
- user session state

### Web caches(proxy server)

- reduce response time for client request
- reduce traffic on an institution’s access link
- Internet dense with caches: enables “poor” content providers to effectively deliver content (so too does P2P file sharing)
- ISP block:smile:

![1568288586258](ch2.assets/1568288586258.png)



## Electronic mail

## DNS

### Type

- A
- NS
- CNAME
- MX

## P2P

![1568290059701](ch2.assets/1568290059701.png)

### BitTorrent

- file divided into 256Kb chunks
- peers in torrent send/receive file chunks

## Video streaming and content distribution networks(CDNs)

- CBR
- VBR

### Dynamic Adaptive Streaming over HTTP(DASH)

根据网速决定压缩程度

Client can determines：

- when to request chunk
- what encoding rate to request
- where to request chunk

## Content Distribution Networks(CDNs)

CDN：stores copies of content at CDN nodes

## Socket Programming with UDP and TCP

