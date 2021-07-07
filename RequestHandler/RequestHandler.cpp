//
// Created by enoelia on 08.06.2021.
//

#include "../includes/RequestHandler.hpp"

RequestHandler::RequestHandler(){
	_response = new Response();

	_method = 0;
	_flagChuked = 0;
	// _flagParsed = 0;
	_wrongMethods = 0;
	_badContentSize = 0;
	_wrongHTTPVersion = 0;
	_badRequest = 0;
	_strsBody.push_back(std::vector<std::string>());
}

RequestHandler::RequestHandler(const RequestHandler &other){
	*this = other;
}

RequestHandler::~RequestHandler() {
	delete _response;
}

RequestHandler&	RequestHandler::operator=( const RequestHandler &other){
	if (this != &other) {
		;
	}
	return (*this);
}

int					RequestHandler::getMethod() const {
	return _method;
}

void				RequestHandler::setMethod(int method) {
	_method = method;
}

Server*				RequestHandler::getServer() const {
	return _server;
}

void				RequestHandler::setServer(Server *server) {
	_server = server;
}

const std::string&	RequestHandler::getRawRequest() const {
	return _rawRequest;
}

void				RequestHandler::setRawRequest(const std::string &rawRequest) {
	_rawRequest = rawRequest;
}

void RequestHandler::pushBody(std::string strBody)
{
	this->_strsBody[this->_strsBody.size() - 1].push_back(strBody);
}

const std::string &RequestHandler::getAnswer() const {
	return _answer;
}

unsigned long RequestHandler::getBytesToSend() const {
	return _bytesToSend;
}

void				RequestHandler::testPrint()
{
	std::cout << std::endl;

	std::cout << "_wrongMethods: "<< _wrongMethods << std::endl;
	std::cout << "_badContentSize: " << _badContentSize << std::endl;
	std::cout << "_wrongHTTPVersion: " << _wrongHTTPVersion << std::endl;
	std::cout << "_badRequest: " << _badRequest << std::endl;

	std::cout << std::endl;

	std::cout << "_method: " << _method << std::endl;
	std::cout << "_url: " << _url << std::endl;

	std::cout << std::endl;

	std::cout << "Host: " << _headers["Host"] << std::endl;
	std::cout << "Content-Type: " << _headers["Content-Type"] << std::endl;
	std::cout << "Content-Length: " << _headers["Content-Length"] << std::endl;
	std::cout << "Transfer-Encoding: " << _headers["Transfer-Encoding"] << std::endl;
	std::cout << "test: " << _headers["lol"] << std::endl;

	std::cout << std::endl;

	std::cout << "_body: " << std::endl << std::endl << _body << std::endl;
}

int					RequestHandler::checkNewPartOfRequest(char *partOfRequest){
	_rawRequest += partOfRequest;

	int ret = 0;

	if ((ret = parseRequest())) {

		//парсинг запроса на готовность к обработке(наличие \r\n\r\n) + заполнение полей
		std::cout << ret << std::endl;
		return 0;
		//1) ищем \r\n\r\n
		//2) если нашли то контен сайз и сравнить с сайзом configa(RequestHandler._server->_max_body_size), если ошибка уставint _badContentSize;
		//3) если не нашли контент сайз то это весь запрос вернуть (1) _url = /index.html
		//4) PUT, DELETE, POST, GET указано что то другое ставишь флаг _wrongMethods
		//
	}
	std::cout << ret << std::endl;
	//  else {
	// 	prepareResponse();
	// 	_rawRequest = "";
		return 1;
	// }
}

