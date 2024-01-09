/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelayad <abelayad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:10:04 by abelayad          #+#    #+#             */
/*   Updated: 2024/01/09 16:11:54 by abelayad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_common.h>
#include "Request.hpp"
#include "Cgi.hpp"

class	Request;

enum e_resource_type
{
	RT_FILE,
	RT_DIR
};

enum e_status_code
{
	STATUS_SUCCESS = 200,
	STATUS_CREATED = 201,
	STATUS_NO_CONTENT = 204,
	STATUS_MOVED = 301,
	STATUS_BAD_REQUEST = 400,
	STATUS_FORBIDDEN = 403,
	STATUS_NOT_FOUND = 404,
	STATUS_NOT_ALLOWED = 405,
	STATUS_CONFLICT = 409,
	STATUS_LENGTH_REQUIRED = 411,
	STATUS_TOO_LARGE = 413,
	STATUS_URI_TOO_LONG = 414,
	STATUS_INTERNAL_ERR = 500,
	STATUS_NOT_IMPLEMENTED = 501
};


class Response
{
	public :
		Response(Request* request);
		bool		isFinished() const;
		std::string	getContent() const;
		std::string	getResource() const;
		Request	*getRequest() const;
	private:
		Request					*_request;
		bool					_isFinished;
		int						_statusCode;
		std::string				_resource;
		enum e_resource_type	_resourceType;
		std::string				_content;
		std::string				_contentType;
		std::string				_finalMsg;
		std::string				_index;

		// Methods
		// bool		isError();
		void		_handleGet();
		void		_handlePost();
		void		_handleDelete();
		std::string	_getStatusCodeMsg();
		std::string	_getErrFilePath();
		void		_errorCheck();
		void		_finishWithCode(enum e_status_code code);
		void		_checkLocation();
		void		_redirect();
		void		_checkMethod();
		void		_checkResource();
		bool		_dirHasIndexFiles(std::vector<std::string> indexes);
		void		_runCgi(std::string cgiPath, std::string filePath);
		bool		_extensionMatch(const std::string& extension, const std::string& filename);
		void		_checkCgi();
		void		_checkDirURI();
		void		_returnFile(std::string filename);
		void		_returnDirAutoIndex();
		void		_checkAutoIndex();
		void		_uploadFile();
		void		_deleteFile();
		void		_tryDeleteDir();
		std::string	_strToLower(const std::string& input);
		std::string	_getMimeType(const std::string& filename);

};
