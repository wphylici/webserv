/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <wphylici@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 22:30:19 by wphylici          #+#    #+#             */
/*   Updated: 2021/05/25 03:14:25 by wphylici         ###   ########.fr       */
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

void head_conf(std::string str, int count_tab)
{
	if (count_tab > 0)
		throw "invalide tabulation";
	str.erase(0, 6);
	del_space_chars(&str);
	if (str.size() > 1)
		throw "invalid symbols";
}

void host_conf(std::string *str, int count_tab)
{
	if (count_tab != 1)
		throw "invalide tabulation";
	if (str->substr(0, str->find(' ')).find(':') == str->npos)
		throw "expected ':' after key name";
	str->erase(0, str->substr(0, str->find(' ')).find(':') + 1);
	del_space_chars(str);
	if (std::count(str->begin(), str->end(), ' '))
		throw "invalid symbols";
}

void port_conf(std::string str, int count_tab)
{
	if (count_tab != 1)
		throw "invalide tabulation";
	if (str[4] != ':')
		throw "expected ':' after key name";
	str.erase(0, 5);
	del_space_chars(&str);
	if (std::count(str.begin(), str.end(), ' '))
		throw "invalid symbols";
	servinfo.setPort(str);
}

void parse_inside_loc(std::string str, int count_tab)
{
	std::map < std::string, std::map<std::string, std::string> > tmp;

	if (count_tab != 2)
		throw "invalide tabulation";
	if (!str.compare(0, 4, "root"))
	{
		if (str[4] != ':')
			throw "expected ':' after key name";
		str.erase(0, 5);
		del_space_chars(&str);
		tmp[servinfo.getLocPath()]["root"] = str;
		servinfo.setMap(tmp);
	}
	else if (!str.empty())
		throw "invalid symbols";
}

void location_conf(std::string str, int count_tab)
{
	std::string loc_path_tmp;

	if (servinfo.getFlagLoc() == false)
	{
		if (count_tab != 1)
			throw "invalide tabulation";
		if (str[8] != ':')
			throw "expected ':' after key name";
		str.erase(0, 9);
		del_space_chars(&str);
		for (size_t i = 0; str[i] != ' '; i++)
		{
			loc_path_tmp.resize(i + 1);
			loc_path_tmp[i] = str[i];
		}
		servinfo.setLocPath(loc_path_tmp);
		str.erase(0, loc_path_tmp.size());
		del_space_chars(&str);
		if (str.size() > 1)
			throw "invalid symbols";
		servinfo.setFlagLoc(true);
	}
	else
		parse_inside_loc(str, count_tab);
}

void parse(char *line)
{
	int count_tab;
	static int flag_serv = 0;
	std::string str = line;

	count_tab = std::count(str.begin(), str.end(), '\t');
	while(str[0] == '\t')
		str.erase(0, 1);
	if (!str.compare(0, 6, "server") && !flag_serv)
	{
		head_conf(str, count_tab);
		flag_serv = 1;
	}
	else if (!str.compare(0, 4, "host"))
	{
		host_conf(&str, count_tab);
		servinfo.setHost(str.substr(0, str.find(' ')));
	}
	else if (!str.compare(0, 4, "port"))
	{
		host_conf(&str, count_tab);
		servinfo.setPort(str.substr(0, str.find(' ')));
	}
	else if ((!str.compare(0, 8, "location") || servinfo.getFlagLoc() == true))
	{
		location_conf(str, count_tab);
	}

	// else if (!str.empty())
	// 	throw "invalid symbols";
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
	std::cout << "host: " << servinfo.getHost() << std::endl;
	std::cout << "port: " << servinfo.getPort() << std::endl;
	std::cout << "root: " << servinfo.getMap()[servinfo.getLocPath()]["root"] << std::endl;
}

int main ()
{
	read_conf();
	return (0);
}