int					RequestHandler::checkFirstStr(std::cmatch result, std::regex rex)
{
	std::string first_str;

	if (_rawRequest.find("\r\n") != std::string::npos)
	{
		first_str = _rawRequest.substr(0, _rawRequest.find("\r\n") + 2);
		if (std::regex_match(first_str.c_str(), result, rex))
		{
			if (result[1] == "GET")
				_method = 1;
			else if (result[1] == "POST")
				_method = 2;
			else if (result[1] == "DELETE")
				_method = 3;
			else if (result[1] == "PUT")
				_method = 4;
			else
			{
				_wrongMethods = 1;
				return (-1);
			}
			_url = result[2];
			if (result[3] != "HTTP/1.1")
			{
				_wrongHTTPVersion = 1;
				return (-1);
			}
			_flagParsed = 1;
		}
		else if (!first_str.empty())
		{
			_badRequest = 1;
			return (-1);
		}
	}
	return (0);
}

int					RequestHandler::checkDoubleFields(std::cmatch result)
{
	char *tmp = (char *)result[1].str().c_str();

	if (islower(tmp[0]))
		tmp[0] = tmp[0] - 32;
	for (size_t i = 1; i < result[1].length(); i++)
	{
		if (!islower(tmp[i]))
			tmp[i] = tmp[i] + 32;
	}
	if (_headers.find(tmp) != _headers.end())
		return (-1);
	return (0);
}

int					RequestHandler::checkHeaders(std::cmatch result, std::regex rex)
{
	std::string oneHeader;
	std::string rawRequestTmp = _rawRequest.substr(0, _rawRequest.find("\r\n\r\n") + 4);

	rawRequestTmp.erase(0, rawRequestTmp.find("\r\n") + 2);
	while (rawRequestTmp != "\r\n")
	{
		oneHeader = rawRequestTmp.substr(0, rawRequestTmp.find("\r\n") + 2);
		if (!std::regex_match(oneHeader.c_str(), result, rex) || checkDoubleFields(result))
		{
			_badRequest = 1;
			return (-1);
		}
		_headers[result[1]] = result[2];
		rawRequestTmp.erase(0, rawRequestTmp.find("\r\n") + 2);
	}
	return (0);
}

int					RequestHandler::checksAfterParse(std::cmatch result, std::regex rex)
{
	if ((_headers.find("Content-Length") != _headers.end() ||
		_headers.find("Transfer-Encoding") != _headers.end()) &&
		_rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4).empty())
		return (0);
	if ((_headers.find("Content-Length") == _headers.end() &&
		_headers.find("Transfer-Encoding") == _headers.end() &&
		!_rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4).empty()) ||
		_headers["Transfer-Encoding"] != "chunked")
	{
		_badRequest = 1;
		return (-1);
	}
	if (_headers.find("Transfer-Encoding") != _headers.end() && _method != 1)
	{
		int count = 0;
		std::string oneStrBody;
		std::regex rexCheckEnd("(0\r\n\r\n)");
		std::string tmpBody = _rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4);

		if (!std::regex_search(tmpBody.c_str(), result, rexCheckEnd))
		{
			_badRequest = 1;
			return (-1);
		}
		while (tmpBody != "\r\n")
		{
			oneStrBody = tmpBody.substr(0, tmpBody.find("\r\n") + 2);
			if (!std::regex_match(oneStrBody.c_str(), result, rex))
			{
				_badRequest = 1;
				return (-1);
			}
			tmpBody.erase(0, tmpBody.find("\r\n") + 2);
			pushBody(result[1]);
			count++;
		}
		for (size_t i = 0; i < count - 1; i += 2)
		{
			if (strtol(_strsBody[0][i].c_str(), NULL, 16) != _strsBody[0][i + 1].size())
			{
				_badRequest = 1;
				return (-1);
			}
		}
		_body = _rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4);
	}
	else if (_headers.find("Content-Length") != _headers.end() && _method != 1)
	{
		if ((strtol(_headers["Content-Length"].c_str(), NULL, 10) > _server->getMaxBodySize() ||
			strtol(_headers["Content-Length"].c_str(), NULL, 10) == 0L ||
			strtol(_headers["Content-Length"].c_str(), NULL, 10) == ULONG_MAX ||
			strtol(_headers["Content-Length"].c_str(), NULL, 10) < 0) &&
			_headers["Content-Length"] != "0")
		{
			_badContentSize = 1;
			return (-1);
		}
		if (_rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4).size() <
			std::atoi(_headers["Content-Length"].c_str()))
		{
			_body = _rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4);
			return (0);
		}
		else
			_body = _rawRequest.substr(_rawRequest.find("\r\n\r\n") + 4, std::atoi(_headers["Content-Length"].c_str()));
	}
	return (1);
}

