/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asekkak <asekkak@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 12:39:23 by mbouyahy          #+#    #+#             */
/*   Updated: 2024/01/02 12:16:42 by asekkak          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <ft_common.h>
 #include "Location.hpp"
 
 class Location;

class   Response
{
    public:
        Response();
        ~Response();
        
        std::string     _status_code;
        std::string     _status_message;
        
        std::string     _contentLenght;
        std::string     _contentType;
        std::string     _host;
        
        std::string _body;

        bool _isDone;
      
};
 
#endif