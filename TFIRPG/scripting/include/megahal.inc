/**
 * vim: set ts=4 :
 * =============================================================================
 * MegaHAL Extension SourcePawn Include
 * Copyright (C) 2011 Afronanny.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */


funcenum MHReply
{
	public (const String:reply[])
};


/**
 * MH_Learn
 * 
 * Essentially the same as MH_DoReply
 * except MH_Learn will not reply to the input
 * 
 * @param input				The string that MegaHAL will store in its brain
 * 
 * @noreturn
 */
native MH_Learn(const String:input[]);


/**
 * MH_DoReply
 * 
 * Takes a string and generates a reply to it
 *
 * 
 * @param input				The string that MegaHAL will generate a reply to
 * 
 * @param ReplyFunction		The function that will be called once MegaHAL 
 * 							generates a reply
 * @noreturn
 */
native MH_DoReply(const String:input[], MHReply:ReplyFunction);

/**
 * MH_Save
 * 
 * Saves the current megahal brain
 *
 * @noreturn
 */
native MH_Save();

/**
 * Do not edit below this line!
 */
public Extension:__ext_megahal = 
{
	name = "MegaHAL Ext",
	file = "megahal.ext",
	autoload = 0,
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};