int					RequestHandler::parseRequest()
{
	std::cmatch result;
	std::regex rex_first_str("(^[\r\n]+|[A-Z]+)(?: )"
							"([\\w\\:\\.\\/-]+)"
							"(?: )"
							"(HTTP/+[\\d]+\\.[\\d]+)"
							"(?:\r\n)");

	std::regex rex_headers("([\\w-]+)"
						   "(?:\\:)"
						   "(?:[ ]{1}|)"
						   "([\\w\\d\\:\\.\\/-]+)"
						   "(?:\r\n)");

	std::regex rex_body("([.]*[^\r\n]+)"
						"(?:\r\n)");

	for (size_t i = 0; _rawRequest[0] == '\r' && _rawRequest[1] == '\n';)
		_rawRequest.erase(0, 2);
	if (_rawRequest.find("\r\n\r\n") != std::string::npos)
	{
		if (checkFirstStr(result, rex_first_str) == -1)
			return (-1);
		if (checkHeaders(result, rex_headers) == -1)
			return (-1);
		return (checksAfterParse(result, rex_body));
	}
	return (0);


	// //<Заглушка>
	// _method = GET;
	// _url = "/index3.html";
	// _headers.insert( std::pair<std::string, std::string>("Content-Length","555"));
	// //</Заглушка>
}

void				RequestHandler::prepareResponse(){
	urlToPath();
	std::ifstream file(_filePath.c_str());
	std::stringstream buffer;

	std::cout << _filePath.c_str() << std::endl;
	//std::cout << buffer.str() << std::endl << std::endl;
	buffer << file.rdbuf();
	if (!file){
		response404();
	} else {
		//цикл по локейшном для определения соотвсевующего
		// если урл не указывает на файл смотерть в индекс, если нет индекса, искать файл индекс.хтмл если файла нет, смотреть autoindex, если и его нет то ошибка 403
		// елси файла нет, вернуть 404
		// если метод не соответвует доступном ошибку 405 и указать досутпные в заголовке Allow
		if (_method == GET) {
			_response->setServerAnswer("HTTP/1.1 200 OK\n");
			_response->setUpBody(buffer);
			_response->setUpHeaders();
		}
	}
	_answer = _response->receiveAnswer();
	_bytesToSend = _answer.length();
}

void				RequestHandler::urlToPath(){
	//<Заглушка>
	_filePath = "/home/enoelia/01_21school/webserv/www" + _url;
	//</Заглушка>
}

void				RequestHandler::response404(){
	std::ifstream file(_server->get404Path().c_str());
	std::stringstream buffer;

	_response->setServerAnswer("HTTP/1.1 404 Not Found\n");
	if (!file) {
		buffer << "<!DOCTYPE html>\n<html><title>404</title><body>Error 404 Not Found</body></html>\n";
		_response->setUpBody(buffer);
		_response->setUpHeaders();
	} else {
		buffer << file.rdbuf();
	}
	//	res += "HTTP/1.1 200 OK\nContent-Length: ";
	//	res += std::to_string(str.length());
	//	res += "\r\n\r\n";
	//	res += str;
	//	//res += "<!DOCTYPE html>\n<html><title>40404</title><body>There was an error finding your error page</body></html>\n";
	//	res +=  "\r\n\r\n";
	//	std::cout << res << std::endl;
	//	writeValue = write(_socketFd, res.c_str(), res.length());
	//	std::cout << "write return : " <<writeValue << std::endl;
	//	_status = CLOSE;
}
