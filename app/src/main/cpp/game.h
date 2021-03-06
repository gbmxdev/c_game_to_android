/*
 ============================================================================
 Name        : game.c
 Author      : George Million
 Version     : 1.0
 Copyright   :  copyrighted 2021
 Description : randomly genorated 10x10 dungeon crawling game.
 Features: four playable classes, plus a randomly genorated one of your choice, usable items , dynamic traps,
 	 	 	 six character attributes, active combat,

 Bugs//todo: there's a bug in the encryption causing the first line to be corrupted
 	 	 need to unlink / delete .tmp file
 ============================================================================
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "class.h"
#include <string.h>
#include <time.h>

//items database
int itemdb_size=0;
struct item db[255];



//functions
std::string trap(int * hpp);
std::string item(struct item invent[100]);
std::string step(int * hpp, struct item  play[100]);
std::string gen_char(struct class_ *toon);
std::string combat(int * hpp, int monster_hp, struct class_ * player);
std::string itemdb( struct item db[255]);//needs work
std::string use(struct class_ * player);
std::string potion( int * trait, int buff);

std::string main1(void) {
    std::string output;
	//10x10 map
	int map[10][10];
	int i=0, e = 0;
	time_t t;
    output.append("This is a test.");
	//itemdb(db);//load items

	srand((unsigned) time(&t));
	//build a board with randomly assigned "monster" or value
	output.append("Welcome to dungeon hack!\nCommands: i[inventory],x[exit],u[use]\n");
	for(i=0;i<10;++i){
		for(e=0;e<10;++e){
			//squares have a fifty fifty chance to have a random monster value on them
			if (rand() % 100 > 50 ){
				map[e][i]= rand() % 100;//monsters hp up to 100
#if defined DEBUG_STATE
				output.append("%d",map[e][i]);
#endif
			} else {
				map[e][i]= 0;
			}
		}
	}
	output.append("Generated 10x10 map.\n");
#if defined DEBUG_STATE
	output.append("\n");
#endif
	struct class_ player;
	gen_char(&player);//genorate player and class
#if defined DEBUG_STATE
	output.append("Your hp is %d \n",player.hp);
	int x=0;
#endif
	int c = 0,in=0;
	int health=player.hp;
	//main game loop
	i=0, e = 0; //set starting position
#if defined DEBUG_STATE
	output.append("Cheats: t[test found items],l[list item db]\n");
#endif
	output.append("Use the w,a,s,d keys to travel.\n"); //turn off input loop for game loop
	{//while (1){
		c = getchar();
		//each movement has a 30 percent chance of hitting a trap
		switch(c){
		//move in grid
		//call combat check
		case 'a':
			e++;
			output.append("You have went left[w,a,s,d].\n");
			if(map[e][i]>0){
				combat(&health,map[e][i],&player);
			} else step(&health,player.inventory);
			break;
		case 's':
			i--;
			output.append("You have gone back[w,a,s,d].\n");
			if(map[e][i]>0){
				combat(&health,map[e][i],&player);
			}else step(&health,player.inventory);
			break;
		case 'd':
			e--;
			output.append("You have gone right[w,a,s,d].\n");
			if(map[e][i]>0){
				combat(&health,map[e][i],&player);
			}else step(&health,player.inventory);
			break;
		case 'w':
			i++;
			output.append("You have gone forward[w,a,s,d].\n");
			if(map[e][i]>0){
				combat(&health,map[e][i],&player);
			}else step(&health,player.inventory);
			break;
		case 'i':
			//list inventory
			while (player.inventory[in].amount=='1') {
				output.append("%d: %s\n");
				output.append(reinterpret_cast<const char *>(in));
                output.append(player.inventory[in].type);
				in++;
			}
			in=0;
			break;
		case 'x':
			exit(0);
		case 'u':
			use(&player);//use item in inventory
			break;
#if defined DEBUG_STATE
		case 't':
			item(player.inventory);
			break;
		case 'l':
			while (x<=itemdb_size){
				output.append("%s,%s,%d\n",db[x].trait,db[x].type,db[x].amount);
				x++;
			}
			x=0;
			break;
#endif
		default:
			output.append("[w,a,s,d]\n");
			//exit(0);
		}
	}

	return output;
}
std::string trap(int * hpp){
    std::string output;
	time_t t;
	srand((unsigned) time(&t));
	int hp = *hpp,dmg=0;
	dmg = rand() % 10;
	hp = hp - dmg;
	output.append("You have stepped on a trap, %d damage.\n");
	output.append(reinterpret_cast<const char *>(dmg));
}//just apply trap dmg
std::string item(struct item invent[100]){
    std::string output;
	time_t t;
	int i=0;
	srand((unsigned) time(&t));
	int item=rand() % itemdb_size;
	while (invent[i].amount=='1'){
		i++;
	}//count items in items db
	strcpy(invent[i].trait,db[item].trait);
	strcpy(invent[i].type,db[item].type);
	invent[i].amount=db[item].amount;//load item into db
	output.append("You have found a  %s.\n");
	output.append(invent[i].type );
#if defined DEBUG_STATE
	output.append("%s\n%d\n",invent[i].trait,i);
#endif
	return output;
}
std::string step(int * hpp, struct item play[100]){
    std::string output;
	time_t t;
	srand((unsigned) time(&t));
	if (rand() % 100 < 30) trap(hpp);//30% to hit trap
	//else if (rand() % 100 > 90) item(play);//10% chance to get item
	return output;
}
std::string gen_char(struct class_ *toon){
    std::string output;
	time_t t;
	int i=0, n;
	char num=-1;
	srand((unsigned) time(&t));
	output.append("Generating you player.\n");//start off as randomly generated class
	toon->Dexterity=rand() % 20 ;
	toon->Stamina=100+rand() % 20 ;
	toon->Wisdom=rand() % 20 ;
	toon->Strength=rand() % 20 ;
	toon->Intelligence=rand() % 20 ;
	toon->Agility=rand() % 20 ;
	toon->hp=100+rand() % 20 ;
	toon->mana=100+rand() % 20 ;
#if defined DEBUG_STATE
	output.append("Your hp is %d \n",toon->hp);
#endif
	output.append("What would you like your class to be called(Presets: mage,fighter,healer,rouge)?\n");
	fgets(toon->clas, 25, stdin);
	i=0;
   // while (toon->clas[i] != '\n' )//sanitize input to lower case letters only
    {//turned off to test portability
		n = sscanf(&toon->clas[i], "%[a-z]", &num);
		if (n != 1) {
			output.append("Error! You can only enter lower case letters:\n");
			fgets(toon->clas, 25, stdin);
			i=-1;
			num = -1;

		}
		i++;
    }
    i=0;
	{//while(1){//chop the new line off the clas char
		//turned off for portability
		if (toon->clas[i]==10){
			toon->clas[i]=0;
			//break;
		} else i++;
	}
	output.append("What would you like your character to be called?\n");
	fgets(toon->name, 25, stdin);
	i=0;
   // while (toon->name[i] != '\n' )//sanitize input to lower case letters only
    {//turned off for portability
		n = sscanf(&toon->name[i], "%[a-z]", &num);
		if (n != 1) {
			output.append("Error! You can only enter lower case letters:\n");
			fgets(toon->clas, 25, stdin);
			i=-1;
			num = -1;

		}
		i++;
    }
    i=0;
	load_clas(toon);//apply class template to generated class
	return output;
}
std::string combat(int * hpp, int monster_hp, struct class_ * player){
    std::string output;
	time_t t;
	int hit_dmg=0, hp = *hpp, heal=0;
	srand((unsigned) time(&t));
	while(monster_hp>0){//combat loop until monster dead
		if (rand() % 100 > 50){
				//monster hits you
			if(player->Dexterity < rand() % 20){//use dex to determine if you avoid an attack
				hit_dmg = rand() % 10 ;
				hp = hp - hit_dmg;
				output.append("You have been hit for %d.\n");
				output.append(reinterpret_cast<const char *>(hit_dmg));
				hit_dmg=0;
			} else {
				output.append("You jump out of the way, avoiding damage.\n");
			}
			if(hp<=0){//check players hp & exit if dead
				output.append("You have died.\n");
				exit(0);
				break;
			} else {
				output.append("Health:%d\n");
				output.append(reinterpret_cast<const char *>(hp));
			}
		} else {
			//you hit monster (or heal yourself)
			if(player->Wisdom < rand()%20){
				player->Stamina = player->Stamina - 10;
				if (player->Stamina <= 0 ) hit_dmg = player->Strength * rand() % 5 ;
					else hit_dmg = player->Strength * rand() % 10 ;
				// if you're out of stamina you do half damage
				monster_hp = monster_hp - hit_dmg;
				output.append("You have hit the monster for %d.\n");
				output.append(reinterpret_cast<const char *>(hit_dmg));
				hit_dmg=0;
			} else {
				heal = rand() % 10;
				output.append("The gods have favor on you, healing you for %d.\n");
				output.append(reinterpret_cast<const char *>(heal));
				player->hp = player->hp + heal;
			}
		}
		//cast a spell
		if(player->Intelligence>rand() % 20 ){
			if (player->mana > 0){
				hit_dmg = rand() % 10 ;
				monster_hp = monster_hp - hit_dmg;
				output.append("Your spell  has hit the monster for %d.\n");
				output.append(reinterpret_cast<const char *>(hit_dmg));
				hit_dmg=0;
				player->mana = player->mana - 10;
			}

		}
		if (player->Agility > rand ()%20){//check agil for chance of additional attack
			output.append("You dodge a blow and are granted an additional attack.\n");

			//hit_dmg = player->Strength * rand() % 10 ;//i don't know why this is producing a negative number
			hit_dmg = player->Strength;
			monster_hp = monster_hp - hit_dmg;
			output.append("You have hit the monster for %d.\n");
			output.append(reinterpret_cast<const char *>(hit_dmg));
#if defined DEBUG_STATE
			output.append("%d str %d monster_hp %d hit_dmg\n",player->Strength, monster_hp, hit_dmg);
#endif
			hit_dmg=0;
		}
		if(monster_hp<=0){//you have killed the monster, reset player
			output.append("The monster is dead.\n");
			player->mana = 100;
			player->Stamina = 100;
			player->hp=100;
		}
	}
	return output;
}
std::string itemdb( struct item db[255])
{
    std::string output;
	int i=0,c,comma=0,letter=0;
    char* fname = "item.db";//data base must be in comma separated value list format (may encrypt or encode to prevent editing)
    FILE * fp= fopen(fname, "r");
    if(!fp) {
        perror("Failed to load item db.\n");
        exit(1);
    }

    while ((c = fgetc(fp)) != EOF) { // loop until end of file
       if (c != ','){
#if defined DEBUG_STATE
    	  putchar(c);
#endif//load item db
    	   if (comma == 0 ) {
    		   db[i].type[letter]=c;
    		   letter++;
    	   } else if (comma == 1){
    		   db[i].trait[letter]=c;
    		   letter++;
    	   } else if (comma == 2){
    		   db[i].amount=c;
    	   }
       }  else {
    	   letter=0;
    	   comma++;
#if defined DEBUG_STATE
    	   putchar(c); //load item db
#endif
       }
       if (c == '\n' ) {
    	   comma=0;//forgot this little guy
    	   i++;
       }
    }
#if defined DEBUG_STATE
    output.append("%s",db[i].trait);
    output.append("%s",db[i].type);
    output.append("itemdb_size %d\n",i);
#endif
    itemdb_size=i;
    if (ferror(fp)) {
#if defined DEBUG_STATE
        puts("I/O error when loading item db.\n");
#endif
    } else if (feof(fp)) {
#if defined DEBUG_STATE
        puts("Item db loaded.\n");
#endif
    }
    fclose(fp);
    return output;
}
 std::string use(struct class_ * player){
    std::string output;
	int i=0,c=0;
	time_t t;
	//use item and call function for right item ID
	srand((unsigned) time(&t));
	output.append("What inventory item do you wish to use?\n");
	c = getc(stdin);
	c = getchar();// i don't know why both of these lines are need but it wont grab a char if they're not there
	c = c-48;//convert from ascii to decimal
	int item_type = player->inventory[c].amount - 48;

#if defined DEBUG_STATE
	output.append("%d - 48 = %d",player->inventory[c].amount,item_type);
#endif
	if(item_type == 1) {
		//item type one is potions
		i= rand() % 3;
		char lhs[255];
		strcpy(lhs,player->inventory[c].trait);
		char rhs[255];
		strcpy(rhs,(char *)"Agility");
		if( strcmp( (char *)&lhs, (char *)&rhs ) == 0){
			potion(&player->Agility,i);
		}
		strcpy(rhs,(char *)"Dexterity");
		if( strcmp( (char *)&lhs, (char *)&rhs ) == 0){
			potion(&player->Dexterity,i);
		}
		strcpy(rhs,(char *)"Stamina");
		if( strcmp((char *) &lhs,(char *) &rhs ) == 0){
			potion(&player->Stamina,i);
		}
		strcpy(rhs,(char *)"Wisdom");
		if( strcmp((char *) &lhs,(char *) &rhs ) == 0){
			potion(&player->Wisdom,i);
		}
		strcpy(rhs,(char *)"Strength");
		if( strcmp( (char *)&lhs,(char *) &rhs ) == 0){
			potion(&player->Strength,i);
		}
		strcpy(rhs,(char *)"Intelligence");
		if( strcmp((char *) &lhs, (char *)&rhs ) == 0){
			potion(&player->Intelligence,i);
		}
		output.append("You use a %s, which increases your %s by %d points.\n");
		output.append(player->inventory[c].type );
        output.append(player->inventory[c].trait);
        output.append(reinterpret_cast<const char *>(i));
		//remove item from inventory
		player->inventory[c].trait[0]='\0';
		player->inventory[c].type[0]='\0';

	}
	return output;
}

std::string potion( int * trait, int buff){
    std::string output;
	//apply potion
	trait = trait + buff;
	return output;
}