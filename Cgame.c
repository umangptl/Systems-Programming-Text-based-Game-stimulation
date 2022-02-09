#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WIN_Respect 80                // type of score system
#define START_Respect 40
#define LOSE_Respect 0

/*- The only global variable-*/
int respect = START_Respect;

typedef struct room room;
struct room{
    int state;
    int roomID;
    room *north;
    room *east;
    room *west;
    room *south;
};

struct creature{
    int type;
    int id;
    room *location;
};

void move_Creature(struct room *r, struct creature *cre, int id, int numbCrea, int cRoom, struct room *rooms);

/* Gets number from user between min and max, to make room and creature */
int getInput(int min,int max){
    int input =0;
    while(input < min || input > max){
        scanf("%d",&input);
    }
    return input;
}

/* specified room for PC to start */
int startRoom(struct creature c[], int s){
    int lastRoom;
    for(int i = 0; i < s; i++){
        if(c[i].type == 0){
            lastRoom = i;
        }
    }
    return lastRoom;
}

/* increase respect if liked */
void like(int typ, int creID){
    if(typ == 1){
        respect++;
        printf("Animal %d licks your face. Respect is now %d. \n",creID,respect);
    }else if(typ == 2){
        respect++;
        printf("NPC %d smiles at you. Respect is now %d. \n",creID,respect);
    }
}

/* decrease respect if disliked*/
void dislike(int typ, int creID){
    if(typ == 1){
        respect--;
        printf("Animal %d growls. Respect is now %d. \n",creID,respect);
    }else if(typ == 2){
        respect--;
        printf("NPC %d grumbles. Respect is now %d. \n",creID,respect);
    }
}

/* if room is full of creatures, return -1 */
int checkifFull(int roomNum, struct creature *cre, int creSize){
    int count = 0;
    for(int i = 0; i < creSize; i++){
        if(cre[i].location->roomID == roomNum){
            count++;
        }
    }if(count < 10 && count >= 0){
        return count;
    }
    return -1;
}

/* Returns pointer of room with Id */
struct room* getRoom(int id, struct room *r, int rSize){
    for(int i = 0; i < rSize; i++){
        if(r[i].roomID == id){
            return &r[i];
        }
    }
}

/* Creates the room with neighbour rooms */
void createRoom(struct room *r,int current,int nRooms, int north, int east, int west, int south){
    r[nRooms].roomID = current;
    if(north != -1){
        r[current].north = getRoom(north,r,nRooms);
    }else {
        r[current].north = NULL;
    }
    if(east != -1){
        r[current].east = getRoom(east,r,nRooms);
    } else{
        r[current].east = NULL;
    }
    if(west != -1){
        r[current].west = getRoom(west,r,nRooms);
    }else {
        r[current].west = NULL;
    }
    if(south != -1){
        r[current].south = getRoom(south,r,nRooms);
    }else {
        r[current].south = NULL;
    }
}

/* moving PC as the users wants */
 int PCmove(char location, struct room *room){
    switch(location){
        case 'n':
            if(room->north != NULL){
                printf("You moved north to room %d \n ", room->north->roomID);
                return room->north->roomID;
            }
            printf("There is nothing to the north \n");
            return -1;
        case 'e':
            if(room->east != NULL){
                printf("You moved east to room %d \n", room->east->roomID);
                return room->east->roomID;
            }
            printf("There is nothing to the east \n");
            return -1;
        case 'w':
            if(room->west != NULL){
                printf("You moved west to room %d \n", room->west->roomID);
                return room->west->roomID;
            }
            printf("There is nothing to the west \n");
            return -1;
        case 's':
            if(room->south != NULL){
                printf("You moved south to room %d \n", room->south->roomID);
                return room->south->roomID;
            }
            printf("There is nothing to the south \n");
            return -1;
    }
}

char* getState(int state){
    if(state == 0){ return "clean";}
    else if(state == 1){ return "half-dirty"; }
    else if(state == 2){ return "dirty";  }
}


