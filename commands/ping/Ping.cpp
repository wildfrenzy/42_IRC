//
// Created by Nadiia Maliarenko on 17.09.23.
//

#include "Ping.hpp"
#include "./../../client/Client.hpp"

Ping::Ping() : Cmd(){}
Ping::~Ping(){}
Ping::Ping(const Ping &p) {
	(void)p;
}

Ping &Ping::operator=(const Ping &p) {
	(void)p;
	return *this;
}

void Ping::execute(Client &who, std::vector <std::string> cmd) const {
	Server *serv = who.getServer();
	if (cmd.size() < 2){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","PING :Not enough parameters");
		return;
	} else if (cmd[1].size() == 1 && cmd[1][0] == ':'){
		serv->reply(&who,"ERR_NOORIGIN",":No origin specified");
		return;
	} else if (cmd[1][0] != ':'){
		serv->replyNoServ(&who,"PONG :" + cmd[1]);
	} else
		serv->replyNoServ(&who,"PONG " + cmd[1]);
}