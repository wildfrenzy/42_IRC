/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 14:35:19 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 22:33:18 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"
#include "./../../client/Client.hpp"

Join::Join() : Cmd(){}

Join::~Join() {}

std::string vecToUsersStr(std::vector<Client *> clients, Channel *cn){
	std::string str;
	for (size_t i = 0; i < clients.size(); ++i) {
		if (std::find(cn->getOperators().begin(), cn->getOperators().end(), clients[i]) != cn->getOperators().end())
			str.append("@");
		str.append(clients[i]->getNickName());
		if (i != clients.size() - 1)
			str.append(" ");
	}
	return str;
}

std::string Join::cmdToStr(std::vector<std::string> cmd) {
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

bool Join::validateChannelName(std::string channel) {
	if (channel.size() > 1 && (channel[0] == '#' || channel[0] == '&')){
		for (std::string::const_iterator it = channel.begin(); it < channel.end(); ++it) {
			if (!isprint(*it) || *it < 33)
				return false;
		}
		return true;
	}
	return false;
}

bool isInvited(std::vector<Client *> invites, Client *who){
	std::vector <Client *>::iterator i = std::find(invites.begin(), invites.end(), who);
	if (i != invites.end())
		return true;
	return false;
}

void Join::joined(Client &who, const std::string& channel, std::map <std::string, Channel *> &channels) const {
	Server *serv = who.getServer();
	std::vector<Client *> members = channels[channel]->getMembers();

	serv->replyNoServ(&who, ":irc_server NOTICE "
							+ channel +
							" :Welcome to " + channel + ", " + who.getNickName() +
							"! Enjoy your stay.");
	for (std::vector<Client *>::iterator it = members.begin(); it != members.end() ; ++it) {
		if ((*it)->getNickName() != who.getNickName())
			serv->replyNoServ((*it), ":" + who.getNickName() + "!" +
				who.getUserName() + "@" + who.getHost() + " JOIN " + channel);
	}
	if (channels[channel]->getTopic().empty())
		serv->reply(&who, "RPL_NOTOPIC", channel + " :No topic is set");
	else
		serv->reply(&who, "RPL_TOPIC", channel + " :" +
							channels[channel]->getTopic());
	serv->reply(&who, "RPL_NAMREPLY",  "= " + channel + " :" + vecToUsersStr(members, channels[channel]));
	serv->reply(&who, "RPL_ENDOFNAMES",  channel + " :End of user's list.");
	
}

void Join::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED",
							   ":You must authenticate with the server.");
		return;
	}else if (cmd.size() < 2){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","JOIN :Not enough parameters");
		return;
	}
	cmd = split(cmdToStr(cmd),',');
	std::vector<std::string> ch;
	std::map<std::string, Channel *> &channels = serv->getChannels();
	for (size_t i = 0; i < cmd.size(); ++i) {
		ch = split(cmd[i], ' ');
		if (ch.empty()){
			serv->reply(&who,"ERR_NEEDMOREPARAMS","JOIN :Not enough parameters");
			return;
		} else if (!validateChannelName(ch[0])){
			serv->reply(&who,"ERR_BADCHANMASK",":Bad Channel Mask");
			return;
		} else if (channels.find(ch[0]) == channels.end()){
			serv->addChannel(ch[0]);
		}
		std::vector<Client *> members = channels[ch[0]]->getMembers();
		for (std::vector<Client *>::iterator jt = members.begin(); jt != members.end() ; ++jt) {
			if (*jt == &who){
				serv->reply(&who, "ERR_USERONCHANNEL", who.getNickName() + " " + ch[0] + " :is already on channel");
				return;
			}
		}
		if ((!channels[ch[0]]->getKey().empty()) && (ch.size() != 2 || channels[ch[0]]->getKey() != ch[1])) {
			serv->reply(&who, "ERR_BADCHANNELKEY", ch[0] + " :Cannot join channel (+k)");
			return;
		} else if (channels[ch[0]]->getInviteOnly() && !isInvited(channels[ch[0]]->getInvitee(), &who)){
			serv->reply(&who, "ERR_INVITEONLYCHAN", ch[0] + " :Cannot join channel (+i)");
			return;
		} else if (channels[ch[0]]->getMemberSize() == channels[ch[0]]->getUserLimit()){
			serv->reply(&who, "ERR_CHANNELISFULL", ch[0] + " :Cannot join channel (+l)");
			return;
		}
		channels[ch[0]]->addMember(who);
		this->joined(who, ch[0], channels);
	}
}

Join &Join::operator=(const Join &j) {
	(void)j;
	return *this;
}

Join::Join(const Join &j) {
	(void)j;
}