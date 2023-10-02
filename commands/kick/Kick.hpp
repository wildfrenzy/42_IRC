/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 05:08:29 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 19:37:41 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
#define KICK_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;
class Channel;

class Kick : public Cmd{
public:
	Kick();
	~Kick();
	void execute(Client &who, std::vector<std::string> cmd) const;

private:
	Kick(Kick const &k);
	Kick &operator=(Kick const &k);
	bool		isMember(std::vector<Client *> members, std::string nick) const;
	std::string	cmdToStr(std::vector<std::string> cmd) const;
};

#endif