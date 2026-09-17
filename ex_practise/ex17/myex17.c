#include <stdio.h>

struct address{
	int id;
	int set;
	char*name;
	char*email;
}

struct database{
	struct address*row;
}

struct connection{
	FILE*file;
	struct database*db;
}
void die(char*message){
	if(errno){
		perror(message)
	}else{
		print("error:%s\n",message)
	}
	exit(1);
}
void database_create(struct connection*conn,int MAX_DATA,int MAX_ROWS){
	conn->db->row=malloc(MAX_ROWS*sizeof(struct address));
	if(conn->db->row)die("memory error for rows");
	for(int i=0;i<MAX_ROWS;i++){	
		conn->db->row[i]->name=malloc(MAX_DATA);
		conn->db->row[i]->email=malloc(MAX_DATA);
		if(!conn->db->row[i]->name||!conn->db->row[i]->email)die("memory error for name or email");
		conn->db->row[i]->id=i;
		conn->db->row[i]->set=0;
	}
}
void database_read(struct connection* conn,int MAX_DATA,int MAX_ROWS){
	for(int i=0;i<MAX_ROWS;i++){
		int rc=fread(&conn->db->row[i]->id,sizeof(int),1,conn->file);
		if(rc!=1)die("memory error for read");
		rc=fread(&conn->db->row[i]->set,sizeof(int),1,conn->file);
		if(rc!=1)die("memory error for read");
		rc=fread(conn->db->row[i]->name,MAX_DATA*sizeof(char),1,conn->file);
		if(rc!=1)die("memory error for read");
		rc=fread(conn->db->row[i]->eamil,MAX_DATA*sizeof(char),1,conn->file);
		if(rc!=1)die("memory error for read");
	}
}
struct connection* database_open(const char* filename,char mode,int MAX_DATA,int MAX_ROWS){
	struct connection*conn=malloc(sizeof(struct connection));
	conn->db=malloc(sizeof(struct database));
	conn->db->row=malloc(MAX_ROWS*sizeof(struct address));
	for(int i=0;i<MAX_ROWS;i++){
	conn->db->row[i]->id=malloc(sizeof(int));
		
	conn->db->row[i]->set=malloc(sizeof(int));
	}
        if(conn->db->row)die("memory error for rows");
        for(int i=0;i<MAX_ROWS;i++){
                conn->db->row[i]->name=malloc(MAX_DATA);
                conn->db->row[i]->email=malloc(MAX_DATA);
                if(!conn->db->row[i]->name||!conn->db->row[i]->email)die("memory error for name or email");
	}
	if(mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_read(conn,MAX_DATA,MAX_ROWS);
        }
    }

    if(!conn->file) die("Failed to open the file");

    return conn;
}
int main(int argc,char*argv[]){
int MAX_DATA=argv[2];
int MAX_ROWS=argv[3];

return 0;
}
