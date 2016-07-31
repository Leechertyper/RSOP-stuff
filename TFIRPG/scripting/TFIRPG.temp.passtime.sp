#pragma newdecls required
#pragma semicolon 1

#include <sourcemod>
#include <events>
#include <tf2>
/**#define <dinosaur>
                        /|/|/|\\|\|                                       
                      |  | | | | | |\                                   
                    | | |  | || || | |\                                  
                   /  | | |  || || ||||                                
                   || | | |  | | |  | |\                                
                 |  | | |  || ||  | | | \                                
                /  |  | |  ||  | || | ||\                               
                | | | | | || |  ||   | | |                              
                | | |    | | || |  | | |||\                             
               || |  | | | |  | || | | ||| \                           
              ||  |    |   | | ||| |     | |                            
              ||| | | |  | | |  || | | ||  |\                           
              | | | | |  |   | || |||| ||| |\                           
             /|  |  | | |  |  | | | || | ||  |                            
             |||| | |__ |__| |__|   || | || ||\                         
            |_| _ --,   ,.. ,   ,.... _|_|_ || |\                       
            /,   ,..,    ,.,    ,.. ,   ,..-|_\| |                       
         _ /.,    ,.,    ,,     ,...   ,...,   .\\                        
      _ /  ,..,    ,     ,      ,.,    ,.,    _ ...\\\\                 
    _/       ',,   ,     ,       ,     .       \  .   .\\\\             
  /     .       _--                       -\    \  .   ..  \\\\\           
 /    O .   __ /    _/                 ____|    \       .       \\\\\\\  
 /      |---  /    /---__________------    |    |---____            |  \  
/    ,; /     |    |                       |nn  |       ---____     |  |  
|,,,','/      /nn  |                                            ---|   |   
 ;,,;/                                                     _______/   /   
                                                   --------_________/  

**/
/**#define <swastika>
        _
       / /\
      / / /
     / / /   _
    /_/ /   / /\
    \ \ \  / /  \
     \ \ \/ / /\ \
  _   \ \ \/ /\ \ \
/_/\   \_\  /  \ \ \
\ \ \  / /  \   \_\/  
 \ \ \/ / /\ \ 
  \ \ \/ /\ \ \
   \ \  /  \ \ \
    \_\/   / / /
          / / /
         /_/ /
         \_\/
**/	

