//
// Created by Nadiia Maliarenko on 15.09.23.
//

#include "Kick.hpp"
#include "./../../client/Client.hpp"

Kick::Kick() : Cmd(){}
Kick::~Kick() {}
Kick::Kick(const Kick &k) {
	(void)&k;
}
Kick &Kick::operator=(const Kick &k) {
	(void)&k;
	return *this;
}

//Command: KICK
//   Parameters: <channel> <user> *( "," <user> ) [<comment>]

// "Servers MAY limit the number of target users per KICK command
// and silently drop targets if the number of targets exceeds the limit."

// - so no matter how many arguments we have, we will kick only first user xD

bool Kick::isMember(std::vector<Client *> members, std::string nick) const{
	for (std::vector<Client*>::iterator it = members.begin() ; it != members.end() ; ++it) {
		if ((*it)->getNickName() == nick)
			return true;
	}
	return false;
}

std::string cmdToStr(std::vector<std::string> cmd){
	std::string str;
	for (size_t i = 3; i < cmd.size(); ++i) {
		str.append(cmd[i]);
		if (i != cmd.size() - 1)
			str.append(" ");
	}
	return str;
}

void Kick::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	std::map<std::string, Channel *> &channels = serv->getChannels();

	if (cmd.size() < 3){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","KICK :Not enough parameters");
		return;
	} else if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED", ":You must authenticate with the server.");
		return;
	} else if (channels.find(cmd[1]) == channels.end()){
		serv->reply(&who, "ERR_NOSUCHCHANNEL", cmd[1] + " :No such channel");
		return;
	} else if (!channels[cmd[1]]->operatorRight(who)){
		serv->reply(&who, "ERR_CHANOPRIVSNEEDED", cmd[1] + " :You're not channel operator");
		return;
	} else if (!isMember(channels[cmd[1]]->getMembers(), cmd[2])){
		serv->reply(&who, "ERR_USERNOTINCHANNEL", cmd[2] + " " + cmd[1] +
			" :They aren't on that channel");
		return;
	} else if (!isMember(channels[cmd[1]]->getMembers(), who.getNickName())){
		serv->reply(&who, "ERR_NOTONCHANNEL", cmd[2] + " " + cmd[1] + " :You aren't on that channel");
		return;
	}

	std::string reason = cmdToStr(cmd);
	if (reason.find(':') != std::string::npos)
		reason = reason.substr(reason.find(':'));
	serv->reply(channels[cmd[1]]->getMembers(), "", "KICK " + cmd[1] + " " + cmd[2] + " " +
		!reason.empty() ? reason : ":No reason specified" );
	channels[cmd[1]]->deleteMembers(who);
}