#ifndef SERVER_REQUESTHANDLER_HPP
# define SERVER_REQUESTHANDLER_HPP
# include <iostream>
# include <fstream>
# include <map>
# include <regex>
# include "Server.hpp"
# include "Response.hpp"
# define GET 1
# define POST 2
# define DELETE 3
# define PUT 4

class Server;
class Response;

class RequestHandler {
private:
	std::string									_rawRequest;
	Server*										_server;

	int											_method;
	std::string									_body;
	std::string									_url;
	std::map<std::string,std::string>			_headers;
	std::vector < std::vector <std::string> >	_strsBody;

	std::string									_filePath;
	Response									*_response;
	std::string									_answer;
	unsigned long								_bytesToSend; 	   // что это ?

	int											_flagParsed;
	int											_badContentSize;
	int 										_flagChuked;
	int											_wrongMethods;     // 405
	int											_wrongHTTPVersion; // 505
	int											_badRequest;       // 400

public:
	RequestHandler();
	RequestHandler(const RequestHandler &);
	virtual ~RequestHandler();
	RequestHandler	&operator=(const RequestHandler &);

	int					getMethod() const;
	void				setMethod(int method);
	Server				*getServer() const;
	void				setServer(Server *server);
	const				std::string &getRawRequest() const;
	void				setRawRequest(const std::string &rawRequest);
	const std::string& 	getAnswer() const;
	unsigned long		getBytesToSend() const;
	void 				pushBody(std::string strBody);

	int					checkNewPartOfRequest(char *partOfRequest);
	int					parseRequest();//парсинг запроса на готовность к обработке(наличие \n\r\n\r) + заполнние полей
	int					checksAfterParse(std::cmatch result, std::regex rex_body);
	int					checkDoubleFields(std::cmatch result);
	int					checkHeaders(std::cmatch result, std::regex rex);
	int					checkFirstStr(std::cmatch result, std::regex rex);
	void				prepareResponse();
	void				urlToPath();
	void				response404();


	void				testPrint(); //удалить потом
};


#endif //SERVER_REQUESTHANDLER_HPP
