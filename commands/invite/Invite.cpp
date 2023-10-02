/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 03:35:02 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 18:02:35 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invite.hpp"
#include "./../../client/Client.hpp"

Invite::Invite() : Cmd(){}
Invite::~Invite() {}
Invite::Invite(const Invite &i) {(void) i;}
Invite &Invite::operator=(const Invite &i) {
	(void)i;
	return *this;
}

bool Invite::isMember(std::vector<Client *> members, std::string nick) const{
	for (std::vector<Client*>::iterator it = members.begin() ; it != members.end() ; ++it) {
		if ((*it)->getNickName() == nick)
			return true;
	}
	return false;
}

 Client &Invite::findClientt(std::vector<Client *> clients, std::string nick) const {
	Client *cl = NULL;
 	for (std::vector<Client*>::iterator it = clients.begin() ; it != clients.end() ; ++it) {
 		if ((*it)->getNickName() == nick)
 			return **it;
 	}
	 return *cl;
 }

// Command: INVITE
// Parameters: <nickname> <channel>

void Invite::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	std::map<std::string, Channel *> &channels = serv->getChannels();
	if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED", ":You must authenticate with the server.");
		return;
	} else if (cmd.size() < 3){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","INVITE :Not enough parameters");
		return;
	} else if (channels.find(cmd[2]) == channels.end()){
		serv->reply(&who, "ERR_NOSUCHCHANNEL", cmd[2] + " :No such channel");
		return;
	} else if (channels[cmd[2]]->getInviteOnly() && !channels[cmd[2]]->operatorRight(who)){
		serv->reply(&who, "ERR_CHANOPRIVSNEEDED", cmd[2] + " :You're not channel operator");
		return;
	} else if (!channels[cmd[2]]->getInviteOnly() && !isMember(channels[cmd[2]]->getMembers(), who.getNickName())){
		serv->reply(&who, "ERR_NOTONCHANNEL", cmd[1] + " " + cmd[2] + " :You aren't on that channel");
		return;
	} else if (isMember(channels[cmd[2]]->getMembers(), cmd[1])){
		serv->reply(&who, "ERR_USERONCHANNEL", cmd[1] + " " + cmd[2] + " :is already on channel");
		return;
	} else if (!isMember(serv->getClients(), cmd[1])){
		serv->reply(&who, "ERR_NOSUCHNICK", cmd[1] + " :No such nick");
		return;
	}
	Client &member = findClientt(serv->getClients(), cmd[1]);

	channels.find(cmd[2])->second->addInvitee(&member);

	serv->replyNoServ(&who, ":" + who.getNickName() + "!" +
		who.getUserName() + "@" + who.getHost() + " 341 " +  who.getNickName() + " " + cmd[1] + " " + cmd[2]);

	serv->replyNoServ(&member, ":" + who.getNickName() + "!" +
							who.getUserName() + "@" + who.getHost() + " INVITE " + cmd[1] + " " + cmd[2]);

}
