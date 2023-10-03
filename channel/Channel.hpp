/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:54:42 by yli               #+#    #+#             */
/*   Updated: 2023/10/03 00:26:59 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "../client/Client.hpp"
#include <string>
#include <vector>

class Client;
class Server;

class Channel
{
    public:
        Channel();
        ~Channel();

        void    setInviteOnly(bool invite_only);
        void    setUserLimit(int size);
        void    setChannelName(std::string& channelName);
        void    setKey(const std::string key);
        std::string getKey(void);
		void	resetKey();
        void    setTopic(std::string& topic);
        void    unsetTopic(void);
        void    setTopicRight(bool topic_right);
        size_t  getUserLimit(void);
        bool    getInviteOnly(void);
        std::string getChannelName(void);
        bool    operatorRight(Client& c);
        size_t  getMemberSize(void);
        bool    getTopicRight(void);
        std::string getTopic(void);
        void    addOperator(Client& c);
		void    deleteOperator(Client* c);
		void    deleteMembers(Client* c);
        void    addMember(Client& c);
        std::vector<Client*> &getInvitee();
        void    addInvitee(Client* c);
        void    removeInvitee(Client* c);
        void    resetInvitee();
        bool    belongToGroup(Client& who, std::vector<Client*> group);
		std::vector<Client*> &getMembers();
		void    broadcast(Server* server, Client* c, std::string msg);
        std::vector<Client*> &getOperators(void);

    private:
        std::string _channelName;
        int    _user_limit;
        bool    _invite_only;
        bool    _topic_right;
        std::string _key;
        std::string _topic;
        std::vector<Client*> _members;
        std::vector<Client*> _operators;
        std::vector<Client*> _invitee;
		Channel(const Channel& other);
		Channel &operator=(const Channel& other);
};


#endif

/*
Change the channel’s mode:

i: Set/remove Invite-only channel
t: Set/remove the restrictions of the TOPIC command to channel operators
k: Set/remove the channel key (password)
o: Give/take channel operator privilege
l: Set/remove the user limit to channel*/