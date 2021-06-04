#include "Server.hpp"

Server::Server()
{
	this->flag_loc = 0;
}

Server::~Server()
{

}

// void Server::setServerName(std::string ServerName)
// {
// 	this->server_name = ServerName;
// }

// void Server::setHost(std::string Host)
// {
// 	this->host = Host;
// }

// void Server::setPort(std::string Port)
// {
// 	this->port = Port;
// }

// void Server::setBodySize(std::string BodySize)
// {
// 	this->max_body_size = BodySize;
// }

void Server::setLocPath(std::string LocPath, int pos)
{
	this->loc_path[pos] = LocPath;
}

void Server::setFlagLoc(bool FlagLoc)
{
	this->flag_loc = FlagLoc;
}

void Server::setMapLoc(std::map < std::string, std::map<std::string, std::string> > map)
{
	this->location = map;
}

void Server::setMapHeadFields(std::map <std::string, std::string> map)
{
	this->head_fields = map;
}

// std::string Server::getServerName(void)
// {
// 	return (this->server_name);
// }

// std::string Server::getHost(void)
// {
// 	return (this->host);
// }

// std::string Server::getPort(void)
// {
// 	return (this->port);
// }

// std::string Server::getBodySize(void)
// {
// 	return (this->max_body_size);
// }

bool Server::getFlagLoc(void)
{
	return (this->flag_loc);
}

std::string Server::getLocPath(int pos)
{
	return (this->loc_path[pos]);
}

std::map < std::string, std::map<std::string, std::string> > Server::getMapLoc(void)
{
	return (this->location);
}

std::map <std::string, std::string> Server::getMapHeadFields(void)
{
	return (this->head_fields);
}
