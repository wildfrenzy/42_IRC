/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 01:13:22 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/16 02:48:44 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Nadiia Maliarenko on 14.09.23.
//

#include "Join.hpp"
#include "./../../client/Client.hpp"
#include <string>
#include <algorithm>

Join::Join() : Cmd(){}

Join::~Join() {}

std::string vecToUsersStr(std::vector<Client *> clients, Channel *cn){
	std::string str;
	for (size_t i = 0; i < clients.size(); ++i) {
		if (std::find(cn->getOperators().begin(), cn->getOperators().end(), clients[i]) != cn->getOperators().end())
			str.append("@");
		/*if (clients[i]->getModes().find('o') != std::string::npos)
			str.append("@");*/
		str.append(clients[i]->getNickName());
		if (i != clients.size() - 1)
			str.append(" ");
	}
	return str;
}

std::string Join::cmdToStr(std::vector<std::string> cmd) const{
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
		serv->reply(&who, "RPL_TOPIC", channel + " :Channel topic is " +
							channels[channel]->getTopic() + "!");
	//serv->reply(&who, "RPL_TOPICWHOTIME", channel + "TemporaryTest 16090246");
	serv->reply(&who, "RPL_NAMREPLY",  "= " + channel + " :" + vecToUsersStr(members, channels[channel]));
	serv->reply(&who, "RPL_ENDOFNAMES",  channel + " :End of user's list.");
	
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
	for (size_t i = 0; i < cmd.size(); ++i) {
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
			std::cout << "channel added: " << ch[0] << std::endl;
			serv->addChannel(ch[0]); //?
		} else if ((!channels[ch[0]]->getKey().empty()) && (ch.size() != 2 || channels[ch[0]]->getKey() != ch[1])) {
			serv->reply(&who, "ERR_BADCHANNELKEY", ch[0] + " :Cannot join channel (+k)");
			return;
		} else if (channels[ch[0]]->getInviteOnly()){
			serv->reply(&who, "ERR_INVITEONLYCHAN", ch[0] + " :Cannot join channel (+i)");
			return;
		} else if (channels[ch[0]]->getMemberSize() == channels[ch[0]]->getUserLimit()){
			serv->reply(&who, "ERR_CHANNELISFULL", ch[0] + " :Cannot join channel (+l)");
			return;
		}
		//std::cout << "check if on channel" << std::endl;
		std::vector<Client *> members = channels[ch[0]]->getMembers();
		for (std::vector<Client *>::iterator jt = members.begin(); jt != members.end() ; ++jt) {
			if (*jt == &who){
				serv->reply(&who, "ERR_USERONCHANNEL", who.getNickName() + " " + ch[0] + " :is already on channel");
				return;
			}
		}
		//std::cout << "check if on channel end" << std::endl;
		channels[ch[0]]->addMember(who);
		this->joined(who, ch[0], channels);
	}
}
