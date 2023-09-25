/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 21:21:00 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/25 01:32:53 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_HPP
#define JOIN_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>

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
	void joined(Client &who, const std::string& channel, std::map <std::string, Channel *> &channels) const;
	static bool validateChannelName(std::string channel) ;
	static std::string cmdToStr(std::vector<std::string> cmd) ;
};


#endif
