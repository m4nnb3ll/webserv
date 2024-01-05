#pragma once

#include <ft_common.h>
#include "Request.hpp"

class	Request;

enum e_resource_type
{
	RT_FILE,
	RT_DIR
};

enum e_status_code
{
	STATUS_SUCCESS = 200,
	STATUS_MOVED = 301,
	STATUS_NOT_FOUND = 404,
	STATUS_NOT_ALLOWED = 405
};


class Response
{
	public :
		Response(Request* request);
	private:
		Request					*_request;
		bool					_isFinished;
		int						_statusCode;
		std::string				_resource;
		enum e_resource_type	_resourceType;
		std::string				_content;
		std::string				_contentType;
		std::string				_finalMsg;

		// Methods
		// bool		isError();
		void		_finishWithCode(enum e_status_code code);
		std::string	_genStatusMsg(int code);
		void		_checkLocation();
		void		_checkMethod();
		void		_checkResource();
		void		_handleGet();
		void		_handlePost();
		void		_handleDelete();
};
