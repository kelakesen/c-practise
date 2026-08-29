## fdopen

### 函数原型 FILE *fdopen(int fd, const char *mode);

核心行为：

1. 不创建新文件描述符，只将现有 fd 与新的 FILE 结构关联

2. mode 必须与 fd 的打开方式兼容（如只读 fd 不能用 "w"）

3. 成功返回新流，失败返回 NULL

4. 关闭流时会同时关闭底层 fd，不要重复关闭

最常见用法：包装管道、socket 等系统调用返回的 fd

```c

int pipefd[2];
pipe(pipefd);
FILE *out = fdopen(pipefd[1], "w");
fprintf(out, "Hello via pipe\n");
fclose(out);  // 同时关闭 pipefd[1]
