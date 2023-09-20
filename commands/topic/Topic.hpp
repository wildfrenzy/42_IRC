/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:29 by yli               #+#    #+#             */
/*   Updated: 2023/09/20 18:53:30 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "./../../cmd/Cmd.hpp"
// #include "./../../channel/Channel.hpp"
// #include "./../../client/Client.hpp"
#include <ctime>

class Client;
class Cmd;
class Channel;

class Topic: public Cmd
{
    public:
        Topic(void);
        ~Topic(void);

        void    execute(Client& who, std::vector<std::string> cmd) const;
        void    setTopic(std::string& topic, Channel *c) const;
        bool    checkOperatorRight(Client& who, Channel *c) const;
        void    unsetTopic(Channel *c) const;
        void    checkTopic(Client& who, Channel *c, std::string channelName) const;
	private:
		Topic(const Topic& other);
		Topic&  operator=(const Topic& other);
};



#endif

//   TOPIC #test :New topic          ; Setting the topic on "#test" to
//                                   "New topic".

//   TOPIC #test :                   ; Clearing the topic on "#test"

//   TOPIC #test                     ; Checking the topic for "#test"