/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <wphylici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 22:30:19 by wphylici          #+#    #+#             */
/*   Updated: 2021/05/25 07:33:33 by wphylici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fcntl.h>
#include "get_next_line/get_next_line.h"

Server servinfo;

void del_space_chars(std::string *str)
{
	while((*str)[0] == '\t' || (*str)[0] == ' ')
		str->erase(0, 1);
}

void first_level_field(std::string *str, int count_tab)
{
	if (((str->substr(0, str->find(':')) == "server" && count_tab > 0) ||
		(str->substr(0, str->find(':')) != "server" && count_tab != 1) ||
		(str->substr(0, str->find(':')) == "location" && count_tab != 1)) &&
		servinfo.getFlagLoc() == false)
		throw "invalide tabulation";
	std::string test = str->substr(0, str->find(' '));
	if (str->substr(0, str->find(' ')).find(':') == str->npos)
		throw "expected ':' after key name";
	str->erase(0, str->substr(0, str->find(' ')).find(':') + 1);
	del_space_chars(str);
	std::string tmp = str->substr(str->substr(0,
	str->find(' ') < 0 ? str->find('\t') : str->find(' ')).size(), str->size());
	del_space_chars(&tmp);
	if (!tmp.empty())
		throw "invalid symbols";
}

void parse_inside_loc(std::string str, int count_tab)
{

	// if (count_tab != 2)
	// 	throw "invalide tabulation";
	if (!str.compare(0, 4, "root"))
	{
		if (str[4] != ':')
			throw "expected ':' after key name";
		str.erase(0, 5);
		del_space_chars(&str);
		// tmp[servinfo.getLocPath()]["root"] = str;
	}
	// else if (!str.empty())
	// 	throw "invalid symbols";
}

void location_conf(std::string str, int count_tab)
{
	static std::map < std::string, std::map<std::string, std::string> > tmp;

	if (servinfo.getFlagLoc() == false)
	{
		first_level_field(&str, count_tab);
		servinfo.setLocPath(str);
		servinfo.setFlagLoc(true);
	}
	else
	{
		if (count_tab != 2)
			throw "invalide tabulation";
		if (str.substr(0, str.find(':')) == "root")
		{
			first_level_field(&str, count_tab);
			tmp[servinfo.getLocPath()]["root"] = str;
		}
		else if (str.substr(0, str.find(':')) == "index")
		{
			first_level_field(&str, count_tab);
			tmp[servinfo.getLocPath()]["index"] = str;
		}
		else if (str.substr(0, str.find(':')) == "method")
		{
			first_level_field(&str, count_tab);
			tmp[servinfo.getLocPath()]["method"] = str;
		}
		else if (str.substr(0, str.find(':')) == "cgi_extension")
		{
			first_level_field(&str, count_tab);
			tmp[servinfo.getLocPath()]["cgi_extension"] = str;
		}
		else if (str.substr(0, str.find(':')) == "autoindex")
		{
			first_level_field(&str, count_tab);
			tmp[servinfo.getLocPath()]["autoindex"] = str;
		}
	}
	servinfo.setMap(tmp);
}

void parse(char *line)
{
	int count_tab = 0;
	static int flag_serv = 0;
	std::string str = line;

	for (size_t i = 0; str[i] == '\t'; i++)
		count_tab++;
	del_space_chars(&str);
	if (str.substr(0, str.find(':')) == "server")
		first_level_field(&str, count_tab);
	else if (str.substr(0, str.find(':') < 0 ? str.find(' ') :
			str.find(':')) == "server_name")
	{
		first_level_field(&str, count_tab);
		servinfo.setServerName(str.substr(0, str.find(' ')));
	}
	else if (str.substr(0, str.find(':') < 0 ? str.find(' ') :
			str.find(':')) == "host")
	{
		first_level_field(&str, count_tab);
		servinfo.setHost(str.substr(0, str.find(' ')));
	}
	else if (str.substr(0, str.find(':') < 0 ? str.find(' ') :
			str.find(':')) == "port")
	{
		first_level_field(&str, count_tab);
		servinfo.setPort(str.substr(0, str.find(' ')));
	}
	else if (str.substr(0, str.find(':') < 0 ? str.find(' ') :
			str.find(':')) == "max_body_size")
	{
		first_level_field(&str, count_tab);
		servinfo.setBodySize(str.substr(0, str.find(' ')));
	}
	else if ((str.substr(0, str.find(':') < 0 ? str.find(' ') :
			str.find(':')) == "location" || servinfo.getFlagLoc() == true))
	{
		location_conf(str, count_tab);
	}
	else if (!str.empty())
		throw "invalid symbols";
}

void read_conf()
{
	char *line;
	int n_line = 1;
	int fd = open("default.conf", O_RDONLY);;

	while (get_next_line(fd, &line) > 0)
	{
		try
		{
			parse(line);
		}
		catch(const char *ex)
		{
			std::cerr << BHI_RED << "config file error: " << RESET << ex
			<< "  (line: " << n_line << ")" << '\n';
			exit (EXIT_FAILURE);
		}
		free(line);
		n_line++;
	}
	free(line);
	std::cout << "server_name: " << servinfo.getServerName() << std::endl;
	std::cout << "host: " << servinfo.getHost() << std::endl;
	std::cout << "port: " << servinfo.getPort() << std::endl;
	std::cout << "max_body_size: " << servinfo.getBodySize() << std::endl << std::endl;
	std::cout << "location path: " << servinfo.getLocPath() << std::endl;

	std::cout << "	root: " << servinfo.getMap()[servinfo.getLocPath()]["root"] << std::endl;
	std::cout << "	index: " << servinfo.getMap()[servinfo.getLocPath()]["index"] << std::endl;
	std::cout << "	method: " << servinfo.getMap()[servinfo.getLocPath()]["method"] << std::endl;
	std::cout << "	cgi_extension: " << servinfo.getMap()[servinfo.getLocPath()]["cgi_extension"] << std::endl;
	std::cout << "	autoindex: " << servinfo.getMap()[servinfo.getLocPath()]["autoindex"] << std::endl;
}

int main ()
{
	read_conf();
	return (0);
}
