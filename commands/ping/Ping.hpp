/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 20:31:04 by nmaliare          #+#    #+#             */
/*   Updated: 2023/10/02 19:33:17 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
#define PING_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;
class Channel;

class Ping : public Cmd{
public:
	Ping();
	~Ping();

	void execute(Client &who, std::vector<std::string> cmd) const;
private:
	Ping(Ping const &p);
	Ping &operator=(Ping const &p);
};


#endif