int cleanRoom(struct room *r, struct creature *cre, int numbCrea,int cRoom, struct room *rooms){
    if(r->state == 0){
        printf("This room is already clean.\n");
        return -1;
    }
    else if(r->state == 2 || r->state == 1){
        r->state -= 1;
        if(r->roomID == rooms[cRoom].roomID){
            printf("The room is cleaned. It is currently %s.\n",getState(r->state));
        }
    }
    for(int i = 0; i < numbCrea; i++){
        if(cre[i].type > 0 && cre[i].location->roomID == r->roomID){
            if(cre[i].type == 2){
                if(r->roomID == rooms[cRoom].roomID){
                    dislike(cre[i].type,i);
                }
                if(cre[i].location->state == 0){
                    move_Creature(r,cre,i,numbCrea,cRoom, rooms);
                }
            } else if(cre[i].type == 1){
                if(cre[i].location->state == 1 || cre[i].location->state == 0){
                    if(r->roomID == rooms[cRoom].roomID){
                        like(cre[i].type,i);
                    }
                }
            }
        }
    }
    return 1;
}
int dirtyRoom(struct room *r, struct creature *cre, int numbCrea, int cRoom, struct room *rooms){
    if(r->state == 2){
        printf("This room is already dirty.\n");
        return -1;
    }else if(r->state == 0 || r->state == 1){
        r->state += 1;
        if(r->roomID == rooms[cRoom].roomID){
            printf("The room is dirtied. It is currently %s.\n",getState(r->state));
        }
    }
    if(r->roomID == rooms[cRoom].roomID){
        for(int i = 0; i < numbCrea; i++){
            if(cre[i].type > 0 && cre[i].location->roomID == r->roomID){
                if(cre[i].type == 2){
                    if(cre[i].location->state == 1 || cre[i].location->state == 2){
                        if(r->roomID == rooms[cRoom].roomID){
                            like(cre[i].type,i);
                        }
                    }
                } else if(cre[i].type == 1 ){
                    if(r->roomID == rooms[cRoom].roomID){
                        dislike(cre[i].type,i);
                    }
                    if(cre[i].location->state == 2){
                        move_Creature(r,cre,i,numbCrea,cRoom, rooms);
                    }
                }
            }
        }
    }
    return 1;
}

void move_Final(struct room *r, struct creature *cre, int id, int numbCrea, char dir, int cRoom, struct room *rooms){
    switch(dir){
        case 'n':
            if(r[0].north != NULL){
                if(cre[id].type == 1 && r[0].north->state == 2){
                    cre[id].location = r[0].north;
                    cleanRoom(cre[id].location,cre,numbCrea, cRoom, rooms);
                    printf("Animal %d moves to the north and cleans the room.\n",cre[id].id);
                }else if(cre[id].type == 2 && r[0].north->state == 0){
                    cre[id].location = r[0].north;
                    dirtyRoom(cre[id].location,cre,numbCrea,cRoom, rooms);
                    printf("NPC %d moves to the north and dirties the room.\n",cre[id].id);
                } else{
                    cre[id].location = r[0].north;
                    if(cre[id].type == 2){
                        printf("NPC %d", cre[id].id);
                    }
                    if(cre[id].type == 1){
                        printf("Animal %d",cre[id].id);
                    }
                    printf(" moves to the north.\n");
                }
            }
                break;
                case 'e':
        if(r[0].east != NULL) {
            if (cre[id].type == 1 && r[0].east->state == 2) {
                cre[id].location = r[0].east;
                cleanRoom(cre[id].location, cre, numbCrea, cRoom, rooms);
                printf("Animal %d moves to the east and cleans the room.\n", cre[id].id);
            } else if (cre[id].type == 2 && r[0].east->state == 0) {
                cre[id].location = r[0].east;
                dirtyRoom(cre[id].location, cre, numbCrea, cRoom, rooms);
                printf("NPC %d moves to the east and dirties the room.\n", cre[id].id);
            } else {
                cre[id].location = r[0].east;
                if (cre[id].type == 2) {
                    printf("NPC %d", cre[id].id);
                }
                if (cre[id].type == 1) {
                    printf("Animal %d", cre[id].id);
                }
                printf(" moves to the east.\n");
            }
        }
                break;
                case 'w':
        if(r[0].west != NULL) {
            if (cre[id].type == 1 && r[0].west->state == 2) {
                cre[id].location = r[0].west;
                cleanRoom(cre[id].location, cre, numbCrea, cRoom, rooms);
                printf("Animal %d moves to the west and cleans the room.\n", cre[id].id);
            } else if (cre[id].type == 2 && r[0].west->state == 0) {
                cre[id].location = r[0].west;
                dirtyRoom(cre[id].location, cre, numbCrea, cRoom, rooms);
                printf("NPC %d moves to the west and dirties the room.\n", cre[id].id);
            } else {
                cre[id].location = r[0].west;
                if (cre[id].type == 2) {
                    printf("NPC %d", cre[id].id);
                }
                if (cre[id].type == 1) {
                    printf("Animal %d", cre[id].id);
                }
                printf(" moves to the west.\n");
            }
        }
                break;
            case 's':
                if(r[0].south != NULL)
                    if(cre[id].type == 1 && r[0].south->state == 2){
                        cre[id].location = r[0].south;
                        cleanRoom(cre[id].location,cre,numbCrea,cRoom, rooms);
                        printf("Animal %d moves to the south and cleans the room.\n",cre[id].id);
                    } else if(cre[id].type == 2 && r[0].south->state == 0){
                        cre[id].location = r[0].south;
                        dirtyRoom(cre[id].location,cre,numbCrea,cRoom, rooms);
                        printf("NPC %d moves to the south and dirties the room.\n",cre[id].id);
                    } else{
                        cre[id].location = r[0].south;
                        if(cre[id].type == 2){
                            printf("NPC %d", cre[id].id);
                        }
                        if(cre[id].type == 1){
                            printf("Animal %d",cre[id].id);
                        }
                        printf(" moves to the south.\n");
                        }
                    break;
        }
 }

