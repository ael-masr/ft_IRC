#include "includes/Server.hpp"
#include "includes/User.hpp"
#include "includes/Commands.hpp"
#include "includes/Parse.hpp"
#include "includes/Channel.hpp"
bool isNumeric(const std::string& str)
{
    // Check if the string is empty or if it's a negative number
    if(str.empty())
        std::cout << "empty input" << std::endl;
    for(std::string::size_type i = 1; i < str.size(); ++i)
    {
        if(!std::isdigit(str[i]))
            return false;
    }
        return true;
}
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
    if (parameters.size() == 0)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
        nick(parameters[0],currentUser);
}
/*###################################################################*/
void    handleKickCommand(std::vector<std::string>& parameters,User &currentUser)
{
    std::string reason;
    if (parameters.size()  < 2)
        std::cout << "461 :Not enough parameters" << std::endl;
    if(parameters[2].size() > 0)
        std::string reason = parameters[2];
    else reason = "";
    if(channel_exists(parameters[0]))
        kick(Channel(parameters[0],true), currentUser,parameters[1],reason);
    else if(!channel_exists(parameters[0]))
        kick(Channel(parameters[0],false), currentUser,parameters[1],reason);
}

/*###################################################################*/
void    handlePrivmsgCommand(std::vector<std::string> parameters,User& currentUser)
{
    std::vector<std::string> message;
    if (parameters.size()  < 2)
    {
        std::cout << "461 :Not enough parameters" << std::endl;
        return;
    }
    for (size_t i = 1; i < parameters.size(); i++)
        message.push_back(parameters[i]);
    if(parameters[0][0] == '#' || parameters[0][0] == '&' )
    {
        if(channel_exists(parameters[0]))
            privmsg(Channel(parameters[0],true),currentUser,message);
        else    
            privmsg(Channel(parameters[0],false),currentUser,message);
    }
    else 
        privmsg(parameters[0],currentUser,message);
}

/*###################################################################*/
void    handleJoinCommand(std::vector<std::string>& parameters,User currentUser)
{
    std::string password;
    if (parameters.size()  < 1)
        std::cout << "461 :Not enough parameters" << std::endl;
    if(parameters[1].size() > 0)
        std::string password = parameters[1];
    else password = "";
    if(channel_exists(parameters[0]))
        join(Channel(parameters[0],true), currentUser,password);
    else if(!channel_exists(parameters[0]))
        join(Channel(parameters[0],false), currentUser,password);
}

/*###################################################################*/
void    handlePartCommand(std::vector<std::string>& parameters,User &currentUser)
{
    if (parameters.size()  < 1)
    {
        std::cout << "461 :Not enough parameters" << std::endl;
        return;
    }
    if(channel_exists(parameters[0]))
        part(Channel(parameters[0],true), currentUser);
    else if(!channel_exists(parameters[0]))
        part(Channel(parameters[0],false), currentUser);
}

/*###################################################################*/
void    handleModeCommand(std::vector<std::string>& parameters,User &currentUser)
{
    if (parameters.size()  < 2)
    {
        std::cout << "461 :Not enough parameters" << std::endl;
        return;
    }
    std::string mode_arg;
    if(parameters[2].size() > 0)
        mode_arg = parameters[2];
    else 
        mode_arg = "";
    if(channel_exists(parameters[0]))
        mode(Channel(parameters[0],true), currentUser,parameters[1][0],parameters[1][1],mode_arg);
    else if(!channel_exists(parameters[0]))
        mode(Channel(parameters[0],false), currentUser,parameters[1][0],parameters[1][1],mode_arg);
}
/*###################################################################*/
void    handleTopicCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  < 1)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
    {
        std::string topic_str;
        if(parameters[1].size() > 0)
            std::string topic = parameters[1];
        else topic_str = "";
        if(channel_exists(parameters[0]))
            topic(Channel(parameters[0],true), currentUser,topic_str);
        else if(!channel_exists(parameters[0]))
            topic(Channel(parameters[0],false), currentUser,topic_str);
    }
}

/*###################################################################*/
void    handleNoticeCommand(std::vector<std::string> &parameters,User &currentUser)
{
    std::vector<std::string> message;
    if (parameters.size()  < 2)
    {
        std::cout << "461 :Not enough parameters" << std::endl;
        return;
    }
    if(Utils::nickname_exists(parameters[0]))
    {
        for (size_t i = 1; i < parameters.size(); i++)
            message.push_back(parameters[i]);
        notice(parameters[0],currentUser,message);
    }
    else
    {
        for (size_t i = 0; i < parameters.size(); i++)
            message.push_back(parameters[i]);
        notice(currentUser,message);
    }
}

/*###################################################################*/
void    handleCapCommand(std::vector<std::string> &parameters,User &currentUser)
{
    if (parameters.size()  < 1)
        std::cout << "461 :Not enough parameters" << std::endl;
    else
    {
        std::string subcommand;
        if(parameters[0].size() > 0)
            std::string subcommand = parameters[0];
        else subcommand = "";
        //cap(subcommand,currentUser);
        //to be implemented
    }
}
/*###################################################################*/

void parse(User &currentUser)
{
    std::string input;
    while (true)
    {
        std::cout << "Enter command: ";
        if (!std::getline(std::cin, input) || input == "QUIT")
        {
            std::cout << "Disconnecting user and exiting program.\n";
            break;
        }
        std::istringstream iss(input);
        std::string command;
        std::vector<std::string> parameters;
        iss >> command; 
        std::string param;
        while (iss >> param && parameters.size() < 2)
        {
            parameters.push_back(param);
        }
        if (command == "NICK")
            handleNickCommand(parameters, currentUser);
        else if (command == "KICK")
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
        else if (command == "QUIT")
            break;
        else
            std::cout << "421 :Unknown command" << std::endl;
    }
}
/*###################################################################*/