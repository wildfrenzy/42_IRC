//
// Created by Nadiia Maliarenko on 15.09.23.
//

#ifndef INVITE_HPP
#define INVITE_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;
class Channel;

class Invite : public Cmd {
public:
	Invite();
	~Invite();

	void execute(Client &who, std::vector<std::string> tokens) const;

private:
	Invite(Invite const &i);
	Invite &operator=(Invite const &i);
	bool isMember(std::vector<Client *> members, std::string nick) const;
	//Client &findClient(std::vector<Client *> clients, std::string nick) const;
};


#endif //INVITE_HPP
