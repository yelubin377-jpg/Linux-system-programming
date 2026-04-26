# 从"一个连接一个线程"到Reactor：C++实现一个极简事件驱动服务器

> 知识分享|C++17

---

## 一、多线程服务器的"死亡陷阱"

### 传统多线程Echo服务器

> 代码位置：`Tradition/main.cpp`

```cpp
void handle_client(int client_fd)
{
    char buf[1024];
    while(read(client_fd,buf,sizeof(buf)))
    {
        write(client_fd,buf,strlen(buf));
    }
    close(client_fd);
}

int main()
{
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    //socket:开一个“电话机”，AF
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(server_fd,(struct sockaddr*)&addr,sizeof(addr));
    listen(server_fd,10);
    while(true)
    {
        int client_fd = accept(server_fd,nullptr,nullptr);
        std::thread(handle_client,client_fd).detach();
    }
}
```

**"这个服务器有什么问题？"**

- 1000个连接 = 1000个线程，上下文切换爆炸
- 每个线程栈占8MB内存，1000个线程 = 8GB
- 大量线程互相抢锁，性能急剧下降

### C10K问题

> 当连接数超过1万，这个模型直接死了。那怎么办？
>
> 用**一个线程**，同时处理**1万个连接** → 这就是 **Reactor**

---

## 二、Reactor模式核心思想（7分钟）

### 什么是事件驱动？

| 模型 | 比喻 | 工作方式 |
|------|------|----------|
| 多线程模型 | 我主动去等每个客户吃饭 | 阻塞等待 |
| Reactor模型 | 客户到了喊我，我再去服务 | 事件通知 |

### Reactor的三个核心角色

| 角色 | 职责 |
|------|------|
| **EventLoop**（事件循环） | 监听所有事件，是整个Reactor的心脏 |
| **Acceptor**（接收器） | 专门处理新连接事件 |
| **Handler**（处理器） | 专门处理已连接客户端的读写事件 |

### 单线程Reactor工作流程

```
客户端发起连接
      ↓
EventLoop 监听到事件
      ↓
   是新连接？──→ 交给 Acceptor 处理 ──→ 创建 Handler
      ↓
   是读写？──→ 交给对应的 Handler 处理 ──→ 回显数据
```

---

## 三、C++从零实现极简Reactor（15分钟）

### 第一步：封装文件描述符（RAII）

> 代码位置：`Reactor/Fd.h`

```cpp
class Fd
{
private:
    int fd_;

public:
    explicit Fd(int fd) : fd_(fd) {};
    ~Fd(){if(fd_ >= 0) close(fd_);}

    Fd(const Fd&) = delete;
    Fd& operator = (const Fd&) = delete;

    Fd(Fd&&other)noexcept : fd_(std::exchange(other.fd_,-1)) {}
    Fd& operator = (Fd&&other) noexcept
    {
        std::swap(fd_,other.fd_);
        return *this;
    }

    int get() const {return fd_;}
};
```

**讲解要点：**

- 构造时拿到fd，析构时自动close，不用手动管
- 禁止拷贝：两个对象不能同时管一个fd
- 允许移动：可以把fd的所有权转让出去

---

### 第二步：封装select事件监听器

> 代码位置：`Reactor/EventLoop.h`

```cpp
class EventLoop
{
private:
    using EventCallback = std::function<void()>;
    bool running_ = true;
    std::unordered_map<int,EventCallback> callbacks_;
public:
    void add_event(int fd,EventCallback callback)
    {
        callbacks_[fd] = std::move(callback);
    }

    void remove_event(int fd)
    {
        callbacks_.erase(fd);
    }

    void loop()
    {
        while(running_)
        {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            int max_fd = -1;

            for(const auto& [fd,_] : callbacks_)
            {
                FD_SET(fd,&read_fds);
                max_fd = std::max(max_fd,fd);
            }

            int nready = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
            if(nready < 0)   continue;
            for(const auto &[fd,callback_] : callbacks_)
            {
                if(FD_ISSET(fd,&read_fds))
                {
                    callback_();
                }
            }
        }
    }

    void stop() {running_ = false;}
};
```