/* Checks if a random room is full, else check the surrounding rooms. If not, the creature moves there and performs any action liked */
void move_Creature(struct room *r, struct creature *cre, int id, int numbCrea, int cRoom, struct room *rooms){
    int rng = rand() % 4;
    if(rng == 0 || rng == 1|| rng == 2|| rng == 3) {
        if(r->north != NULL && checkifFull(r[0].north->roomID, cre, numbCrea) != -1){
            move_Final(r,cre,id,numbCrea,'n',cRoom, rooms);
        } else if(r->east != NULL && checkifFull(r[0].east->roomID, cre, numbCrea) != -1){
            move_Final(r,cre,id,numbCrea,'e',cRoom, rooms);
        } else if(r->west != NULL && checkifFull(r[0].west->roomID, cre, numbCrea) != -1){
            move_Final(r,cre,id,numbCrea,'w',cRoom, rooms);
        }else if(r->south != NULL && checkifFull(r[0].south->roomID, cre, numbCrea) != -1){
            move_Final(r,cre,id,numbCrea,'s',cRoom, rooms);
        }else{
            /* Kill Creature, no adjacent rooms to move - resulting end */
            printf("Creature %d drills a hole in the ceiling and disappears from the simulation.\n" ,cre[0].id);
            cre[0].id = -1;
        }
    }
}

/*Print out creatures in that room */
void print_Creatures(int roomNum, struct creature *cr, int s){
    printf("contains: \n");
    printf("PC\n");
    for(int f = 0; f < s; f++){
        if(cr[f].type >=0 && cr[f].location->roomID == roomNum){
            if(cr[f].type == 1){
                printf("Animal %d \n", cr[f].id);
            }else if(cr[f].type == 2){
                printf("NPC %d \n", cr[f].id);
            }
        }
    }
}

/* shows information about neighbouring rooms */
void print_Room( struct room *r){
    printf("You are in room %d, it`s %s \n", r->roomID, getState(r->state));
    if(r->north){
        printf("neighbours to north is room# %d. \n",r->north->roomID);
    }
    if(r->east){
        printf("neighbours to east is room# %d. \n", r->east->roomID);
    }
    if(r->west){
        printf("neighbours to west is room# %d. \n", r->west->roomID);
    }
    if(r->south){
        printf("neighbours to south is room# %d. \n", r->south->roomID);
    }
}


