//
// Created by Nadiia Maliarenko on 14.09.23.
//

#include "Join.hpp"
#include "./../../client/Client.hpp"
#include <string>

Join::Join() : Cmd(){}

Join::~Join() {}

std::string vecToUsersStr(std::vector<Client *> clients){
	std::string str;
	for (size_t i = 0; i < clients.size(); ++i) {
		if (clients[i]->getModes().find('o') != clients[i]->getModes().end())
			str.append("@");
		str.append(clients[i]->getNickName());
		if (i != clients.size() - 1)
			str.append(" ");
	}
	return str;
}

std::string cmdToStr(std::vector<std::string> cmd){
	std::string str;
	for (size_t i = 1; i < cmd.size(); ++i) {
		str.append(cmd[i]);
		if (i != cmd.size() - 1)
			str.append(" ");
	}
	return str;
}

std::vector<std::string> split(std::string str, char delim){
	std::vector <std::string> vec;
	while (str.find(delim) != std::string::npos){
		vec.push_back(str.substr(0, str.find(delim)));
		str = str.substr(str.find(delim) + 1);
	}
	if (!str.empty())
		vec.push_back(str);
	return vec;
}

bool Join::validateChannelName(std::string channel) const {
	if (channel.size() > 1 && (channel[0] == '#' || channel[0] == '&')){
		for (std::string::const_iterator it = channel.begin(); it < channel.end(); ++it) {
			if (!isprint(*it) || *it < 33)
				return false;
		}
		return true;
	}
	return false;
}

void Join::joined(Client &who, std::string channel, std::map <std::string, Channel *> &channels) const {
	Server *serv = who.getServer();
	std::vector<Client *> members = channels[channel]->getMembers();

	serv->reply(members, "", "JOIN " + channel + " "
						  + who.getNickName() + " is joining the channel " + channel);

	if (channels[channel]->getTopic().empty())
		serv->reply(&who, "RPL_NOTOPIC", channel + " :No topic is set")
	else
		serv->reply(&who, "RPL_TOPIC", channel + " " + ":Welcome to the channel " +
							channels[channel]->getTopic()) + "!";
	serv->reply(&who, "RPL_NAMREPLY", "= " + channel + " " + vecToUsersStr(members));
	serv->reply(&who, "RPL_ENDOFNAMES", channel + " :End of user's list.");
}

void Join::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	if (cmd.size() < 2){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","JOIN :Not enough parameters");
		return;
	} else if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED", ":You must authenticate with the server.");
		return;
	}
	cmd = split(cmdToStr(cmd),',');
	std::vector<std::string> ch;
	std::map<std::string, Channel *> &channels = serv->getChannels();
	for (int i = 0; i < cmd.size(); ++i) {
		ch = split(cmd[i], ' ');
		if (ch.size() < 1){
			serv->reply(&who,"ERR_NEEDMOREPARAMS","JOIN :Not enough parameters");
			return;
		}
		if (!validateChannelName(ch[0])){
			serv->reply(&who,"ERR_BADCHANMASK",":Bad Channel Mask");
			return;
		}
		if (channels.find(ch[0]) == channels.end()){
			serv->addChannel(ch[0]);
		} else if ((!channels[ch[0]]->getKey().empty()) && (ch.size() != 2 || channels[ch[0]]->getKey() != ch[1])) {
			serv->reply(&who, "ERR_BADCHANNELKEY", ch[0] + " :Cannot join channel (+k)");
			return;
		} else if (channels[ch[0]]->getInviteOnly()){
			serv->reply(&who, "ERR_INVITEONLYCHAN", ch[0] + ":Cannot join channel (+i)");
			return;
		} else if (channels[ch[0]]->getMemberSize() == channels[ch[0]]->getUserLimit()){
			serv->reply(&who, "ERR_CHANNELISFULL", ch[0] + ":Cannot join channel (+l)");
			return;
		}
		channels[ch[0]]->addMember(who);
		this->joined(who, ch[0], channels);
	}
}
