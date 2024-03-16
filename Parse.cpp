#include "includes/Server.hpp"
#include "includes/User.hpp"
#include "includes/Commands.hpp"
#include "includes/Parse.hpp"
#include "includes/Channel.hpp"
/*###################################################################*/

std::vector<std::string> split(const std::string str)
{
	std::vector<std::string> vector;
	std::istringstream iss(str);
	std::string cmd;
	while (iss >> std::skipws >> cmd)
		vector.push_back(cmd);
	return vector;
}
/*###################################################################*/


void    handleNickCommand(std::vector<std::string>& parameters,User& currentUser)
{
    if (parameters.size() != 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    else
        Commands::nick(parameters[0],currentUser);
}
/*###################################################################*/
void    handleKickCommand(std::vector<std::string>& parameters,User &currentUser)
{
    std::string reason;
    if (parameters.size()  != 2)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    if(parameters[2].size() > 0)
        std::string reason = parameters[2];
    else reason = "";
        Commands::kick(Channel(parameters[0],false), currentUser,parameters[1],reason);
}

/*###################################################################*/
void    handlePrivmsgCommand(std::vector<std::string> parameters,User& currentUser)
{
    std::vector<std::string> message;
    if (parameters.size()  < 2)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    for (size_t i = 1; i < parameters.size(); i++)
        message.push_back(parameters[i]);
    if(parameters[0][0] == '#' || parameters[0][0] == '&' )
            Commands::privmsg(Channel(parameters[0],false),currentUser,message);
    else 
        Commands::privmsg(parameters[0],currentUser,message);
}

/*###################################################################*/
void    handleJoinCommand(std::vector<std::string>& parameters,User currentUser)
{
    std::string password;
    if (parameters.size()  != 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    if(parameters[1].size() > 0)
        std::string password = parameters[1];
    else password = "";
        Commands::join(Channel(parameters[0],false), currentUser,password);
}

/*###################################################################*/
void    handlePartCommand(std::vector<std::string>& parameters,User &currentUser)
{
    if (parameters.size()  < 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    if(channel_exists(parameters[0]))
        Commands::part(Channel(parameters[0],true), currentUser);
    else if(!channel_exists(parameters[0]))
        Commands::part(Channel(parameters[0],false), currentUser);
}

/*###################################################################*/
void    handleModeCommand(std::vector<std::string>& parameters,User &currentUser)
{
    if (parameters.size()  < 2)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    std::string mode_arg;
    if(parameters[2].size() > 0)
        mode_arg = parameters[2];
    else 
        mode_arg = "";
    if(channel_exists(parameters[0]))
        Commands::mode(Channel(parameters[0],true), currentUser,parameters[1][0],parameters[1][1],mode_arg);
    else if(!channel_exists(parameters[0]))
        Commands::mode(Channel(parameters[0],false), currentUser,parameters[1][0],parameters[1][1],mode_arg);
}
/*###################################################################*/
void    handleTopicCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  < 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    else
    {
        std::string topic_str;
        if(parameters[1].size() > 0)
            std::string topic = parameters[1];
        else topic_str = "";
        if(channel_exists(parameters[0]))
            Commands::topic(Channel(parameters[0],true), currentUser,topic_str);
        else if(!channel_exists(parameters[0]))
            Commands::topic(Channel(parameters[0],false), currentUser,topic_str);
    }
}

/*###################################################################*/
void    handleNoticeCommand(std::vector<std::string> &parameters,User &currentUser)
{
    std::vector<std::string> message;
    if (parameters.size()  < 1 || (Utils::nickname_exists(parameters[0]) && parameters.size() < 2))
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    if(Utils::nickname_exists(parameters[0]))
    {
        for (size_t i = 1; i < parameters.size(); i++)
            message.push_back(parameters[i]);
        Commands::notice(parameters[0],currentUser,message);
    }
    else
    {
        for (size_t i = 0; i < parameters.size(); i++)
            message.push_back(parameters[i]);
        Commands::notice(currentUser,message);
    }
}

/*###################################################################*/
void    handleCapCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  < 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
	if(parameters.size() >= 3 && parameters[0] == "LS" && parameters[1] == "302")
	{
		std::string S = "CAP * ACK :multi-prefix\r\n";
		send(currentUser.getFd(), S.c_str(), strlen(S.c_str()), 0);
	}
    else if (parameters.size() >= 2 && parameters[0] == "LS")
	{
	    std::string S = "CAP * ACK :multi-prefix\r\n";
		send(currentUser.getFd(),S.c_str(), strlen(S.c_str()), 0);
	}
	else if (parameters.size() >= 3 && parameters[0] == "REQ" && parameters[1] == "multi-prefix")
	{
		std::string S = "CAP * ACK :multi-prefix\n";
		send(currentUser.getFd(),S.c_str(), strlen(S.c_str()), 0);
	}
    return;
}

/*###################################################################*/
void    handlePassCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  != 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
        Commands::pass(password,currentUser);
}
/*###################################################################*/
void   handleUserCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  != 1)
    {
        Utils::sendErrorMessage(currentUser.getFd(), "461 :Not enough parameters", 461);
        return;
    }
    Commands::user(parameters[0],currentUser);
}
/*###################################################################*/

void parse(User &currentUser, std::string input)
{
        std::istringstream iss(input);
        std::string command;
        std::vector<std::string> parameters;
        iss >> command; 
        std::string param;
        if (currentUser.user_flag && currentUser.nick_flag && currentUser.pass_flag)
            currentUser.setRegistered(true);
        while(iss >> param)
            parameters.push_back(param);
        if (command == "NICK")
            handleNickCommand(parameters, currentUser);
        else if (command == "PASS")
            handlePassCommand(parameters,currentUser);
        else if (command == "USER")
            handleUserCommand(parameters,currentUser);
        else if(currentUser.isRegistered() == true)
        {
            if (command == "KICK")
                handleKickCommand(parameters,currentUser);
            else if (command == "PRIVMSG")
                handlePrivmsgCommand(parameters,currentUser);
            else if (command == "JOIN")
                handleJoinCommand(parameters,currentUser);
            else if (command == "PART")
                handlePartCommand(parameters,currentUser);
            else if (command == "MODE")
                handleModeCommand(parameters,currentUser);
            else if(command == "TOPIC")
                handleTopicCommand(parameters,currentUser);
            else if (command == "NOTICE")
                handleNoticeCommand(parameters,currentUser);
            else if(command == "CAP")
                handleCapCommand(parameters,currentUser);
            else
            {
                Utils::sendErrorMessage(currentUser.getFd(), "421 :Unknown command", 421);
                return;
            }
        }
        else if(currentUser.isRegistered() == false)
            Utils::sendErrorMessage(currentUser.getFd(), "451 :You have not registered", 451);
        else
            Utils::sendErrorMessage(currentUser.getFd(), "421 :Unknown command", 421);
}
/*###################################################################*/