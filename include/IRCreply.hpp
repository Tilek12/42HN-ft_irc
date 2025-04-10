#pragma once

struct IRCreply {

	static constexpr const char* RPL_WELCOME = "001";
	static constexpr const char* RPL_YOURHOST = "002";
	static constexpr const char* RPL_CREATED = "003";
	static constexpr const char* RPL_MYINFO = "004";
	static constexpr const char* RPL_ISUPPORT = "005";

	static constexpr const char* RPL_BOUNCE = "010";
	static constexpr const char* RPL_TRACELINK = "200";
	static constexpr const char* RPL_TRACECONNECTING = "201";
	static constexpr const char* RPL_TRACEHANDSHAKE = "202";
	static constexpr const char* RPL_TRACEUNKNOWN = "203";
	static constexpr const char* RPL_TRACEOPERATOR = "204";
	static constexpr const char* RPL_TRACEUSER = "205";
	static constexpr const char* RPL_TRACESERVER = "206";
	static constexpr const char* RPL_TRACESERVICE = "207";
	static constexpr const char* RPL_TRACENEWTYPE = "208";
	static constexpr const char* RPL_TRACECLASS = "209";

	static constexpr const char* RPL_STATSLINKINFO = "211";
	static constexpr const char* RPL_STATSCOMMANDS = "212";
	static constexpr const char* RPL_STATSCLINE = "213";
	static constexpr const char* RPL_STATSNLINE = "214";
	static constexpr const char* RPL_STATSILINE = "215";
	static constexpr const char* RPL_STATSKLINE = "216";
	static constexpr const char* RPL_STATSYLINE = "218";
	static constexpr const char* RPL_ENDOFSTATS = "219";

	static constexpr const char* RPL_UMODEIS = "221";
	static constexpr const char* RPL_SERVLIST = "234";
	static constexpr const char* RPL_SERVLISTEND = "235";

	static constexpr const char* RPL_STATSLLINE = "241";
	static constexpr const char* RPL_STATSUPTIME = "242";
	static constexpr const char* RPL_STATSOLINE = "243";
	static constexpr const char* RPL_STATSHLINE = "244";
	static constexpr const char* RPL_STATSSLINE = "245";
	static constexpr const char* RPL_STATSPING = "246";
	static constexpr const char* RPL_STATSBLINE = "247";

	static constexpr const char* RPL_LUSERCLIENT = "251";
	static constexpr const char* RPL_LUSEROP = "252";
	static constexpr const char* RPL_LUSERUNKNOWN = "253";
	static constexpr const char* RPL_LUSERCHANNELS = "254";
	static constexpr const char* RPL_LUSERME = "255";

	static constexpr const char* RPL_ADMINME = "256";
	static constexpr const char* RPL_ADMINLOC1 = "257";
	static constexpr const char* RPL_ADMINLOC2 = "258";
	static constexpr const char* RPL_ADMINEMAIL = "259";

	static constexpr const char* RPL_TRACELOG = "261";
	static constexpr const char* RPL_TRACEEND = "262";
	static constexpr const char* RPL_TRYAGAIN = "263";

	static constexpr const char* RPL_NONE = "300";
	static constexpr const char* RPL_AWAY = "301";
	static constexpr const char* RPL_USERHOST = "302";
	static constexpr const char* RPL_ISON = "303";
	static constexpr const char* RPL_UNAWAY = "305";
	static constexpr const char* RPL_NOWAWAY = "306";
	static constexpr const char* RPL_WHOISUSER = "311";
	static constexpr const char* RPL_WHOISSERVER = "312";
	static constexpr const char* RPL_WHOISOPERATOR = "313";
	static constexpr const char* RPL_WHOWASUSER = "314";
	static constexpr const char* RPL_ENDOFWHO = "315";
	static constexpr const char* RPL_WHOISIDLE = "317";
	static constexpr const char* RPL_ENDOFWHOIS = "318";
	static constexpr const char* RPL_WHOISCHANNELS = "319";

	static constexpr const char* RPL_LISTSTART = "321";
	static constexpr const char* RPL_LIST = "322";
	static constexpr const char* RPL_LISTEND = "323";
	static constexpr const char* RPL_CHANNELMODEIS = "324";
	static constexpr const char* RPL_UNIQOPIS = "325";
	static constexpr const char* RPL_NOTOPIC = "331";
	static constexpr const char* RPL_TOPIC = "332";
	static constexpr const char* RPL_INVITING = "341";
	static constexpr const char* RPL_SUMMONING = "342";

	static constexpr const char* RPL_VERSION = "351";
	static constexpr const char* RPL_WHOREPLY = "352";
	static constexpr const char* RPL_NAMREPLY = "353";
	static constexpr const char* RPL_LINKS = "364";
	static constexpr const char* RPL_ENDOFLINKS = "365";
	static constexpr const char* RPL_ENDOFNAMES = "366";
	static constexpr const char* RPL_BANLIST = "367";
	static constexpr const char* RPL_ENDOFBANLIST = "368";
	static constexpr const char* RPL_INFO = "371";
	static constexpr const char* RPL_MOTD = "372";
	static constexpr const char* RPL_ENDOFINFO = "374";
	static constexpr const char* RPL_MOTDSTART = "375";
	static constexpr const char* RPL_ENDOFMOTD = "376";
	static constexpr const char* RPL_YOUREOPER = "381";
	static constexpr const char* RPL_REHASHING = "382";
	static constexpr const char* RPL_TIME = "391";
	static constexpr const char* RPL_USERSSTART = "392";
	static constexpr const char* RPL_USERS = "393";
	static constexpr const char* RPL_ENDOFUSERS = "394";
	static constexpr const char* RPL_NOUSERS = "395";

};