#define AUTHOR "Leechertyper"
#define PLUGIN_VERSION "0.0.1"
#define URL "https://redsun.tf"
public Plugin myinfo = {
	name = "TF2 PassTimeRPG",
	author = AUTHOR,
	description = "Score the Jack to gain various powers.",
	version = PLUGIN_VERSION,
	url = URL
}
#define Duration TFCondDuration_Infinite
#define AmmountOfPowers 10 //change this for a change to the number of powers
enum Powers //add powers here, use https://wiki.teamfortress.com/w/index.php?title=Cheats&redirect=no#addcond
{
	InvalidPower, //This does not count basically a throw error power
	MiniCritOnKill = TFCond_MiniCritOnKill,
	Regeneration = TFCond_MegaHeal,
	SmallBlastResist = TFCond_SmallBlastResist,
	SmallFireResist = TFCond_SmallFireResist,
	SmallBulletResist = TFCond_SmallBulletResist,
	PreventDeath = TFCond_PreventDeath,
	SpeedBoost = TFCond_HalloweenSpeedBoost,
	MultiJump = TFCond_CritHype,
	DodgeChance = TFCond_DodgeChance,
	FriendlyTelepathic = TFCond_SpawnOutline,
};
Powers IntToPower(int rng){
	switch(rng){
		case 0:
			return MiniCritOnKill;
		case 1:
			return Regeneration;
		case 2:
			return SmallBlastResist;
		case 3:
			return SmallFireResist;
		case 4:
			return SmallBulletResist;
		case 5:
			return PreventDeath;
		case 6:
			return SpeedBoost;
		case 7:
			return MultiJump;
		case 8:
			return DodgeChance;
		case 9:
			return FriendlyTelepathic;
		default:
			return InvalidPower;
	}
}
void addCondition(int client, int power){//makes it easier to add conditions
	TF2_AddCondition(client,view_as<TFCond>(IntToPower(power)), Duration);
}
void removeCondition(int client, int power){
	TF2_RemoveCondition(client, view_as<TFCond>(IntToPower(power)));
}
int Capped_Powers[MAXPLAYERS+1][AmmountOfPowers];
int Last_Power[MAXPLAYERS+1];
int GenIntNoRepeats(int client){//Absolutely unsafe but here for testing purposes and no further than one lvl
//Creates an integer with no repeats using Capped_Powers to check them
	int temporary_int;
	temporary_int = GetRandomInt(0,AmmountOfPowers-1);
	if (Capped_Powers[client][temporary_int] == 1){
		return GenIntNoRepeats(client);
	}
	return temporary_int;
}
bool CheckFree(int client){
	int howManyPowers = 0;
	for(int i = 0; i < AmmountOfPowers; i++){
		if (Capped_Powers[client][i] != 0){
			howManyPowers++;
		}
	}
	if(howManyPowers == AmmountOfPowers){
		return false;
	}
	else{
		return true;
	}
}
public void OnPluginStart(){
	HookEvent("pass_get", Generate_Power);
	HookEvent("post_inventory_application", Give_Power); //hooking respawns and inventory changes
	HookEvent("teamplay_round_start", reset_Game); //hooking round starts
	HookEvent("player_initial_spawn", First_Message);
	HookEvent("player_spawn", RespawnMessage);
	HookEvent("pass_score", Cap_Power);
	HookEvent("pass_pass_caught", Passed_Jack);
	HookEvent("pass_ball_stolen", Jack_Stolen);
	HookEvent("pass_free", Free_Ball);
}
public void First_Message(Event event, char[] name, bool dontBroadcast){
	int client = GetClientOfUserId(event.GetInt("userid"));
	PrintCenterText(client, "Welcome to TF2 PT_RPG.");
	CreateTimer(3.0, NextMessage, client);// i am here
}
public Action NextMessage(Handle timer, int client){
	PrintCenterText(client, "Score the Jack to recieve a power.");
}
public void RespawnMessage(Event event, char[] name, bool dontBroadcast){
	int client = GetClientOfUserId(event.GetInt("userid"));
	PrintCenterText(client, "Touch the resupply locker to recieve your capped powers.");
}
public void reset_Game(Event event, char[] name, bool dontBroadcast){ //resets the Powers for the whole game
	for (int j = 0; j < MAXPLAYERS+1; j++){
		for (int t = 0; t < AmmountOfPowers; t++){
			Capped_Powers[j][t] = 0;
		}
	}
}
public void Generate_Power(Event event, char[] name, bool dontBroadcast){
	int rng;
	int client;
	bool hasFree;
	client = event.GetInt("owner")+1;
	if(Last_Power[client] != -1){
		hasFree = CheckFree(client);
		if(hasFree){
			rng = GenIntNoRepeats(client);
			PrintCenterText(client, "You are Currently Carrying the Jack and receiving a Temporary power");
			addCondition(client, rng);
			Last_Power[client] = rng;
		}			
		else{
			PrintCenterText(client, "You have no more space for new powers!");
		}
	}
	else{
		PrintToChat(client, "You are Currently Carrying the Jack");
		addCondition(client, Last_Power[client]);
	}
}
public void Cap_Power(Event event, char[] name, bool dontBroadcast){
	int client = event.GetInt("player") + 1;
	Capped_Powers[client][Last_Power[client]] = 1;
}
public void Give_Power(Event event,  char[] name, bool dontBroadcast){//regives Capped_Powers as long as they touch the resupply locker
	int client = GetClientOfUserId(event.GetInt("userid"));
	for(int i = 0; i < AmmountOfPowers; i++){
		if (Capped_Powers[client][i] != 0){
			addCondition(client, i);
		}
	}
}
public void Passed_Jack(Event event, char[] name, bool dontBroadcast){
	int Passer = event.GetInt("passer")+1;
	int catcher = event.GetInt("catcher")+1;
	int rng;
	bool hasFree;
	if(Last_Power[catcher] != -1){
		hasFree = CheckFree(catcher);
		if(hasFree){
			rng = GenIntNoRepeats(catcher);
			PrintCenterText(catcher, "You are Currently Carrying the Jack and receiving a Temporary power");
			addCondition(catcher, rng);
			Last_Power[catcher] = rng;
			removeCondition(Passer, Last_Power[Passer]);
			Last_Power[Passer] = -1;
		}			
		else{
			PrintCenterText(catcher, "You have no more space for new powers!");
			removeCondition(Passer, Last_Power[Passer]);
			Last_Power[Passer] = -1;
		}
	}
	else{
		PrintToChat(catcher, "You are Currently Carrying the Jack");
		addCondition(catcher, Last_Power[catcher]);
		removeCondition(Passer, Last_Power[Passer]);
		Last_Power[Passer] = -1;
	}
}
public void Jack_Stolen(Event event, char[] name, bool dontBroadcast){
	int owner = event.GetInt("owner")+1;
	int attacker = event.GetInt("attacker")+1;
	int rng;
	bool hasFree;
	if(Last_Power[attacker] != -1){
		hasFree = CheckFree(attacker);
		if(hasFree){
			rng = GenIntNoRepeats(attacker);
			PrintCenterText(attacker, "You are Currently Carrying the Jack and receiving a Temporary power");
			addCondition(attacker, rng);
			Last_Power[attacker] = rng;
			removeCondition(owner, Last_Power[owner]);
			Last_Power[owner] = -1;
		}			
		else{
			PrintCenterText(attacker, "You have no more space for new powers!");
			removeCondition(owner, Last_Power[owner]);
			Last_Power[owner] = -1;
		}
	}
	else{
		PrintToChat(attacker, "You are Currently Carrying the Jack");
		addCondition(attacker, Last_Power[attacker]);
		removeCondition(owner, Last_Power[owner]);
		Last_Power[owner] = -1;
	}
}
public void Free_Ball(Event event, char[] name, bool dontBroadcast){
	int owner = event.GetInt("owner")+1;
	if(Capped_Powers[owner][Last_Power[owner]] != 1){
		removeCondition(owner, Last_Power[owner]);
	}
}