/**
 * @brief Creates md5hash for given file name
 * @remarks throws an error if file not found
 * 
 * @param filename 	file to hash
 * @param buffer	buffer to safe md5 hash (optimal size 33 [32chars + \0])
 * @param maxlen	maxlength of buffer
 * @return 			none
 */
native MD5File(const String:filename[], String:buffer[], maxlen);

/**
 * @brief Creates md5hash for given string
 * @remarks
 * 
 * @param filename 	string to hash
 * @param buffer	buffer to safe md5 hash (optimal size 33 [32chars + \0])
 * @param maxlen	maxlength of buffer
 * @return 			none
 */
native MD5String(const String:string[], String:buffer[], maxlen);