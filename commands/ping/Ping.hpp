//
// Created by Nadiia Maliarenko on 17.09.23.
//

#ifndef PING_HPP
#define PING_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;
class Channel;

class Ping  : public Cmd{
public:
	Ping();
	~Ping();

	void execute(Client &who, std::vector<std::string> cmd) const;
private:
	Ping(Ping const &p);
	Ping &operator=(Ping const &p);
};


#endif //INC_42_IRC_PING_HPP
