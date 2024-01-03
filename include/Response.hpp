#pragma once

#include <ft_common.h>

enum e_status_code
{
	STATUS_SUCCESS = 200,
	STATUS_MOVED = 301,
	STATUS_NOT_FOUND = 404,
}

class Response
{
	public :
		Response();
		bool	isFinished() const;
		void	setFinished(enum e_status_code);
	private:
		bool		_isFinished;
		int			_statusCode;
		std::string	_content;
		std::string	_contentType;
		std::string	_finalMsg;
};
