/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:33:36 by yli               #+#    #+#             */
/*   Updated: 2023/09/15 14:15:46 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

//#include "./../client/Client.hpp"
#include <vector>
#include <string>

class Client;

class Cmd
{
    public:
		Cmd();
        ~Cmd();
        Cmd(const Cmd & other);
        Cmd& operator=(const Cmd& other);
        
        virtual void	execute(Client &who, std::vector<std::string> tokens) const = 0;

    
    // protected:
    //     std::vector<std::string> _tokens;
};


#endif