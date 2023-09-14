//
// Created by Nadiia Maliarenko on 14.09.23.
//

#ifndef JOIN_HPP
#define JOIN_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;
class Channel;

class Join : public Cmd{
public:
	Join();
	~Join();

	void execute(Client &who, std::vector<std::string> cmd) const;
private:
	Join(Join const &j);
	Join &operator=(Join const &j);

};


#endif
