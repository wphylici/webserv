#include "Connection.hpp"

Connection::Connection(int listenSocketFd, Server *server) :
	_status(READ),
	_addrlen(sizeof(_addr)),
	_requestHandler(new RequestHandler(server)),
	_alreadySent(0){
	int flags;

	_socketFd = accept(listenSocketFd,(sockaddr*)&_addr, &_addrlen);
	if (_socketFd == -1){
		throw Exceptions::AcceptException();
	}
	//nonblocking mode
	if ((flags = fcntl(_socketFd, F_GETFL)) == -1)
		throw Exceptions::FcntlException();
	if ((fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK) == -1))
		throw Exceptions::FcntlException();
}

Connection::Connection(const Connection &other){
	*this = other;
}

Connection::~Connection() {
	delete _requestHandler;
}

Connection&	Connection::operator=( const Connection &other){
	if (this != &other) {
		_status = other._status;
		_socketFd = other._socketFd;
		_addr = other._addr;
		_addrlen = other._addrlen;
	}
	return (*this);
}

int			Connection::getStatus() const {
	return _status;
}

void		Connection::setStatus(int status) {
	_status = status;
}

int			Connection::getSocketFd() const {
	return _socketFd;
}

void		Connection::setSocketFd(int fd) {
	_socketFd = fd;
}

void		Connection::readFromSocket() {
	int readValue;
	char buf[BUFFER_SIZE];

	if ((readValue = read(_socketFd, buf, BUFFER_SIZE + 1)) == -1) {
		close(_socketFd);
		_status = CLOSE; //remove client
	}
	if (readValue > 0){
		try {
			if (_requestHandler->checkNewPartOfRequest(buf, readValue)) {
				_status = WRITE;
			} else {
				close(_socketFd);
				_status = CLOSE;
			}
		} catch (std::exception &e) {
			_status = CLOSE;
			std::cerr << e.what() << std::endl;
		}

	} else {
		close(_socketFd);
		_status = CLOSE;
	}
}

void		Connection::writeToSocket(){
	std::string tmp;

	tmp = _requestHandler->getAnswer();
	_alreadySent += write(_socketFd, tmp.c_str() + _alreadySent, tmp.length() - _alreadySent);
	if (_alreadySent == -1) {
		close(_socketFd);
		_status = CLOSE; // remove client
	}
	if (_alreadySent == _requestHandler->getBytesToSend()){
		close(_socketFd);
		_status = CLOSE;
	}
}
