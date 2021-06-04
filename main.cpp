/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 22:30:19 by wphylici          #+#    #+#             */
/*   Updated: 2021/06/04 06:16:03 by wphylici         ###   ########.fr       */
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
int pos_loc = -1;

void del_space_chars(std::string *str)
{
	while((*str)[0] == '\t' || (*str)[0] == ' ')
		str->erase(0, 1);
}

void field_valid_check(std::string *str, int count_tab)
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

void parse_2nd_level(std::string str, int count_tab)
{
	std::string tmp;
	static std::map < std::string, std::map<std::string, std::string> > tmp_map_loc;
	std::string second_level[5] = {"root", "index", "method", "cgi_extension", "autoindex"};

	tmp = str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':'));
	for (size_t i = 0; i < 5; i++)
	{
		if (second_level[i] == tmp)
		{
			field_valid_check(&str, count_tab);
			tmp_map_loc[servinfo.getLocPath(pos_loc)][tmp] = str.substr(0, str.find(' '));
			servinfo.setMapLoc(tmp_map_loc);
			return ;
		}
	}
	if (!tmp.empty())
		throw "invalid symbols";
}

void parse_1st_level(std::string str, int count_tab)
{
	std::string tmp;
	static std::map <std::string, std::string> tmp_map_1lvl;
	std::string first_level[6] = {"server", "server_name", "host", "port", "max_body_size", "location"};

	tmp = str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':'));
	for (size_t i = 0; i < 6; i++)
	{
		if (first_level[i] == tmp)
		{
			if (tmp == "location")
			{
				field_valid_check(&str, count_tab);
				servinfo.setLocPath(str, pos_loc);
				servinfo.setFlagLoc(true);
				return ;
			}
			field_valid_check(&str, count_tab);
			tmp == "server" && !str.empty() ? throw "invalid symbols" : 0;
			tmp_map_1lvl[tmp] = str.substr(0, str.find(' '));
			servinfo.setMapHeadFields(tmp_map_1lvl);
			return ;
		}
	}
	if (!tmp.empty())
		throw "invalid symbols";
}

void parse(char *line)
{
	int count_tab = 0;
	std::string str = line;

	for (size_t i = 0; str[i] == '\t'; i++)
		count_tab++;
	del_space_chars(&str);
	if (str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':')) == "location")
	{
		pos_loc++;
		servinfo.setFlagLoc(false);
	}
	if (servinfo.getFlagLoc() == false)
		parse_1st_level(str, count_tab);
	else
		parse_2nd_level(str, count_tab);
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
	std::cout << "server_name: " << servinfo.getMapHeadFields()["server_name"] << std::endl;
	std::cout << "host: " << servinfo.getMapHeadFields()["host"] << std::endl;
	std::cout << "port: " << servinfo.getMapHeadFields()["port"] << std::endl;
	std::cout << "max_body_size: " << servinfo.getMapHeadFields()["max_body_size"] << std::endl << std::endl;

	for (size_t i = 0; i <= pos_loc; i++)
	{
		std::cout << "location path: " << servinfo.getLocPath(i) << std::endl;
		std::cout << "	root: " << servinfo.getMapLoc()[servinfo.getLocPath(i)]["root"] << std::endl;
		std::cout << "	index: " << servinfo.getMapLoc()[servinfo.getLocPath(i)]["index"] << std::endl;
		std::cout << "	method: " << servinfo.getMapLoc()[servinfo.getLocPath(i)]["method"] << std::endl;
		std::cout << "	cgi_extension: " << servinfo.getMapLoc()[servinfo.getLocPath(i)]["cgi_extension"] << std::endl;
		std::cout << "	autoindex: " << servinfo.getMapLoc()[servinfo.getLocPath(i)]["autoindex"] << std::endl;
	}


}

int main ()
{
	read_conf();
	return (0);
}
