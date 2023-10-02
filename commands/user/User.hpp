/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 18:00:49 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/13 18:00:49 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;

class User : public Cmd{
public:
	User();
	~User();

	void execute(Client &who, std::vector<std::string> cmd) const;
private:
	User(User const &u);
	User &operator=(User const &u);
};


#endif
