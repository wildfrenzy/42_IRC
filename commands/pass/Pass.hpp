/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmaliare <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 18:20:57 by nmaliare          #+#    #+#             */
/*   Updated: 2023/09/12 18:20:57 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include <iostream>
#include <vector>

class Pass {
public:
	Pass();
	~Pass();

	void execute(std::vector<std::string> cmd);

private:

	//std::vector<std::string> _pass;

	Pass(Pass const &p);
	Pass &operator=(Pass const &p);
};


#endif
