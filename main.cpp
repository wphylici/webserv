/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wphylici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 22:30:19 by wphylici          #+#    #+#             */
/*   Updated: 2021/06/22 03:15:07 by wphylici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <fcntl.h>
#include "get_next_line/get_next_line.h"

int g_pos_loc = -1;
int g_pos_serv = 0;
std::vector <Server *> g_servinfo;

void del_space_chars(std::string *str)
{
	while((*str)[0] == '\t' || (*str)[0] == ' ')
		str->erase(0, 1);
}

void check_tabs(std::string *str, int count_tab)
{
	if (g_servinfo[g_pos_serv]->getFlagLoc() == true && count_tab != 2)
		throw "invalide tabulation";
	else if (((str->substr(0, str->find(':')) == "server" && count_tab > 0) ||
		(str->substr(0, str->find(':')) != "server" && count_tab != 1) ||
		(str->substr(0, str->find(':')) == "location" && count_tab != 1)) &&
		g_servinfo[g_pos_serv]->getFlagLoc() == false)
		throw "invalide tabulation";
}

void field_valid_check(std::string *str, int count_tab)
{
	int flag_method_and_errors = 0;
	str->substr(0, str->find(':') == -1 ? str->find(' ') : str->find(':')) == "method" ||
	str->substr(0, str->find(':') == -1 ? str->find(' ') : str->find(':')) == "error_page" ?
	flag_method_and_errors = 1 : 0;
	check_tabs(str, count_tab);
	if (str->substr(0, str->find(' ')).find(':') == str->npos)
		throw "expected ':' after key name";
	str->erase(0, str->substr(0, str->find(' ')).find(':') + 1);
	del_space_chars(str);
	std::string tmp = str->substr(str->substr(0,
	str->find(' ') < 0 ? str->find('\t') : str->find(' ')).size(), str->size());
	del_space_chars(&tmp);
	if (!tmp.empty() && !flag_method_and_errors)
		throw "invalid symbols";
}

void parse_2nd_level(std::string str, int count_tab)
{
	std::string tmp;
	std::string second_level[5] = {"root", "index", "method", "cgi_extension", "autoindex"};

	tmp = str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':'));
	g_servinfo[g_pos_serv]->getMapLoc()[g_servinfo[g_pos_serv]->getValueLocPath()[g_pos_loc]].count(tmp) == 1 ? throw "double field" : 0;
	for (size_t i = 0; i < 5; i++)
	{
		if (tmp == "method")
		{
			char *tmp;

			field_valid_check(&str, count_tab);
			tmp = strtok((char *)str.c_str(), " ");
			for (size_t i = 0; tmp; i++)
			{
				for (size_t i = 0; g_servinfo[g_pos_serv]->getMethods()[g_pos_loc].find(i) !=
					g_servinfo[g_pos_serv]->getMethods()[g_pos_loc].end(); i++)
				{
					if (g_servinfo[g_pos_serv]->getMethods()[g_pos_loc][i] == tmp)
						throw "double method value";
				}
				g_servinfo[g_pos_serv]->getMethods()[g_pos_loc][i] = std::string(tmp);
				tmp = strtok(NULL, " ");
			}
			return ;
		}
		if (second_level[i] == tmp)
		{
			field_valid_check(&str, count_tab);
			g_servinfo[g_pos_serv]->getMapLoc()[g_servinfo[g_pos_serv]->getValueLocPath()[g_pos_loc]][tmp] = str.substr(0, str.find(' '));
			return ;
		}
	}
	if (!tmp.empty())
		throw "invalid symbols";
}

void parse_1st_level(std::string str, int count_tab)
{
	std::string tmp;
	std::string first_level[7] = {"server", "server_name", "host", "port", "max_body_size", "location", "error_page"};

	tmp = str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':'));
	g_servinfo[g_pos_serv]->getMapHeadFields().count(tmp) == 1 ? throw "double field" : 0;
	for (size_t i = 0; i < 7; i++)
	{
		if (tmp == "error_page")
		{
			char *tmp;
			char *num_error;

			field_valid_check(&str, count_tab);
			tmp = strtok((char *)str.c_str(), " ");
			num_error = tmp;
			tmp = strtok(NULL, " ");
			g_servinfo[g_pos_serv]->getErrorPages()[num_error] = std::string(tmp);

			if ((tmp = strtok(NULL, " ")))
				throw "invalid symbols";
			return ;
		}
		if (tmp == "location")
		{
			field_valid_check(&str, count_tab);
			for (size_t i = 0; i < g_pos_loc; i++)
			{
				if (g_servinfo[g_pos_serv]->getLocPath()[0][i] == str)
					throw "double location path";
			}
			g_servinfo[g_pos_serv]->setLocPath(str);
			g_servinfo[g_pos_serv]->setFlagLoc(true);
			return ;
		}
		if (first_level[i] == tmp)
		{
			field_valid_check(&str, count_tab);
			tmp == "server" && !str.empty() ? throw "invalid symbols" : 0;
			g_servinfo[g_pos_serv]->getMapHeadFields()[tmp] = str.substr(0, str.find(' '));
			return ;
		}
	}
	if (!tmp.empty())
		throw "invalid symbols";
}

void check_delimiter(int count_serv)
{
	if (count_serv - 1 > g_pos_serv)
		throw "expected delimiter: '---'";

	else if (count_serv - 1 < g_pos_serv)
		throw "extra delimiter character '---'";
}

void parse(char *line)
{
	int count_tab = 0;
	static int count_serv = 1;
	std::string str = line;

	for (size_t i = 0; str[i] == '\t'; i++)
		count_tab++;
	del_space_chars(&str);
	if (str .substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':')) == "server")
		check_delimiter(count_serv++);
	if (str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':')) == "location")
	{
		g_pos_loc++;
		g_servinfo[g_pos_serv]->setFlagLoc(false);
	}
	if (str.substr(0, str.find(':') == -1 ? str.find(' ') : str.find(':')) == "---")
	{
		g_pos_serv++;
		g_pos_loc = -1;
		g_servinfo.push_back(new Server());
	}
	else if (g_servinfo[g_pos_serv]->getFlagLoc() == false)
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
	for (size_t j = 0; j <= g_pos_serv; j++)
	{
		std::cout << "server_name: " << g_servinfo[j]->getMapHeadFields()["server_name"] << std::endl;
		std::cout << "host: " << g_servinfo[j]->getMapHeadFields()["host"] << std::endl;
		std::cout << "port: " << g_servinfo[j]->getMapHeadFields()["port"] << std::endl;
		std::cout << "max_body_size: " << g_servinfo[j]->getMapHeadFields()["max_body_size"] << std::endl << std::endl;
		std::cout << "error_page: " << g_servinfo[j]->getErrorPages()["404"] << std::endl;
		std::cout << "error_page: " << g_servinfo[j]->getErrorPages()["403"] << std::endl << std::endl;

		for (size_t i = 0; i <= g_pos_loc; i++)
		{
			std::cout << "location path: " << g_servinfo[j]->getValueLocPath()[i] << std::endl;
			std::cout << "	root: " << g_servinfo[j]->getMapLoc()[g_servinfo[j]->getValueLocPath()[i]]["root"] << std::endl;
			std::cout << "	index: " << g_servinfo[j]->getMapLoc()[g_servinfo[j]->getValueLocPath()[i]]["index"] << std::endl;

			std::cout << "	method: ";
			for (size_t i = 0; g_servinfo[j]->getMethods()[g_pos_loc].find(i) !=
				g_servinfo[j]->getMethods()[g_pos_loc].end(); i++)
				std::cout << g_servinfo[j]->getMethods()[g_pos_loc][i] << " ";
			std::cout << std::endl;

			std::cout << "	cgi_extension: " << g_servinfo[j]->getMapLoc()[g_servinfo[j]->getValueLocPath()[i]]["cgi_extension"] << std::endl;
			std::cout << "	autoindex: " << g_servinfo[j]->getMapLoc()[g_servinfo[j]->getValueLocPath()[i]]["autoindex"] << std::endl;

			std::cout << std::endl << "---" << std::endl << std::endl;
		}
	}
}

int main ()
{
	g_servinfo.push_back(new Server());
	read_conf();
	for (size_t i = 0; i <= g_pos_serv; i++)
		delete g_servinfo[i];
	return (0);
}
