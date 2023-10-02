/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:08 by yli               #+#    #+#             */
/*   Updated: 2023/09/20 18:53:09 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "./../../cmd/Cmd.hpp"


class Cmd;
class Client;

class Nick: public Cmd
{
    public:
        Nick();
        ~Nick();
    
        void    execute(Client& who, std::vector<std::string> cmd) const;
    
    private:
        Nick(const Nick& other);
        Nick& operator=(const Nick& other);
};




#endif