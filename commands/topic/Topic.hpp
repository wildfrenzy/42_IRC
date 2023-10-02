/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:29 by yli               #+#    #+#             */
/*   Updated: 2023/09/28 17:11:45 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPIC_HPP
#define TOPIC_HPP

#include "./../../cmd/Cmd.hpp"
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
        void    setTopic(std::string topic, Channel *c) const;
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

//RPL_TOPIC (332) 
//  "<client> <channel> :<topic>"
//Sent to a client when joining the <channel> to inform them of the current topic of the channel.


//RPL_TOPICWHOTIME (333) 
//  "<client> <channel> <nick> <setat>"
//Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC 

/*
ERR_CHANOPRIVSNEEDED (482) 
  "<client> <channel> :You're not channel operator"
Indicates that a command failed because the client does not have the appropriate channel privileges. 
This numeric can apply for different prefixes such as halfop, operator, etc. 
The text used in the last param of this message may vary.
*/