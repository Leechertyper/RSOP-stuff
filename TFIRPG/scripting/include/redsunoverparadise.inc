#if defined _redsunoverparadise_included
 #endinput
#endif
#define _redsunoverparadise_included

/**
 * Get the item in the clients specific slot or type
 *
 * @param client	Client index
 * @return			Equipped item identifier of client OR true or false depending on slot or type you are retrieving
 */
// returns item id (int)
native Loadout_GetAction(client);
native Loadout_GetChat(client);
native Loadout_GetTaunt(client);
native Loadout_GetDodgeball(client);
native Loadout_GetSpecial(client);
native Loadout_GetMedicCall(client);
native Loadout_GetBattlecry(client);
native Loadout_GetDonatorLevel(client);
native Loadout_GetSkin(const char[] class, client);
native Loadout_GetSkinQuality(const char[] class, client);

// returns boolean
native bool Loadout_HasPassive(client, item); // experimental, tohru test this you nigger

/**
 * Compares the client's rank between the inputted rank
 *
 * @param client	Client index
 * @param rank		Rank to compare
 * @return			Equipped item identifier of client
 */
native bool Loadout_IsClientRank(client, rank);

/**
 * Empties the client's action slot and deletes the item in that slot
 *
 * @param client	Client index
 * @noreturn
 */
native Loadout_EmptyActionSlot(client);

/**
 * Award the client with Battle Points
 *
 * @param client	Client index
 * @param coins		Battle Points to give to client
 * @noreturn
 */
native Loadout_GiveCash(client, coins);

/**
 * Award the client with an item with a broadcasted chat notification in-game
 *
 * @param client	Client index
 * @param itemid	Identifier of the item to give
 * @param itemname	The name of the item
 * @noreturn
 */
native Loadout_GiveItem(client, itemid, const String:itemname[]);

/**
 * Gets the chat vanity of a client, titles are self-explanatory
 *
 * @param client	Client index
 * @param buffer	The buffer string where the value is put in
 * @param size		Size of string (UNUSED)
 * @return			Data you are looking for
 */
native Loadout_ChatColor(client, const String:buffer[], size);
native Loadout_ChatIcon(client, const String:buffer[], size);
native Loadout_DonatorTitle(client, const String:buffer[], size);

/**
 * Bans the client by adding a row to the database and a message and stuff, yada yada
 *
 * @param client	Client index banned
 * @param banner	Client index banner
 * @param hours		How many hours does the ban last?
 * @param bantype		The type ban 
 * @param reason	The name of the item
 * @noreturn
 */
native Loadout_InsertBan(client, banner, hours, const String:bantype[], const String:reason[]);

/**
 * Returns a true or false using the chance of the donator's chance of keeping a token after use
 *
 * @param client	Client index
 * @return			true or false
 */
native bool Loadout_ShouldDeleteToken(client);

/**
 * Returns a float using the donator's perk of how long a token is extended
 *
 * @param client	Client index
 * @return			
 */
native Float:Loadout_TokenMultiplier(client);

/**
 * Returns a true or false based on whether the client is gameplay-banned or not
 *
 * @param client	Client index
 * @return			True if banned, false if not	
 */
native bool Loadout_GetGameplayBan(client);

/**
 * Flags the client index to be gameplay banned
 *
 * @param client	Client index
 * @noreturn
 */
native Loadout_SetGameplayBan(client);

/**
 * Returns a true or false based on whether the client is spray-banned or not
 *
 * @param client	Client index
 * @return			True if banned, false if not	
 */
native bool Loadout_GetSprayBan(client);

/**
 * Flags the client index to be spray banned
 *
 * @param client	Client index
 * @noreturn
 */
native Loadout_SetSprayBan(client);

/**
 * Gets the amount of Battle Points a client is carrying (prefetched)
 *
 * @param client	Client index
 * @return			Amount of Battle Points (coins) the client has
 */
native Loadout_GetCoins(iClient);

/**
 * Adds a number to/Selects a row of the user selected
 *
 * @param client	Client index, needs to be higher than REDSUN_NONE (-1)
 * @param amount	Increase by this much if adding
 * @param row		Database row of user to target
 * @noreturn/@return if selecting
 */
native Loadout_AddToGeneric(client, amount, const String:row[]);
native Loadout_SelectFromGeneric(client, const String:row[]);

/**
 * Adds a badge to the client
 *
 * @param client	Client index, needs to be higher than REDSUN_NONE (-1)
 * @param badge		Badge ID that is added
 * @param name		Badge name to display for everyone
 * @noreturn
 */
native Loadout_AwardBadge(client, badge, const String:name[]);


//
// BONUS FUNCTIONS
//
#define COLOR_ITEM		"\x07F25137"
#define COLOR_TEXT		"\x07F78C56"
#define COLOR_ERROR		"\x07B42C1E"
#define COLOR_HIGHLIGHT	"\x07CF7336"
#define COLOR_GENERIC	"\x07729E42"

#define ADMIN_NONE			0
#define ADMIN_FULL			1

#define REDSUN_NONE			-1
#define REDSUN_NORMAL		0
#define REDSUN_DEVELOPER	1
#define REDSUN_MODERATOR	2

stock SuperPrecacheSound(String:strSound[]) {
	decl String:strPath[PLATFORM_MAX_PATH];
	Format(strPath, sizeof(strPath), "sound/%s", strSound);
	
	PrecacheSound(strSound, true);
	AddFileToDownloadsTable(strPath);
}

stock IsValidClient(iClient) {
	if (iClient <= 0 || iClient > MaxClients) return false;
	if (!IsClientInGame(iClient)) return false;
	return true;
}

stock AdminStatus(iClient) {
	if (iClient <= 0) return ADMIN_FULL;
	if (!IsClientInGame(iClient)) return ADMIN_NONE;
	
	new AdminId:iAdmin = GetUserAdmin(iClient);
	if(iAdmin == INVALID_ADMIN_ID) {
		return ADMIN_NONE;
	}
	
	if (GetAdminFlag(iAdmin, Admin_Root)) return ADMIN_FULL;
	
	return ADMIN_NONE;
}

stock bool IsClassname(iEntity, String:strClassname[])
{
	if (iEntity <= 0) return false;
	if (!IsValidEdict(iEntity)) return false;
	
	decl String:strClassname2[32];
	GetEdictClassname(iEntity, strClassname2, sizeof(strClassname2));
	if (!StrEqual(strClassname, strClassname2, false)) return false;
	
	return true;
}

/* DO NOT EDIT BELOW THIS LINE */
public SharedPlugin:__pl_redsunoverparadise = 
{
	name = "redsunoverparadise",
	file = "redsunoverparadise_main.smx",
#if defined REQUIRE_PLUGIN
	required = 1,
#else
	required = 0,
#endif
};