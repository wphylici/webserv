#pragma once

/* Повышенной интенсивности (High Intensty) */

# define HI_BLACK		"\e[0;90m"
# define HI_RED			"\e[0;91m"
# define HI_GREEN		"\e[0;92m"
# define HI_YELLOW		"\e[0;93m"
# define HI_BLUE		"\e[0;94m"46
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
#include <vector>
#include <sstream>
#include <string>

class Server
{
	public:
		Server();
		~Server();
		Server(Server const &server);

		// set
		void setFlagLoc(bool FlagLoc);
		void setLocPath(std::string LocPath);
		void seMethods(std::map < int, std::map<int, std::string> > methods);
		void setMapLoc(std::map < std::string, std::map<std::string, std::string> > map);
		void setMapHeadFields(std::map <std::string, std::string> map_head_fields);
		void setErrorPages(std::map <std::string, std::string> error_pages);

		//get
		bool getFlagLoc(void);
		std::vector <std::string> getValueLocPath();
		std::vector < std::vector <std::string> > getLocPath();
		std::map < int, std::map<int, std::string> > &getMethods(void);
		std::map < std::string, std::map<std::string, std::string> > &getMapLoc(void);
		std::map <std::string, std::string> &getErrorPages(void);
		std::map <std::string, std::string> &getMapHeadFields(void);

	private:
		Server &operator=(Server const &server);

		bool _flag_loc;
		std::vector < std::vector <std::string> > _loc_path;
		std::map < int, std::map<int, std::string> > _methods;
		std::map <std::string, std::string> _error_pages;
		std::map < std::string, std::map<std::string, std::string> > _location;
		std::map <std::string, std::string> _head_fields;
};

extern int g_pos_loc;
extern int g_pos_serv;
extern std::vector <Server *> g_servinfo;