//
// Created by Nadiia Maliarenko on 14.09.23.
//

#include "Join.hpp"
#include "./../../client/Client.hpp"

Join::Join() : Cmd(){}

Join::~Join() {}

void Join::execute(Client &who, std::vector <std::string> cmd) const {
	if (cmd.size() < 2){
		who.getServer()->reply(&who,
							   "ERR_NEEDMOREPARAMS",
							   "JOIN :Not enough parameters");
		return;
	}
	std::map <std::string, Channel *> &channels = who.getServer()->getChannels();
	for (int i = 1; i < cmd.size(); ++i) {
		if (channels.find(cmd[i]) == channels.end()){
			channels[cmd[i]] = new Channel();
			who.joinChannel(cmd[i], channels[cmd[i]]);
		}
	}
}