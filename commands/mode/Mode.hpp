/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:53:01 by yli               #+#    #+#             */
/*   Updated: 2023/09/28 19:46:37 by yli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
#define MODE_HPP

#include "./../../cmd/Cmd.hpp"

class Client;
class Cmd;
class Channel;

#include <iostream>
#include <string>

class   Mode: public Cmd
{
    public:
        Mode(void);
        ~Mode(void);
    
        virtual    void    execute(Client& who, std::vector<std::string> cmd) const;

        void    plusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const;
        void    minusmode(Client& who, std::vector<std::string> cmd, Channel*    channel) const;
	private:
		Mode(const Mode& other);
		Mode& operator=(const Mode& other);
};


#endif

/*
Setting a channel mode:
/MODE #channel +/-mode parameters


To set the channel as invite-only:
/MODE #example +i

TO set the Channel Key:
/MODE #example +k mysecretpassword

To set the channel topic (used by channel operators):
/MODE #example +t 

To give/take channel operator privilege
/MODE #channel +o Nickname

/MODE #channel +l limit
#channel: Replace this with the name of the channel where you want to set the user limit.
+l: This indicates that you are setting a user limit.
limit: Replace this with the maximum number of users you want to allow in the channel.

In IRC (Internet Relay Chat), if you send the command "/MODE #channelname" without specifying any mode changes or parameters, you typically won't receive an error. The IRC server will usually respond by showing you the current modes of the channel, including things like whether it's moderated (+m), invite-only (+i), or other channel-specific settings.

Here's an example of what a response might look like:

arduino
Copy code
[Server] MODE #channelname +tn
In this example, the server is indicating that the channel "#channelname" has modes +t and +n set. The specific modes and their meanings can vary depending on the IRC network and channel configuration.

If you intend to make specific mode changes, you would include the appropriate parameters after the "/MODE" command. For example:

/MODE #channelname +o username would give operator status to the user with the username.
/MODE #channelname +i would set the channel to invite-only mode.
/MODE #channelname -m would remove the moderated mode.
Make sure to follow the correct syntax and understand the channel's current modes and permissions to avoid unintended consequences.

*/