**讲解要点：**

- `callbacks_` 是一张表：fd → 回调函数
- `add_event`：往表里注册"这个fd有事件时该干嘛"
- `loop`：死循环，用select等事件，有事件就查表调用对应的回调

---

### 第三步：实现Acceptor和EchoHandler

> 代码位置：`Reactor/Acceptor.h`

```cpp
class EchoHandler : public std::enable_shared_from_this<EchoHandler>
{
public:
    EchoHandler(EventLoop& loop, int client_fd)
        : loop_(loop), client_fd_(client_fd)
    {
        loop_.add_event(client_fd_.get(), [this]() { handle_read(); });
    }

private:
    void handle_read()
    {
        char buf[1024];
        int n = read(client_fd_.get(), buf, sizeof(buf));
        if (n <= 0)
        {
            loop_.remove_event(client_fd_.get());
            return;
        }
        write(client_fd_.get(), buf, n);
    }

    EventLoop& loop_;
    Fd client_fd_;
};

class Acceptor
{
public:
    Acceptor(EventLoop& loop, int port) : loop_(loop), server_fd_(create_listen_fd(port))
    {
        loop_.add_event(server_fd_.get(), [this]() { handle_accept(); });
    }

private:
    static int create_listen_fd(int port)
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(port);
        bind(fd, (struct sockaddr*)&addr, sizeof(addr));
        listen(fd, 10);
        return fd;
    }

    void handle_accept()
    {
        int client_fd = accept(server_fd_.get(), nullptr, nullptr);
        auto handler = std::make_shared<EchoHandler>(loop_, client_fd);
        handlers_.push_back(handler);
    }

    EventLoop& loop_;
    Fd server_fd_;
    std::vector<std::shared_ptr<EchoHandler>> handlers_;
};
```

**讲解要点：**

- Acceptor负责接客，来一个新连接就创建一个EchoHandler
- EchoHandler负责干活，收到什么就回什么
- 用shared_ptr管理Handler生命周期，客户端断了自动清理

---

### 第四步：主函数

```cpp
int main()
{
    EventLoop loop;
    Acceptor acceptor(loop, 8080);
    std::cout << "Reactor server running on port 8080" << std::endl;
    loop.loop();
    return 0;
}
```

---

## 四、现场性能对比演示（5分钟）

### 准备

| 程序 | 说明 |
|------|------|
| 程序A | `Tradition/main.cpp` 传统多线程服务器 |
| 程序B | `Reactor/` Reactor单线程服务器 |

### 压测命令

```bash
ab -c 1000 -n 10000 http://localhost:8080/
```

### 预期结果

| 指标 | 多线程服务器 | Reactor单线程 |
|------|-------------|--------------|
| QPS | ~1000 | ~5000+ |
| CPU占用 | 100% | ~20% |
| 稳定性 | 可能崩溃 | 稳定运行 |

**结论：一个线程的Reactor，性能是多线程服务器的5倍以上**

---

## 五、总结与展望（3分钟）

### 今天解决了什么

- 多线程服务器的C10K问题
- 用事件驱动替代了线程驱动
- 用C++ RAII和std::function写出了优雅的代码

### 接下来要学什么

| 方向 | 内容 |
|------|------|
| select的缺点 | 升级到epoll |
| 单线程瓶颈 | 主从Reactor模式（Nginx、Redis就是这么干的） |
| 计算密集任务 | 多线程Reactor + 线程池 |

### C++专属加分项

- `std::shared_ptr` 管理Handler生命周期：解决野指针问题
- `std::function` 替代函数指针：支持lambda，代码更灵活
- RAII管理所有资源：不用手动close
- 禁止拷贝，允许移动：符合现代C++语义

---

> Reactor不是一个框架，而是一种思想。当你理解了事件驱动，你就理解了整个Linux网络编程的核心。
