#pragma once

struct IRCerror {

	static constexpr const char* ERR_NOSUCHNICK = "401";		// No such nick/channel
	static constexpr const char* ERR_NOSUCHSERVER = "402";		// No such server
	static constexpr const char* ERR_NOSUCHCHANNEL = "403";		// No such channel
	static constexpr const char* ERR_CANNOTSENDTOCHAN = "404";	// Cannot send to channel
	static constexpr const char* ERR_TOOMANYCHANNELS = "405";	// Too many channels
	static constexpr const char* ERR_WASNOSUCHNICK = "406";		// There was no such nick
	static constexpr const char* ERR_TOOMANYTARGETS = "407";	// Too many targets
	static constexpr const char* ERR_NOORIGIN = "409";			// No origin specified
	static constexpr const char* ERR_NORECIPIENT = "411";		// No recipient given
	static constexpr const char* ERR_NOTEXTTOSEND = "412";		// No text to send
	static constexpr const char* ERR_NOTOPLEVEL = "413";		// No top level domain
	static constexpr const char* ERR_WILDTOPLEVEL = "414";		// Wild card top level domain
	static constexpr const char* ERR_BADMASK = "415";			// Invalid mask
	static constexpr const char* ERR_UNKNOWNCOMMAND = "421";	// Unknown command
	static constexpr const char* ERR_NOMOTD = "422";			// MOTD file missing
	static constexpr const char* ERR_NOADMININFO = "423";		// No admin information available
	static constexpr const char* ERR_FILEERROR = "424";			// File error
	static constexpr const char* ERR_NONICKNAMEGIVEN = "431";	// No nickname given
	static constexpr const char* ERR_ERRONEUSNICKNAME = "432";	// Erroneous nickname
	static constexpr const char* ERR_NICKNAMEINUSE = "433";		// Nickname is already in use
	static constexpr const char* ERR_NICKCOLLISION = "436";		// Nickname collision
	static constexpr const char* ERR_UNAVAILRESOURCE = "437";	// Nick/channel is temporarily unavailable
	static constexpr const char* ERR_USERNOTINCHANNEL = "441";	// They aren't on that channel
	static constexpr const char* ERR_NOTONCHANNEL = "442";		// You're not on that channel
	static constexpr const char* ERR_USERONCHANNEL = "443";		// User is already on channel
	static constexpr const char* ERR_NOLOGIN = "444";			// User not logged in
	static constexpr const char* ERR_SUMMONDISABLED = "445";	// SUMMON command disabled
	static constexpr const char* ERR_USERSDISABLED = "446";		// USERS command disabled
	static constexpr const char* ERR_NOTREGISTERED = "451";		// You have not registered
	static constexpr const char* ERR_NEEDMOREPARAMS = "461";	// Not enough parameters
	static constexpr const char* ERR_ALREADYREGISTRED = "462";	// Already registered
	static constexpr const char* ERR_NOPERMFORHOST = "463";		// No permission for operation
	static constexpr const char* ERR_PASSWDMISMATCH = "464";	// Password mismatch
	static constexpr const char* ERR_YOUREBANNEDCREEP = "465";	// You're banned
	static constexpr const char* ERR_KEYSET = "467";			// Channel key already set
	static constexpr const char* ERR_CHANNELISFULL = "471";		// Channel is full
	static constexpr const char* ERR_UNKNOWNMODE = "472";		// Unknown mode
	static constexpr const char* ERR_INVITEONLYCHAN = "473";	// Invite-only channel
	static constexpr const char* ERR_BANNEDFROMCHAN = "474";	// Banned from channel
	static constexpr const char* ERR_BADCHANNELKEY = "475";		// Bad channel key
	static constexpr const char* ERR_BADCHANMASK = "476";		// Bad channel mask
	static constexpr const char* ERR_NOCHANMODES = "477";		// Channel modes can't be changed
	static constexpr const char* ERR_BANLISTFULL = "478";		// Ban list is full
	static constexpr const char* ERR_NOPRIVILEGES = "481";		// No privileges
	static constexpr const char* ERR_CHANOPRIVSNEEDED = "482";	// You're not channel operator
	static constexpr const char* ERR_CANTKILLSERVER = "483";	// Can't kill server
	static constexpr const char* ERR_RESTRICTED = "484";		// Restricted command
	static constexpr const char* ERR_UNIQOPPRIVSNEEDED = "485";	// You need uniqop privileges
	static constexpr const char* ERR_NOOPERHOST = "491";		// No O-line for your host
	static constexpr const char* ERR_UMODEUNKNOWNFLAG = "501";	// Unknown MODE flag
	static constexpr const char* ERR_USERSDONTMATCH = "502";	// Cannot change or set mode for others

};
