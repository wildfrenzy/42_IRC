/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yli <yli@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:54:59 by yli               #+#    #+#             */
/*   Updated: 2023/10/02 20:45:44 by nmaliare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "./../client/Client.hpp"

Bot::Bot() : Cmd() {}

Bot::~Bot() {}

Bot::Bot(const Bot& other) {(void) other;}

Bot& Bot::operator=(const Bot& other)
{
	(void) other;
	return *this;
}

std::string    Bot::answerTime() const
{
	char buffer[80];

	std::time_t now = std::time(0);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	std::string strTime(buffer);
	return ("The time is now " + strTime);
}

std::string    Bot::drawanimals(std::string str) const
{
	if (str == "dog")
	{
		std::string msg = 
   ".....;;...                ................ \n"
   ".......;;;;;/mmmmmmmmmmmmm\\/.................. \n"
   "........;;;mmmmmmmmmmmmmmmmmmm..................... \n"
   ".........;;m/;;;;/mmmmmm/;;;;\\m...................... \n"
   "..........;;;m;;mmmm;;mmmm;;mmmmm;;m...................... \n"
   "..........;;;;;mmmnnnmmmmmmmmmmnnnmmm\\.................... \n"
   ".........  ;;;;n/#####\\nmmmn/#####\\nm\\................. \n"
   ".......     ;;;;n##...##nmmmmn##...##nmmmm\\............. \n"
   "....        ;;;n#.....|nmmmmn#.....#nmmmmm,l......... \n"
   "..          /mmmn\\.../nmmmmmmn\\.../nmmmm,m,lll..... \n"
   "           /mmmmmmmmmmmmmmmmmmmmmmmmmmm,mmmm,llll.. \n"
   "       /mmmmmmmmmmmmmmmmmmmmmm\\nmmmn/mmmmmmm,lll/ \n"
   "    /mmmmm/..........\\mmmmmmmmmnnmnnmmmmmmmmm,ll \n"
   "   /mmmmmm|...........|mmmmmmmmmmmmmmmmmmmmmmmm,ll \n"
   "   \\mmmmm\\......./mmmmmmmmmmmmmmmmmmmmmmmmm,llo \n"
   "     \\mmmmm\\.../mmmmmmmmmmmmmmmmmmmmmmmm,lloo \n"
   "       \\mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm,ll/oooo \n"
   "         \\mmmmmmmmmll..;;;.;;;;;;/mmm,lll/ooooo\\ \n"
   "                    ll..;;;.;;;;;;/llllll/ooooooooo \n"
   "                    ll.;;;.;;;;;/.llll/oooooooooo..o \n"
   "                    ll;;;.;;;;;;..ll/ooooooooooo...oo \n"
   "                    \\;;;.;;;;;..ll/ooooo...ooooo..o\\ \n"
   "                  ;;;;;;;;;;;;..ll|oooo.....oooooooooo \n"
   "                 ;;;;;;.;;;;;;.ll/oooo.....ooooooo....\n"
   "                 ;;;;;.;;;;;;;ll/ooooooooooooo.....oooo \n"
   "                  \\;;.;;;;;;/oooooooooooo.....ooooooo\\ \n"
   "                   \\;;.;;;;/ooooooooo.....ooooooooooooo \n"
   "                     \\;;;/ooooooo.....ooooooooooo...oo\\ \n"
   "                     |o\\/oooo.....ooooooooooooo......ooo \n"
   "                     oooooo....ooooooooooooooooooo.....o\\ \n"
   "                    oooo....oooooooooooooooooooooooo..oooo \n"
   "                   ___.oooooooooooooo....ooooooooooooooooo\\ \n"
   "                 /XXX\\ooooooooooo.....ooooooooooooooooooo \n"
   "                  |XXX|ooooo.oooooo....ooooooooooooooooooooo \n"
   "                /oo\\X/oooo..ooooooooooooooooooo..ooooooooooo \n"
   "              /ooooooo..ooooo..oooooooooooooo.....ooooooooo... \n"
   "            /ooooo...ooooo.....oooooooooooo.......ooooooo.....o \n";
    return msg;
  }
  if (str == "cat")
  {
    std::string msg = 
    "   ,               \n"
    "   \\`-._           __       \n"
    "    \\  `-..____,.'  `.      \n"
    "     :`.         /    \\`.     \n"
    "     :  )       :      : \\    \n"
    "      ;'        '   ;  |  :    \n"
    "      )..      .. .:.`.;  :    \n"
    "     /::...  .:::...   ` ;     \n"
    "     ; _ '    __        /:\\   \n"
    "     `:o>   /\\o_>      ;:. `.   \n"
    "    `-`.__ ;   __..--- /:.   \\  \n"
    "    === \\_/   ;=====_.':.     ; \n"
    "     ,/'`--'...`--....        ;  \n"
    "          ;                    ; \n"
    "        .'                      ;  \n"
    "      .'                        ;  \n"
    "    .'     ..     ,      .       ; \n"
    "   :       ::..  /      ;::.     | \n"
    "  /      `.;::.  |       ;:..    ; \n"
    " :         |:.   :       ;:.    ;  \n"
    " :         ::     ;:..   |.    ; \n"
    "  :       :;      :::....|     | \n"
    "  /\\     ,/ \\      ;:::::;     ; \n"
    ".:. \\:..|    :     ; '.--|     ;  \n"
   "::.  :''  `-.,,;     ;'   ;     ; \n"
".-'. _.'\\      / `;      \\,__:       \n"
"`---'    `----'   ;      /    \\,.,,,/ \n";  
  return msg;
  }
  if (str == "bird")
  {
    std::string msg = 
"  /\\_/\\  \n"
" ((@v@)) \n"
" ():::() \n"
"  VV-VV  \n";
    return msg;
  }
  return ("please type cat/dog/bird");
}

void Bot::execute(Client &who, std::vector <std::string> cmd) const
{
	Server *serv = who.getServer();
	if (!who.getRegistered()) {
		serv->reply(&who, "ERR_NOTREGISTERED",
					":You must authenticate with the server.");
		return;
	} else if (cmd.size() < 2){
		serv->reply(&who,"ERR_NEEDMOREPARAMS","BOT :Not enough parameters");
		return;
	}
	std::string sub = cmd[1].substr(1);
	if (sub == "time")
	{
		std::string msg = answerTime();
		who.getServer()->replyNoServ(&who, msg);
			return;
	}
	if (sub == "draw" && cmd[2] == "a")
	{
		std::string msg = drawanimals(cmd[3]);
		who.getServer()->replyNoServ(&who, msg);
			return;
	}
	who.getServer()->replyNoServ(&who, "I don't support this cmd");
			return;
}