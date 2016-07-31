#if defined _itemsgame_included_
	#endinput
#endif
#define _itemsgame_included_

#define ITEMSGAME_RETURN_INDEX          0
#define ITEMSGAME_RETURN_CLASSNAME      1

/**
 * Returns an array containing all results fitting the search.
 *
 * @param strSearch         Search options. Example: "classname => tf_wearable_item, slot => misc". Can also be specific entries like: "drop_type => none".
 * @param iReturn           ITEMSGAME_RETURN_INDEX returns an array containing entindexes, ITEMSGAME_RETURN_CLASSNAME returns an array of classnames.
 * @return                  Handle with results. Needs to be closed with CloseHandle after use.
 */
native Handle:ItemsGameSearch(String:strSearch[], iReturn = ITEMSGAME_RETURN_INDEX);

/**
 * Returns specific information on an item index.
 *
 * @param iIndex            Item index you're interested in
 * @param strInfo           The specific piece of information you want. Examples: "classname", "slot", "drop_type", etc.. Pretty much anything in its item_game.txt entry.
 * @param strReturn         String variable to save the result to.
 * @param iMaxSize          The maximum size of above string.
 * @return                  Whether the data was successfully collected.
 */
native bool:ItemsGameInfo(iIndex, String:strInfo[], String:strReturn[], iMaxSize);

/**
 * Returns the same information as ItemsGameInfo, but uses an entity as input. (NOTE: not completed yet)
 *
 * @param iEntity           Entity you're interested in
 * @param strInfo           The specific piece of information you want. Examples: "classname", "slot", "drop_type", etc.. Pretty much anything in its item_game.txt entry.
 * @param strReturn         String variable to save the result to.
 * @param iMaxSize          The maximum size of above string.
 * @return                  Whether the data was successfully collected.
 */
//native Handle:ItemsGameEntityInfo(iEntity, String:strInfo[], String:strReturn[], iMaxSize);