#pragma once

/* Повышенной интенсивности (High Intensty) */

# define HI_BLACK		"\e[0;90m"
# define HI_RED			"\e[0;91m"
# define HI_GREEN		"\e[0;92m"
# define HI_YELLOW		"\e[0;93m"
# define HI_BLUE		"\e[0;94m"
# define HI_PURPLE		"\e[0;95m"
# define HI_CYAN		"\e[0;96m"
# define HI_WHITE		"\e[0;97m"

/* Жирный шрифт с повышенной интенсивностью (Bold High Intensty) */

# define BHI_BLACK		"\e[1;90m"
# define BHI_RED		"\e[1;91m"
# define BHI_GREEN		"\e[1;92m"
# define BHI_YELLOW 	"\e[1;93m"
# define BHI_BLUE		"\e[1;94m"
# define BHI_PURPLE		"\e[1;95m"
# define BHI_CYAN		"\e[1;96m"
# define BHI_WHITE		"\e[1;97m"

/* Сброс */

# define RESET			"\e[0m"

#include <iostream>
#include <map>

class Server
{
	public:
		Server();
		~Server();
		// void Server::setMap(std::string loc_name, std::string field, std::string value);
		void setServerName(std::string ServerName);
		void setHost(std::string Host);
		void setPort(std::string Port);
		void setBodySize(std::string BodySize);
		void setFlagLoc(bool FlagLoc);
		void setLocPath(std::string LocPath);
		void setMap(std::map < std::string, std::map<std::string, std::string> > map);
		std::map < std::string, std::map<std::string, std::string> > getMap(void);
		std::string getServerName(void);
		std::string getHost(void);
		std::string getPort(void);
		std::string getBodySize(void);
		bool getFlagLoc(void);
		std::string getLocPath(void);

	private:
		bool flag_loc;
		std::string server_name;
		std::string host;
		std::string port;
		std::string max_body_size;
		std::string error_pages;
		std::string loc_path;
		std::map < std::string, std::map<std::string, std::string> > location;
};
