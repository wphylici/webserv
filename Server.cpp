#include "Server.hpp"

Server::Server()
{
	this->_flag_loc = false;
	this->_loc_path.push_back(std::vector<std::string>());
}

Server::~Server()
{

}

// void Server::setServerName(std::string ServerName)
// {
// 	this->_server_name = ServerName;
// }

// void Server::setHost(std::string Host)
// {
// 	this->_host = Host;
// }

// void Server::setPort(std::string Port)
// {
// 	this->_port = Port;
// }

// void Server::setBodySize(std::string BodySize)
// {
// 	this->_max_body_size = BodySize;
// }

void Server::setLocPath(std::string LocPath)
{
	this->_loc_path[this->_loc_path.size() - 1].push_back(LocPath);
}

void Server::setFlagLoc(bool FlagLoc)
{
	this->_flag_loc = FlagLoc;
}

void Server::seMethods(std::map < int, std::map<int, std::string> > methods)
{
	this->_methods = methods;
}

void Server::setMapLoc(std::map < std::string, std::map<std::string, std::string> > map)
{
	this->_location = map;
}

void Server::setMapHeadFields(std::map <std::string, std::string> map_head_fields)
{
	this->_head_fields = map_head_fields;
}

void Server::setErrorPages(std::map <std::string, std::string> error_pages)
{
	this->_error_pages = error_pages;
}

// std::string Server::getServerName(void)
// {
// 	return (this->_server_name);
// }

// std::string Server::getHost(void)
// {
// 	return (this->_host);
// }

// std::string Server::getPort(void)
// {
// 	return (this->_port);
// }

// std::string Server::getBodySize(void)
// {
// 	return (this->_max_body_size);
// }

bool Server::getFlagLoc(void)
{
	return (this->_flag_loc);
}

std::vector <std::string> Server::getValueLocPath(void)
{
	return(this->_loc_path[0]);
}

std::vector < std::vector <std::string> > Server::getLocPath()
{
	return (this->_loc_path);
}

std::map < int, std::map<int, std::string> > &Server::getMethods(void)
{
	return (this->_methods);
}

std::map < std::string, std::map<std::string, std::string> > &Server::getMapLoc(void)
{
	return (this->_location);
}

std::map <std::string, std::string> &Server::getErrorPages(void)
{
	return (this->_error_pages);
}

std::map <std::string, std::string> &Server::getMapHeadFields(void)
{
	return (this->_head_fields);
}