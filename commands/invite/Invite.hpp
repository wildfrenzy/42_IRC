/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 03:33:36 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 19:22:05 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	Client &findClientt(std::vector<Client *> clients, std::string nick) const;
};

#endif //INVITE_HPP
