#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    FILE *file;
    struct Database *db;
};

// ======================= 全局变量声明 =======================
static struct Connection *g_conn = NULL;

// ======================= 函数前置声明 =======================
void Database_close(void);

// ======================= 工具函数 =======================
void die(const char *message)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    // 退出前自动清理全局连接
    Database_close();
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

// ======================= 数据库核心操作（无参数版本） =======================
void Database_load(void)
{
    if (!g_conn) die("No connection to load from.");
    int rc = fread(g_conn->db, sizeof(struct Database), 1, g_conn->file);
    if(rc != 1) die("Failed to load database.");
}

void Database_open(const char *filename, char mode)
{
    // 如果已存在连接，先关闭清理
    if (g_conn) Database_close();

    g_conn = malloc(sizeof(struct Connection));
    if(!g_conn) die("Memory error");

    g_conn->db = malloc(sizeof(struct Database));
    if(!g_conn->db) die("Memory error");

    if(mode == 'c') {
        g_conn->file = fopen(filename, "w");
    } else {
        g_conn->file = fopen(filename, "r+");
        if(g_conn->file) {
            Database_load();
        }
    }

    if(!g_conn->file) die("Failed to open the file");
}

void Database_close(void)
{
    if(g_conn) {
        if(g_conn->file) fclose(g_conn->file);
        if(g_conn->db) free(g_conn->db);
        free(g_conn);
        g_conn = NULL;  // 置空防止野指针
    }
}

void Database_write(void)
{
    if (!g_conn) die("No connection to write.");
    rewind(g_conn->file);

    int rc = fwrite(g_conn->db, sizeof(struct Database), 1, g_conn->file);
    if(rc != 1) die("Failed to write database.");

    rc = fflush(g_conn->file);
    if(rc == -1) die("Cannot flush database.");
}

void Database_create(void)
{
    if (!g_conn) die("No connection to create.");
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address addr = {.id = i, .set = 0};
        g_conn->db->rows[i] = addr;
    }
}

void Database_set(int id, const char *name, const char *email)
{
    if (!g_conn) die("No connection to set.");
    struct Address *addr = &g_conn->db->rows[id];
    if(addr->set) die("Already set, delete it first");

    addr->set = 1;
    // WARNING: 保留原书的 strncpy bug 以作演示
    char *res = strncpy(addr->name, name, MAX_DATA);
    if(!res) die("Name copy failed");

    res = strncpy(addr->email, email, MAX_DATA);
    if(!res) die("Email copy failed");
}

void Database_get(int id)
{
    if (!g_conn) die("No connection to get.");
    struct Address *addr = &g_conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_delete(int id)
{
    if (!g_conn) die("No connection to delete.");
    struct Address addr = {.id = id, .set = 0};
    g_conn->db->rows[id] = addr;
}

void Database_list(void)
{
    if (!g_conn) die("No connection to list.");
    int i = 0;
    struct Database *db = g_conn->db;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];
        if(cur->set) {
            Address_print(cur);
        }
    }
}

// ======================= 主函数 =======================
int main(int argc, char *argv[])
{
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    int id = 0;

    // 打开全局连接（不再接收返回值）
    Database_open(filename, action);

    // 解析 ID（只有需要 ID 的动作才真正使用）
    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There's not that many records.");

    switch(action) {
        case 'c':
            Database_create();
            Database_write();
            break;

        case 'g':
            if(argc != 4) die("Need an id to get");
            Database_get(id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set");
            Database_set(id, argv[4], argv[5]);
            Database_write();
            break;

        case 'd':
            if(argc != 4) die("Need id to delete");
            Database_delete(id);
            Database_write();
            break;

        case 'l':
            Database_list();
            break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
    }

    // 显式清理全局连接
    Database_close();

    return 0;
}
