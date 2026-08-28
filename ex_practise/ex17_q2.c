#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct Address {
    int id;
    int set;
    char* name;
    char* email;
};

struct Database {
    struct Address* rows;
};

struct Connection {
    FILE *file;
    struct Database *db;
    int MAX_DATA;
    int MAX_ROWS;
};

void die(const char *message){
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addr){
    printf("%d %s %s\n",
            addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    for(int i = 0; i < conn->MAX_ROWS; i++) {
        struct Address *addr = &conn->db->rows[i];
        fread(&addr->id,  sizeof(int), 1, conn->file);
        fread(&addr->set, sizeof(int), 1, conn->file);
        fread(addr->name,  sizeof(char), conn->MAX_DATA, conn->file);
        fread(addr->email, sizeof(char), conn->MAX_DATA, conn->file);
    }
}

struct Connection *Database_open(const char *filename, char mode,int MAX_DATA,int MAX_ROWS)
{
    
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error");

    conn->MAX_DATA = MAX_DATA;
    conn->MAX_ROWS = MAX_ROWS;

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error");

    conn->db->rows = malloc(MAX_ROWS*sizeof(struct Address));
    if(!conn->db->rows) die("Memory error");

    for(int i=0;i<MAX_ROWS;i++){
	    conn->db->rows[i].name = malloc(MAX_DATA*sizeof(char));
	    if(!conn->db->rows[i].name) die("Memory error");

	    conn->db->rows[i].email = malloc(MAX_DATA*sizeof(char));
	    if(!conn->db->rows[i].email) die("Memory error");
    }

    if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }

    if(!conn->file) die("Failed to open the file");

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        if(conn->file) fclose(conn->file);
	if(conn->db->rows){
		for(int i=0;i<conn->MAX_ROWS;i++){
			free(conn->db->rows[i].name);
	
			free(conn->db->rows[i].email);
		}

	       	free(conn->db->rows);
	}

        if(conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);
    for(int i = 0; i < conn->MAX_ROWS; i++) {
        struct Address *addr = &conn->db->rows[i];
        fwrite(&addr->id,  sizeof(int), 1, conn->file);
        fwrite(&addr->set, sizeof(int), 1, conn->file);
        fwrite(addr->name,  sizeof(char), conn->MAX_DATA, conn->file);
        fwrite(addr->email, sizeof(char), conn->MAX_DATA, conn->file);
    }

    int rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.");
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < conn->MAX_ROWS; i++) {
        // make a prototype to initialize it
	conn->db->rows[i].id = i;
  	conn->db->rows[i].set = 0;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first");

    addr->set = 1;
    // WARNING: bug, read the "How To Break It" and fix this
    char *res = strncpy(addr->name, name, conn->MAX_DATA);
    addr->name[conn->MAX_DATA-1]='\0';
    // demonstrate the strncpy bug
    if(!res) die("Name copy failed");

    res = strncpy(addr->email, email, conn->MAX_DATA);
    addr->email[conn->MAX_DATA-1]='\0';
    if(!res) die("Email copy failed");
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set");
    }
}

void Database_delete(struct Connection *conn, int id)
{
	conn->db->rows[id].set = 0;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < conn->MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];

        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 5) die("USAGE: ex17 <dbfile> <action> [action params]");

    char *filename = argv[1];
    char action = argv[2][0];
    int MAX_DATA = atoi(argv[3]);
    int MAX_ROWS = atoi(argv[4]);
    struct Connection *conn = Database_open(filename, action,MAX_DATA,MAX_ROWS);
    int id = 0;

    if(argc > 5) id = atoi(argv[5]);
    if(id >= MAX_ROWS) die("There's not that many records.");

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 6) die("Need an id to get");

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 8) die("Need id, name, email to set");

            Database_set(conn, id, argv[6], argv[7]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 6) die("Need id to delete");

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list");
    }

    Database_close(conn);

    return 0;
}
