#include <ft_common.h>

Response::Response()
	:	_finished(false), _statusCode(STATUS_CODE_SUCCESS)
{ }

bool	Response::isFinished() const
{
	return (_finished);
}

void	Response::setFinished(enum e_status_code code)
{
	_statusCode = code;
	_isFinished = true;
}