int main(){
    int numRooms;
    int roomLoc;
    int currentRoom;
    int numCreatures;
    char command[15];

/* Gets number of rooms and input for each room from user and puts it in array of room structs */
    printf("Enter number of rooms\n");
    numRooms = getInput(1,100);

    struct room *rooms = (struct room*)malloc(sizeof(struct room)*numRooms);
    for(int k = 0; k < numRooms; k++){
        rooms[k].roomID = k;
    }
    printf("For each Room, Enter 5 numbers, state of the current room and number for neighbours by the #Ids \n");
    printf("state clean =0, half-dirty =1, dirty =2 and no neighbors =-1 \n");
    printf("e.g state, north, east, west, south (ex 0 -1 1 -1 2) | number space number | style \n");
    for(int i = 0; i < numRooms; ++i){
        int n,s,e,w;
        scanf("%d %d %d %d %d",&rooms[i].state,&n,&s,&e,&w);
        if( n >= numRooms){
            n = -1;
        }
        if( s >= numRooms){
            s = -1;
        }
        if( e >= numRooms){
            e = -1;
        }
        if( w >= numRooms){
            w = -1;
        }
        createRoom(rooms,i,numRooms,n,s,e,w);
    }

/* Gets number of creatures and creates creature array and take input from user and fills array */
    printf("Enter number of creatures: \n");
    numCreatures = getInput(1,100);
    struct creature *creatures = (struct creature *)malloc(sizeof(struct creature)*numCreatures);
    printf("For each creature, Enter 2 numbers, creature type and the room number\n");
    printf("PC = 0, animals = 1, NPC = 2 and #Room \n");
    printf("e.g creaturetype, #room (ex 0 1) | number space number | style \n");
    for(int j = 0; j < numCreatures; j++){
        scanf("%d %d",&creatures[j].type,&roomLoc);
        creatures[j].location = getRoom(roomLoc,rooms,numRooms);
        creatures[j].id = j;
    }

    currentRoom = startRoom(creatures,numRooms);            /* Sets the start room for the PC */
    printf("Enter a command (look, n, w, e, s, creatureID:action, help,  or exit)\n");
    scanf("%s", command);
    while (strcmp(command,"exit") != 0){
        if(strcmp(command,"n") == 0 || strcmp(command,"north") == 0){
            int nextRoom;
            nextRoom = PCmove('n',&rooms[currentRoom]);
            if(nextRoom != -1){
                currentRoom = nextRoom;
            }
        } else if(strcmp(command,"e") == 0 || strcmp(command,"east") == 0){
            int nextRoom;
            nextRoom = PCmove('e',&rooms[currentRoom]);
            if(nextRoom != -1){
                currentRoom = nextRoom;
            }
        } else if(strcmp(command,"w") == 0 ||strcmp(command,"west") == 0){
            int nextRoom;
            nextRoom = PCmove('w',&rooms[currentRoom]);
            if(nextRoom != -1){
                currentRoom = nextRoom;
            }
        } else if(strcmp(command,"s") == 0 ||strcmp(command,"south") == 0){
            int nextRoom;
            nextRoom = PCmove('s', &rooms[currentRoom]);
            if (nextRoom != -1) {
                currentRoom = nextRoom;
            }
        } else if(strcmp(command,"look") == 0){
            print_Room(&rooms[currentRoom]);
            print_Creatures(currentRoom,creatures,numCreatures);
        } else if(strcmp(command,"dirty") == 0){
            dirtyRoom(&rooms[currentRoom],creatures,numCreatures, currentRoom,rooms);
        } else if(strcmp(command,"clean") == 0) {
            cleanRoom(&rooms[currentRoom], creatures, numCreatures, currentRoom, rooms);
        } else if (strcmp(command,"help") == 0){
            printf("All the Commands are in lowercase. \n");
            printf("look - Shows information of the current room. \n");
            printf("help - Shows available commands. \n");
            printf("respect - Shows your current respect. \n");
            printf("exit - Quit the Game. \n");
            printf("clean - creature ID#:clean eg 1:clean, Clean`s the room, animals like clean rooms. \n");
            printf("dirty - creature ID#:dirty eg 2:dirty, Dirt`s the room, NPCs like dirty rooms. \n");
            printf("north/east/west/south - Use to move room north, east, west, south. \n");
            printf("creatureID#:action - All Creature can perform clean,dirty actions in rooms eg 1:dirty and 2:clean.\n "
                   "All creature can move north,east,west and south. And PC can forcfully move Creatures eg 2:north and 1:south. \n");
        }
        else if(strstr(command,":") != NULL){
            char* cmd = strtok(command,":");
            int intID = atoi(cmd);
            cmd = strtok(NULL,":");
            if(intID  >= numCreatures && creatures[intID].location->roomID != currentRoom){
                printf("Creature %d is not in this room. \n", intID);
            }else{
                if(strcmp(cmd,"north") == 0 || strcmp(cmd,"n") == 0){
                    if(rooms[currentRoom].north != NULL && checkifFull(rooms[currentRoom].north->roomID, creatures, numCreatures) != -1){
                        move_Final(&rooms[currentRoom],creatures,intID,numCreatures,'n',currentRoom, rooms);
                    }else {
                        printf("That room is full or isn't there. ");
                        dislike(creatures[intID].type,intID);
                    }
                }else if(strcmp(cmd,"east") == 0 || strcmp(cmd,"e") == 0){
                    if(rooms[currentRoom].east != NULL && checkifFull(rooms[currentRoom].east->roomID, creatures, numCreatures) != -1){
                        move_Final(&rooms[currentRoom],creatures,intID,numCreatures,'e',currentRoom, rooms);
                    }else {
                        printf("That room is full or isn't there. ");
                        dislike(creatures[intID].type,intID);
                    }
                }else if(strcmp(cmd,"west") == 0 || strcmp(cmd,"w") == 0){
                    if(rooms[currentRoom].west != NULL && checkifFull(rooms[currentRoom].west->roomID, creatures, numCreatures) != -1){
                        move_Final(&rooms[currentRoom],creatures,currentRoom,numCreatures,'w',currentRoom, rooms);
                    }else {
                        printf("That room is full or isn't there. ");
                        dislike(creatures[intID].type,intID);
                    }
                }else if(strcmp(cmd,"south") == 0 || strcmp(cmd,"s") == 0){
                    if(rooms[currentRoom].south != NULL && checkifFull(rooms[currentRoom].south->roomID, creatures, numCreatures) != -1){
                        move_Final(&rooms[currentRoom],creatures,intID,numCreatures,'s',currentRoom, rooms);
                    }else {
                        printf("That room is full or isn't there. ");
                        dislike(creatures[intID].type,intID);
                    }
                }else if(strcmp(cmd,"clean") == 0){
                    if(cleanRoom(&rooms[currentRoom],creatures,numCreatures,currentRoom, rooms) != -1){
                        if(creatures[intID].type == 2){
                            dislike(creatures[intID].type, intID);
                            dislike(creatures[intID].type, intID);
                            printf("NPC %d - cleaned the room. \n", intID);
                        } else if(creatures[intID].type ==1){
                            like(creatures[intID].type,intID);
                            like(creatures[intID].type,intID);
                            printf("Animal %d - cleaned the room. \n", intID);
                        }
                    }
                }else if (strcmp(cmd,"dirty") == 0){
                    if(dirtyRoom(&rooms[currentRoom],creatures,numCreatures,currentRoom, rooms) != -1){
                        if(creatures[intID].type == 2){
                            like(creatures[intID].type,intID);
                            like(creatures[intID].type,intID);
                            printf("NPC %d - dirtied the room. \n", intID);
                        } else if(creatures[intID].type ==1){
                            dislike(creatures[intID].type, intID);
                            dislike(creatures[intID].type, intID);
                            printf("Animal %d - dirtied the room. \n", intID);
                        }
                    }
                }
            }
        }else if(strcmp(command,"respect")==0){
            printf("Your respect is %d\n", respect);
        }else {
            printf("Command not found | All command are in lowercase| type 'help' for detail instructions. \n");
        }
            /* Some type of Score/respect to end game*/
        if(respect <= LOSE_Respect){
            printf("You lost! \n All the Creatures Hate You.\n Game Over! \n");
            break;
        }
        if(respect >= WIN_Respect){
            printf("You won! \n All the Creatures Like You.\n Game Over! \n");
            break;
        }
        printf("Enter a command (look, n, s, w, e, creatureID:action, help,  or exit.) \n");
        scanf("%s",command);
    }
        free(rooms);
        free(creatures);

    printf("GoodBye!\n");
    return 0;
}
