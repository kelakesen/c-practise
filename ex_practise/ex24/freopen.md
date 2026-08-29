## freopen

### 函数原型 FILE *freopen(const char *filename, const char *mode, FILE *stream);

--- ---

核心行为：

1. 先关闭stream 当前关联的文件（类似 fclose）

2. 用 filename + mode 打开新文件，绑定到 stream

3. 成功返回 stream，失败返回 NULL

4. 清空的是新文件，不是旧输出

**常见用法：重定向标准流**

> freopen("out.txt", "w", stdout); // printf 全部写入文件
> freopen("in.txt", "r", stdin);   // scanf 从文件读取
