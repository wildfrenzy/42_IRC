/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:20:57 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/13 03:19:44 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include <iostream>
#include <vector>

#include "./../../cmd/Cmd.hpp"

class Cmd;
class Client;

class Pass : public Cmd {
public:
	Pass();
	~Pass();

	void execute(Client &who, std::vector<std::string> cmd) const;

private:

	Pass(Pass const &p);
	Pass &operator=(Pass const &p);
};


#endif
