#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "ex19.h"

int Monster_attack(void *self,int damage){
           
       	Monster *monster = self;	

        printf("You attack the %s \n",monster->_(description));

        monster->HP -= damage;

        if(monster->HP > 0){ 

                printf("the monster is still alive with %d HP \n",monster->HP);

                return 1;

        }   
        else{
                printf("the monster is dead,congratulation");
            
                return 0;
        }   

}
int Monster_init(void *self){

        Monster *monster = self;

        monster->HP = 10; 

        return 1;
}
Object MonsterProto = {
        .init = Monster_init,
        .attack = Monster_attack
};

void *Room_move(void *self,Direction direction){
	
	Room *room = self;
	Room *next = NULL;
	switch(direction){
		case NORTH:
			if(room->north)next = room->north;
			printf("You go north, into:\n");
			break;
	
		case SOUTH:
			if(room->south)next = room->south;
			printf("You go south, into:\n");
			break;

		case EAST:
			if(room->east)next = room->east;
			printf("You go east, into:\n");
			break;

		case WEST:
			if(room->west)next = room->west;
			printf("You go west, into:\n");
			break;
		default:
			printf("You can't go that direction");
			break;
		}

	if(next)next->_(describe)(next);
  	 

	return next;

}

int Room_attack(void *self,int damage){

	Room *room = self;
	Monster *monster = room->bad_guy;

	if(monster){
		monster->_(attack)(monster,damage);
		return 1;
	}
	else{
		printf("You flail in the air at nothing,idiot\n");
		return 0;
	}

}

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};


void *Map_move(void *self,Direction direction){

	Map *map = self;
	Room *location = map->location;
	Room *next = NULL;

	next = location->_(move)(location,direction);

	if(next){

		map->location = next;
	
	}

	return next;
}

int Map_attack(void *self,int damage){

	Map *map = self;
	Room *location = map->location;

	return location->_(attack)(location,damage);

}

int Map_init(void *self){
	
	Map *map = self;

	Room *hall = NEW(Room,"the great hall");
	Room *room1 = NEW(Room,"room1");
	Room *room2 = NEW(Room,"room2");
	Room *room3 = NEW(Room,"room3");
	Room *room4 = NEW(Room,"room4");

	hall->north = room1;
	hall->south = room2;
	hall->west = room3;
	hall->east = room4;

	room1->south = hall;
	room2->north = hall;
	room3->east = hall;
	room4->west = hall;

	room1->bad_guy = NEW(Monster,"big evil");

	map->start = hall;
	map->location = hall;

	return 1;

}

Object MapProto = {
	.move = Map_move,
	.attack = Map_attack,
	.init = Map_init
};

int process_input(Map *game)
{
    printf("\n> ");

    char ch = getchar();
    getchar(); // eat ENTER

    int damage = rand() % 4;

    switch(ch) {
        case 'g':
            printf("Giving up? You suck.\n");
            return 0;
            break;

        case 'n':
            game->_(move)(game, NORTH);
            break;

        case 's':
            game->_(move)(game, SOUTH);
            break;

        case 'e':
            game->_(move)(game, EAST);
            break;

        case 'w':
            game->_(move)(game, WEST);
            break;

        case 'a':

            game->_(attack)(game, damage);
            break;
        case 'l':
            printf("You can go:\n");
            if(game->location->north) printf("NORTH\n");
            if(game->location->south) printf("SOUTH\n");
            if(game->location->east) printf("EAST\n");
            if(game->location->west) printf("WEST\n");
            break;

        default:
            printf("What?: %c\n", ch);
    }

    return 1;
}

int main(int argc,char *argv[]){

	srand(time(NULL));

	Map *game = NEW(Map,"The Hall of the Minotaur.");

	printf("You enter the ");
	game->location->_(describe)(game->location);

        while(process_input(game)) {
        }

        return 0;
}






























