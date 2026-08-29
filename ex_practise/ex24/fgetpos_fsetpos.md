## fgetpos / fsetpos

### 函数原型 

``` c

int fgetpos(FILE *stream, fpos_t *pos);   
int fsetpos(FILE *stream, const fpos_t *pos);  

注意事项与坑点  

- fpos_t 是不透明类型：你不应该手动创建或修改 fpos_t 对象的内容，只能通过 fgetpos 赋值，然后原样传递给 fsetpos。直接操作其内部字节可能导致未定义行为。

- 成对使用：fsetpos 只能使用由 fgetpos 在同一文件流上保存的位置。在不同流之间使用无效。

- 流必须可定位：fgetpos/fsetpos 只能用于可定位的流（如常规文件）。对于管道、socket、终端等非可定位流，调用会失败并设置 errno 为 ESPIPE。

- 线程安全：与所有标准 I/O 函数一样，在同一流上并发操作需要同步。

- 与 fseek 的互操作性：fgetpos 保存的位置不保证能直接用 fseek 恢复（反之亦然），因为 fpos_t 可能包含额外状态。应使用对应的 fsetpos.

FILE *fp = fopen("data.txt", "r");
fpos_t bookmark;
fgetpos(fp, &bookmark);   // 保存当前位置

// 读取一些数据
char line[100];
fgets(line, sizeof(line), fp);

// 处理后发现需要重新读取刚才的内容
fsetpos(fp, &bookmark);   // 回到保存的位置
// 再次读取同一行
fgets(line, sizeof(line), fp);
fclose(fp);
